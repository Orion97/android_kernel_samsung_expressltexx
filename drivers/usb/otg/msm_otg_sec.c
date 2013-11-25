/*
 * drivers/usb/otg/msm_otg_sec.c
 *
 * Copyright (c) 2012, Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/gpio.h>
#include <linux/host_notify.h>
#ifdef CONFIG_USB_SWITCH_FSA9485
#include <linux/i2c/fsa9485.h>
#endif

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s %d: " fmt, __func__, __LINE__

#define SM_WORK_TIMER_FREQ	(jiffies + msecs_to_jiffies(1000))

#if 0
#define MSM_OTG_LATE_INIT
#define MSM_OTG_DIRECT_VBUS
#endif

enum usb_notify_state {
	ACC_POWER_ON = 0,
	ACC_POWER_OFF,
	ACC_POWER_OVER_CURRENT,
};

static int msm_otg_reset(struct usb_phy *phy);
static void msm_otg_init_sm(struct msm_otg *motg);
static void msm_hsusb_vbus_power(struct msm_otg *motg, bool on);
static int ulpi_write(struct usb_phy *phy, u32 val, u32 reg);
static int ulpi_read(struct usb_phy *phy, u32 reg);
static void msm_otg_notify_charger(struct msm_otg *motg, unsigned mA);
static void msm_otg_start_peripheral(struct usb_otg *otg, int on);
static void msm_chg_detect_work(struct work_struct *w);
static bool msm_chg_mhl_detect(struct msm_otg *motg);
static int msm_otg_start_srp(struct usb_otg *otg);
static void msm_otg_start_timer(struct msm_otg *motg, int time, int bit);
static void msm_otg_del_timer(struct msm_otg *motg);
static void msm_otg_start_host(struct usb_otg *otg, int on);
static int msm_otg_link_reset(struct msm_otg *motg);
static void msm_chg_enable_aca_intr(struct msm_otg *motg);
static void msm_chg_enable_aca_det(struct msm_otg *motg);
static void msm_chg_disable_aca_intr(struct msm_otg *motg);
static bool msm_chg_check_aca_intr(struct msm_otg *motg);
static void msm_otg_host_hnp_enable(struct usb_otg *otg, bool enable);

static void msm_otg_set_id_state_pbatest(int id, struct host_notify_dev *ndev)
{
	struct msm_otg *motg = container_of(ndev, struct msm_otg, ndev);

#ifdef MSM_OTG_DIRECT_VBUS
	pr_info("[OTG] %s %d, id: %d\n", __func__, __LINE__, id);
	msm_hsusb_vbus_power(motg, id);
#else
	struct usb_phy *phy = &motg->phy;

	pr_info("[OTG] %s %d, id: %d\n", __func__, __LINE__, id);
	if (atomic_read(&motg->in_lpm))
		pm_runtime_resume(phy->dev);

	if (!id)
		set_bit(ID, &motg->inputs);
	else
		clear_bit(ID, &motg->inputs);

	schedule_work(&motg->sm_work);
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485
	if (!id)
		fsa9485_otg_detach();
#endif
}

static void msm_otg_host_phy_tune(struct msm_otg *otg,
		u32 paramb, u32 paramc)
{
	ulpi_write(&otg->phy, paramb, 0x81);
	ulpi_write(&otg->phy, paramc, 0x82);

	pr_info("ULPI 0x%x: 0x%x: 0x%x: 0x%x\n",
			ulpi_read(&otg->phy, 0x80),
			ulpi_read(&otg->phy, 0x81),
			ulpi_read(&otg->phy, 0x82),
			ulpi_read(&otg->phy, 0x83));
	mdelay(100);
}

static void msm_otg_host_notify_set(struct msm_otg *motg, int state)
{
	pr_info("boost : %d\n", state);

	if (state &&
		(motg->ndev.mode == NOTIFY_HOST_MODE ||
		motg->ndev.mode == NOTIFY_TEST_MODE))
		motg->ndev.booster = NOTIFY_POWER_ON;
	else
		motg->ndev.booster = NOTIFY_POWER_OFF;
}

static void msm_otg_host_notify(struct msm_otg *motg, int on)
{
	pr_info("host_notify: %d, dock %d\n", on, motg->smartdock);

	if (on)
		msm_otg_host_phy_tune(motg, 0x33, 0x14);

	if (motg->smartdock)
		return;

	if (on) {
		motg->ndev.mode = NOTIFY_HOST_MODE;
		host_state_notify(&motg->ndev, NOTIFY_HOST_ADD);
	} else {
		motg->ndev.mode = NOTIFY_NONE_MODE;
		host_state_notify(&motg->ndev, NOTIFY_HOST_REMOVE);
	}
}

void msm_otg_set_dock_state(int enable)
{
	struct msm_otg *motg = the_msm_otg;
	struct usb_phy *phy = &motg->phy;

	if (enable) {
		pr_info("DOCK : attached\n");
		motg->smartdock = true;
		clear_bit(ID, &motg->inputs);

		if (atomic_read(&motg->in_lpm)) {
			pr_info("DOCK : in LPM\n");
			pm_runtime_resume(phy->dev);
		}

		if (test_bit(B_SESS_VLD, &motg->inputs)) {
			pr_info("clear B_SESS_VLD\n");
			clear_bit(B_SESS_VLD, &motg->inputs);
		}

		schedule_work(&motg->sm_work);

	} else {
		pr_info("DOCK : detached\n");
		motg->smartdock = false;
		set_bit(ID, &motg->inputs);
	}

}
EXPORT_SYMBOL_GPL(msm_otg_set_dock_state);

#ifdef MSM_OTG_LATE_INIT
static void msm_otg_sm_work_timer_func(unsigned long data)
{
	struct msm_otg *motg = (struct msm_otg *) data;
	struct usb_phy *phy = &motg->phy;

	if (atomic_read(&motg->in_lpm)) {
		dev_info(motg->phy.dev, "sm_work_timer: in lpm\n");
		return;
	}

	if (phy->state > OTG_STATE_B_IDLE) {
		dev_info(motg->phy.dev, "sm_work_timer: on working\n");
		return;
	}

	if (!schedule_work(&motg->sm_work)) {
		dev_info(motg->phy.dev, "sm_work_timer: pending\n");
		mod_timer(&motg->sm_work_timer, SM_WORK_TIMER_FREQ);
	}
}
#endif

static void msm_otg_power_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg, otg_power_work);
	int otg_test = gpio_get_value_cansleep(motg->pdata->otg_test_gpio);

	msleep(200);

	pr_info("id: %d, otg_test: %d\n",
			test_bit(ID, &motg->inputs), otg_test);

	/* host */
	if (!test_bit(ID, &motg->inputs)) {
		if (otg_test) {
			motg->ndev.booster = NOTIFY_POWER_ON;
		} else {
			motg->ndev.booster = NOTIFY_POWER_OFF;
			host_state_notify(&motg->ndev, NOTIFY_HOST_OVERCURRENT);
			pr_err("OTG Overcurrent!\n");
		}

	} else {
		if (motg->ndev.mode == NOTIFY_HOST_MODE && otg_test == 0)
			motg->ndev.booster = NOTIFY_POWER_OFF;
	}
}

static irqreturn_t msm_otg_test_irq(int irq, void *data)
{
	struct msm_otg *motg = data;

	schedule_work(&motg->otg_power_work);

	return IRQ_HANDLED;
}

static int msm_host_notify_init(struct device *dev, struct msm_otg *motg)
{
	int ret = 0;

	if (motg->pdata->otg_test_gpio) {
		int otg_test_irq = gpio_to_irq(motg->pdata->otg_test_gpio);

		ret = request_threaded_irq(otg_test_irq, NULL,
						msm_otg_test_irq,
						IRQF_TRIGGER_RISING |
						IRQF_TRIGGER_FALLING,
						"gpio_vbus", motg);
		if (ret)
			pr_err("request irq failed for otg_test\n");
		else
			pr_err("request irq succeed for otg_test\n");

		INIT_WORK(&motg->otg_power_work, msm_otg_power_work);
	}

	motg->ndev.name = "usb_otg";
	motg->ndev.set_booster = msm_otg_set_id_state_pbatest;
	ret = host_notify_dev_register(&motg->ndev);
	if (ret)
		dev_err(dev, "Failed to host_notify_dev_register\n");
	else
		dev_info(dev, "success to host_notify_dev_register\n");

#ifdef MSM_OTG_LATE_INIT
	motg->notify_state = ACC_POWER_OFF;
#endif
	return 0;
}

static int msm_host_notify_exit(struct msm_otg *motg)
{
	int ret = 0;

	host_notify_dev_unregister(&motg->ndev);

#ifdef MSM_OTG_LATE_INIT
	cancel_delayed_work_sync(&motg->late_power_work);
#endif

	return ret;
}

/*
 * New sm work
 */

#ifndef USE_ORIGINAL_SM_WORK
static int	sm_work_OTG_STATE_UNDEFINED(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	msm_otg_reset(otg->phy);
	msm_otg_init_sm(motg);
	psy = power_supply_get_by_name("usb");
	if (!psy)
		pr_err("couldn't get usb power supply\n");

	otg->phy->state = OTG_STATE_B_IDLE;

	if (!test_bit(B_SESS_VLD, &motg->inputs) &&
		test_bit(ID, &motg->inputs)) {

		pm_runtime_put_noidle(otg->phy->dev);
		pm_runtime_suspend(otg->phy->dev);

		return 1;
	}
	return 0;
}

static int sm_work_b_idle_chg_detected(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	switch (motg->chg_type) {
	case USB_DCP_CHARGER:
		/* Enable VDP_SRC */
		ulpi_write(otg->phy, 0x2, 0x85);
		/* fall through */
	case USB_PROPRIETARY_CHARGER:
		msm_otg_notify_charger(motg,
				IDEV_CHG_MAX);
		pm_runtime_put_noidle(otg->phy->dev);
		pm_runtime_suspend(otg->phy->dev);
		break;
	case USB_ACA_B_CHARGER:
		msm_otg_notify_charger(motg, IDEV_ACA_CHG_MAX);
		/*
		 * (ID_B --> ID_C) PHY_ALT interrupt can
		 * not be detected in LPM.
		 */
		break;
	case USB_CDP_CHARGER:
		msm_otg_notify_charger(motg,
				IDEV_CHG_MAX);
		msm_otg_start_peripheral(otg, 1);
		otg->phy->state =
			OTG_STATE_B_PERIPHERAL;
		break;
	case USB_ACA_C_CHARGER:
		msm_otg_notify_charger(motg,
				IDEV_ACA_CHG_MAX);
		msm_otg_start_peripheral(otg, 1);
		otg->phy->state =
			OTG_STATE_B_PERIPHERAL;
		break;
	case USB_SDP_CHARGER:
		msm_otg_start_peripheral(otg, 1);
		otg->phy->state =
			OTG_STATE_B_PERIPHERAL;
		break;
	default:
		break;
	}
	return 0;
}

static int sm_work_b_idle_session_valid(struct msm_otg *motg)
{
	pr_debug("b_sess_vld\n");

	switch (motg->chg_state) {
	case USB_CHG_STATE_UNDEFINED:
		msm_chg_detect_work(&motg->chg_work.work);
		break;
	case USB_CHG_STATE_DETECTED:
		sm_work_b_idle_chg_detected(motg);
		break;
	default:
		break;
	}

	return 0;
}

static int sm_work_OTG_STATE_B_IDLE(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	if (test_bit(MHL, &motg->inputs)) {
		/* allow LPM */
		pm_runtime_put_noidle(otg->phy->dev);
		pm_runtime_suspend(otg->phy->dev);

	} else if ((!test_bit(ID, &motg->inputs) ||
				test_bit(ID_A, &motg->inputs))
				&& otg->host) {

		pr_debug("!id || id_A\n");

		if (msm_chg_mhl_detect(motg))
			return 1;

		clear_bit(B_BUS_REQ, &motg->inputs);
		set_bit(A_BUS_REQ, &motg->inputs);
		otg->phy->state = OTG_STATE_A_IDLE;

		return 1;

	} else if (test_bit(B_SESS_VLD, &motg->inputs)) {
		pr_debug("b_sess_vld\n");
		sm_work_b_idle_session_valid(motg);

	} else if (test_bit(B_BUS_REQ, &motg->inputs)) {
		pr_debug("b_sess_end && b_bus_req\n");
		if (msm_otg_start_srp(otg) < 0) {
			clear_bit(B_BUS_REQ, &motg->inputs);
			return 1;
		}
		otg->phy->state = OTG_STATE_B_SRP_INIT;
		msm_otg_start_timer(motg, TB_SRP_FAIL, B_SRP_FAIL);

	} else {
		pr_debug("chg_work cancel");
		cancel_delayed_work_sync(&motg->chg_work);
		motg->chg_state = USB_CHG_STATE_UNDEFINED;
		motg->chg_type = USB_INVALID_CHARGER;
		msm_otg_notify_charger(motg, 0);
		msm_otg_reset(otg->phy);
		pm_runtime_put_noidle(otg->phy->dev);
		pm_runtime_suspend(otg->phy->dev);
	}

	return 0;
}

static int sm_work_OTG_STATE_B_SRP_INIT(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if (!test_bit(ID, &motg->inputs) ||
		test_bit(ID_A, &motg->inputs) ||
		test_bit(ID_C, &motg->inputs) ||
		(test_bit(B_SESS_VLD, &motg->inputs) &&
		 !test_bit(ID_B, &motg->inputs))) {

		pr_debug("!id || id_a/c || b_sess_vld+!id_b\n");
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_B_IDLE;
		/*
		 * clear VBUSVLDEXTSEL and VBUSVLDEXT register
		 * bits after SRP initiation.
		 */
		ulpi_write(otg->phy, 0x0, 0x98);
		work = 1;

	} else if (test_bit(B_SRP_FAIL, &motg->tmouts)) {
		pr_debug("b_srp_fail\n");
		pr_info("A-device did not respond to SRP\n");
		clear_bit(B_BUS_REQ, &motg->inputs);
		clear_bit(B_SRP_FAIL, &motg->tmouts);
		otg_send_event(OTG_EVENT_NO_RESP_FOR_SRP);
		ulpi_write(otg->phy, 0x0, 0x98);
		otg->phy->state = OTG_STATE_B_IDLE;
		motg->b_last_se0_sess = jiffies;
		work = 1;
	}
	return work;
}

static int sm_work_OTG_STATE_B_PERIPHERAL(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int srp_reqd;
	int work = 0;

	if (!test_bit(ID, &motg->inputs) ||
		test_bit(ID_A, &motg->inputs) ||
		test_bit(ID_B, &motg->inputs) ||
		!test_bit(B_SESS_VLD, &motg->inputs)) {

		pr_debug("!id  || id_a/b || !b_sess_vld\n");

		motg->chg_state = USB_CHG_STATE_UNDEFINED;
		motg->chg_type = USB_INVALID_CHARGER;
		msm_otg_notify_charger(motg, 0);
		srp_reqd = otg->gadget->otg_srp_reqd;
		msm_otg_start_peripheral(otg, 0);

		if (test_bit(ID_B, &motg->inputs))
			clear_bit(ID_B, &motg->inputs);

		clear_bit(B_BUS_REQ, &motg->inputs);
		otg->phy->state = OTG_STATE_B_IDLE;
		motg->b_last_se0_sess = jiffies;

		if (srp_reqd)
			msm_otg_start_timer(motg, TB_TST_SRP, B_TST_SRP);
		else
			work = 1;

	} else if (test_bit(B_BUS_REQ, &motg->inputs) &&
			otg->gadget->b_hnp_enable &&
			test_bit(A_BUS_SUSPEND, &motg->inputs)) {

		pr_debug("b_bus_req && b_hnp_en && a_bus_suspend\n");

		msm_otg_start_timer(motg, TB_ASE0_BRST, B_ASE0_BRST);
		/* D+ pullup should not be disconnected within 4msec
		 * after A device suspends the bus. Otherwise PET will
		 * fail the compliance test.
		 */
		udelay(1000);
		msm_otg_start_peripheral(otg, 0);
		otg->phy->state = OTG_STATE_B_WAIT_ACON;
		/*
		 * start HCD even before A-device enable
		 * pull-up to meet HNP timings.
		 */
		otg->host->is_b_host = 1;
		msm_otg_start_host(otg, 1);

	} else if (test_bit(A_BUS_SUSPEND, &motg->inputs) &&
			test_bit(B_SESS_VLD, &motg->inputs)) {

		pr_debug("a_bus_suspend && b_sess_vld\n");
		if (motg->caps & ALLOW_LPM_ON_DEV_SUSPEND) {
			pm_runtime_put_noidle(otg->phy->dev);
			pm_runtime_suspend(otg->phy->dev);
		}

	} else if (test_bit(ID_C, &motg->inputs)) {
		msm_otg_notify_charger(motg, IDEV_ACA_CHG_MAX);
	}

	return work;
}

static int sm_work_OTG_STATE_B_WAIT_ACON(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if (!test_bit(ID, &motg->inputs) ||
		test_bit(ID_A, &motg->inputs) ||
		test_bit(ID_B, &motg->inputs) ||
		!test_bit(B_SESS_VLD, &motg->inputs)) {

		pr_debug("!id || id_a/b || !b_sess_vld\n");
		msm_otg_del_timer(motg);
		/*
		 * A-device is physically disconnected during
		 * HNP. Remove HCD.
		 */
		msm_otg_start_host(otg, 0);
		otg->host->is_b_host = 0;

		clear_bit(B_BUS_REQ, &motg->inputs);
		clear_bit(A_BUS_SUSPEND, &motg->inputs);
		motg->b_last_se0_sess = jiffies;
		otg->phy->state = OTG_STATE_B_IDLE;
		msm_otg_reset(otg->phy);
		work = 1;

	} else if (test_bit(A_CONN, &motg->inputs)) {
		pr_debug("a_conn\n");
		clear_bit(A_BUS_SUSPEND, &motg->inputs);
		otg->phy->state = OTG_STATE_B_HOST;
		/*
		 * PET disconnects D+ pullup after reset is generated
		 * by B device in B_HOST role which is not detected by
		 * B device. As workaorund , start timer of 300msec
		 * and stop timer if A device is enumerated else clear
		 * A_CONN.
		 */
		msm_otg_start_timer(motg, TB_TST_CONFIG,
				B_TST_CONFIG);
	} else if (test_bit(B_ASE0_BRST, &motg->tmouts)) {
		pr_debug("b_ase0_brst_tmout\n");
		pr_info("B HNP fail:No response from A device\n");
		msm_otg_start_host(otg, 0);
		msm_otg_reset(otg->phy);
		otg->host->is_b_host = 0;
		clear_bit(B_ASE0_BRST, &motg->tmouts);
		clear_bit(A_BUS_SUSPEND, &motg->inputs);
		clear_bit(B_BUS_REQ, &motg->inputs);
		otg_send_event(OTG_EVENT_HNP_FAILED);
		otg->phy->state = OTG_STATE_B_IDLE;
		work = 1;

	} else if (test_bit(ID_C, &motg->inputs)) {
		msm_otg_notify_charger(motg, IDEV_ACA_CHG_MAX);
	}

	return work;
}

static int sm_work_OTG_STATE_B_HOST(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if (!test_bit(B_BUS_REQ, &motg->inputs) ||
		!test_bit(A_CONN, &motg->inputs) ||
		!test_bit(B_SESS_VLD, &motg->inputs)) {

		pr_debug("!b_bus_req || !a_conn || !b_sess_vld\n");
		clear_bit(A_CONN, &motg->inputs);
		clear_bit(B_BUS_REQ, &motg->inputs);
		msm_otg_start_host(otg, 0);
		otg->host->is_b_host = 0;
		otg->phy->state = OTG_STATE_B_IDLE;
		msm_otg_reset(otg->phy);
		work = 1;
	} else if (test_bit(ID_C, &motg->inputs)) {
		msm_otg_notify_charger(motg, IDEV_ACA_CHG_MAX);
	}
	return work;
}

static int sm_work_OTG_STATE_A_IDLE(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	otg->default_a = 1;

	if (test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) {

		pr_debug("id && !id_a\n");
		otg->default_a = 0;
		clear_bit(A_BUS_DROP, &motg->inputs);
		otg->phy->state = OTG_STATE_B_IDLE;
		del_timer_sync(&motg->id_timer);
		msm_otg_link_reset(motg);
		msm_chg_enable_aca_intr(motg);
		msm_otg_notify_charger(motg, 0);
		work = 1;

	} else if (!test_bit(A_BUS_DROP, &motg->inputs) &&
			(test_bit(A_SRP_DET, &motg->inputs) ||
			 test_bit(A_BUS_REQ, &motg->inputs))) {

		pr_debug("!a_bus_drop && (a_srp_det || a_bus_req)\n");

		clear_bit(A_SRP_DET, &motg->inputs);
		/* Disable SRP detection */
		writel_relaxed((readl_relaxed(USB_OTGSC) &
					~OTGSC_INTSTS_MASK) &
				~OTGSC_DPIE, USB_OTGSC);

		otg->phy->state = OTG_STATE_A_WAIT_VRISE;
		/* VBUS should not be supplied before end of SRP pulse
		 * generated by PET, if not complaince test fail.
		 */
		usleep_range(10000, 12000);
		/* ACA: ID_A: Stop charging untill enumeration */
		if (test_bit(ID_A, &motg->inputs))
			msm_otg_notify_charger(motg, 0);
		else
			msm_hsusb_vbus_power(motg, 1);
		msm_otg_start_timer(motg, TA_WAIT_VRISE, A_WAIT_VRISE);

	} else {
		pr_debug("No session requested\n");
		clear_bit(A_BUS_DROP, &motg->inputs);
		if (test_bit(ID_A, &motg->inputs)) {
			msm_otg_notify_charger(motg,
					IDEV_ACA_CHG_MAX);
		} else if (!test_bit(ID, &motg->inputs)) {
			msm_otg_notify_charger(motg, 0);
			/*
			 * A-device is not providing power on VBUS.
			 * Enable SRP detection.
			 */
			writel_relaxed(0x13, USB_USBMODE);
			writel_relaxed((readl_relaxed(USB_OTGSC) &
						~OTGSC_INTSTS_MASK) |
					OTGSC_DPIE, USB_OTGSC);
			mb();
		}
	}
	return work;
}

static int sm_work_OTG_STATE_A_WAIT_VRISE(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs) ||
		test_bit(A_WAIT_VRISE, &motg->tmouts)) {

		pr_debug("id || a_bus_drop || a_wait_vrise_tmout\n");
		clear_bit(A_BUS_REQ, &motg->inputs);
		msm_otg_del_timer(motg);
		msm_hsusb_vbus_power(motg, 0);
		otg->phy->state = OTG_STATE_A_WAIT_VFALL;
		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);

	} else if (test_bit(A_VBUS_VLD, &motg->inputs)) {
		pr_debug("a_vbus_vld\n");
		otg->phy->state = OTG_STATE_A_WAIT_BCON;

		if (TA_WAIT_BCON > 0)
			msm_otg_start_timer(motg, TA_WAIT_BCON, A_WAIT_BCON);

		msm_otg_start_host(otg, 1);
		msm_chg_enable_aca_det(motg);
		msm_chg_disable_aca_intr(motg);
		mod_timer(&motg->id_timer, ID_TIMER_FREQ);

		if (msm_chg_check_aca_intr(motg))
			work = 1;
	}
	return work;
}


static int sm_work_OTG_STATE_A_WAIT_BCON(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs) ||
		test_bit(A_WAIT_BCON, &motg->tmouts)) {

		pr_debug("(id && id_a/b/c) || a_bus_drop ||"
				"a_wait_bcon_tmout\n");

		if (test_bit(A_WAIT_BCON, &motg->tmouts)) {
			pr_info("Device No Response\n");
			otg_send_event(OTG_EVENT_DEV_CONN_TMOUT);
		}

		msm_otg_del_timer(motg);
		clear_bit(A_BUS_REQ, &motg->inputs);
		clear_bit(B_CONN, &motg->inputs);
		msm_otg_start_host(otg, 0);
		/*
		 * ACA: ID_A with NO accessory, just the A plug is
		 * attached to ACA: Use IDCHG_MAX for charging
		 */
		if (test_bit(ID_A, &motg->inputs))
			msm_otg_notify_charger(motg, IDEV_CHG_MIN);
		else
			msm_hsusb_vbus_power(motg, 0);
		otg->phy->state = OTG_STATE_A_WAIT_VFALL;
		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);

	} else if (!test_bit(A_VBUS_VLD, &motg->inputs)) {
		pr_debug("!a_vbus_vld\n");
		clear_bit(B_CONN, &motg->inputs);
		msm_otg_del_timer(motg);
		msm_otg_start_host(otg, 0);
		otg->phy->state = OTG_STATE_A_VBUS_ERR;
		msm_otg_reset(otg->phy);

	} else if (test_bit(ID_A, &motg->inputs)) {
		msm_hsusb_vbus_power(motg, 0);
	} else if (!test_bit(A_BUS_REQ, &motg->inputs)) {
		/*
		 * If TA_WAIT_BCON is infinite, we don;t
		 * turn off VBUS. Enter low power mode.
		 */
		if (TA_WAIT_BCON < 0)
			pm_runtime_put_sync(otg->phy->dev);
	} else if (!test_bit(ID, &motg->inputs)) {
		msm_hsusb_vbus_power(motg, 1);
	}
	return 0;
}

static int sm_work_OTG_STATE_A_HOST(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs)) {

		pr_debug("id_a/b/c || a_bus_drop\n");
		clear_bit(B_CONN, &motg->inputs);
		clear_bit(A_BUS_REQ, &motg->inputs);
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_WAIT_VFALL;
		msm_otg_start_host(otg, 0);

		if (!test_bit(ID_A, &motg->inputs))
			msm_hsusb_vbus_power(motg, 0);

		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);

	} else if (!test_bit(A_VBUS_VLD, &motg->inputs)) {
		pr_debug("!a_vbus_vld\n");
		clear_bit(B_CONN, &motg->inputs);
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_VBUS_ERR;
		msm_otg_start_host(otg, 0);
		msm_otg_reset(otg->phy);

	} else if (!test_bit(A_BUS_REQ, &motg->inputs)) {
		/*
		 * a_bus_req is de-asserted when root hub is
		 * suspended or HNP is in progress.
		 */
		pr_debug("!a_bus_req\n");
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_SUSPEND;

		if (otg->host->b_hnp_enable)
			msm_otg_start_timer(motg, TA_AIDL_BDIS, A_AIDL_BDIS);
		else
			pm_runtime_put_sync(otg->phy->dev);

	} else if (!test_bit(B_CONN, &motg->inputs)) {
		pr_debug("!b_conn\n");
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_WAIT_BCON;
		if (TA_WAIT_BCON > 0)
			msm_otg_start_timer(motg, TA_WAIT_BCON,
					A_WAIT_BCON);
		if (msm_chg_check_aca_intr(motg))
			work = 1;

	} else if (test_bit(ID_A, &motg->inputs)) {
		msm_otg_del_timer(motg);
		msm_hsusb_vbus_power(motg, 0);
		if (motg->chg_type == USB_ACA_DOCK_CHARGER)
			msm_otg_notify_charger(motg,
					IDEV_ACA_CHG_MAX);
		else
			msm_otg_notify_charger(motg,
					IDEV_CHG_MIN - motg->mA_port);

	} else if (!test_bit(ID, &motg->inputs)) {
		motg->chg_state = USB_CHG_STATE_UNDEFINED;
		motg->chg_type = USB_INVALID_CHARGER;
		msm_otg_notify_charger(motg, 0);
		msm_hsusb_vbus_power(motg, 1);
	}
	return work;
}

static int sm_work_OTG_STATE_A_SUSPEND(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs) ||
		test_bit(A_AIDL_BDIS, &motg->tmouts)) {

		pr_debug("id_a/b/c || a_bus_drop ||"
				"a_aidl_bdis_tmout\n");
		msm_otg_del_timer(motg);
		clear_bit(B_CONN, &motg->inputs);
		otg->phy->state = OTG_STATE_A_WAIT_VFALL;
		msm_otg_start_host(otg, 0);
		msm_otg_reset(otg->phy);

		if (!test_bit(ID_A, &motg->inputs))
			msm_hsusb_vbus_power(motg, 0);
		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);

	} else if (!test_bit(A_VBUS_VLD, &motg->inputs)) {
		pr_debug("!a_vbus_vld\n");
		msm_otg_del_timer(motg);
		clear_bit(B_CONN, &motg->inputs);
		otg->phy->state = OTG_STATE_A_VBUS_ERR;
		msm_otg_start_host(otg, 0);
		msm_otg_reset(otg->phy);

	} else if (!test_bit(B_CONN, &motg->inputs) &&
			otg->host->b_hnp_enable) {
		pr_debug("!b_conn && b_hnp_enable");
		otg->phy->state = OTG_STATE_A_PERIPHERAL;
		msm_otg_host_hnp_enable(otg, 1);
		otg->gadget->is_a_peripheral = 1;
		msm_otg_start_peripheral(otg, 1);

	} else if (!test_bit(B_CONN, &motg->inputs) &&
			!otg->host->b_hnp_enable) {

		pr_debug("!b_conn && !b_hnp_enable");
		/*
		 * bus request is dropped during suspend.
		 * acquire again for next device.
		 */
		set_bit(A_BUS_REQ, &motg->inputs);
		otg->phy->state = OTG_STATE_A_WAIT_BCON;

		if (TA_WAIT_BCON > 0)
			msm_otg_start_timer(motg, TA_WAIT_BCON,
					A_WAIT_BCON);

	} else if (test_bit(ID_A, &motg->inputs)) {
		msm_hsusb_vbus_power(motg, 0);
		msm_otg_notify_charger(motg,
				IDEV_CHG_MIN - motg->mA_port);

	} else if (!test_bit(ID, &motg->inputs)) {
		msm_otg_notify_charger(motg, 0);
		msm_hsusb_vbus_power(motg, 1);
	}
	return 0;
}

static int sm_work_OTG_STATE_A_PERIPHERAL(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs)) {

		pr_debug("id _f/b/c || a_bus_drop\n");
		/* Clear BIDL_ADIS timer */
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_WAIT_VFALL;
		msm_otg_start_peripheral(otg, 0);
		otg->gadget->is_a_peripheral = 0;
		msm_otg_start_host(otg, 0);
		msm_otg_reset(otg->phy);

		if (!test_bit(ID_A, &motg->inputs))
			msm_hsusb_vbus_power(motg, 0);
		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);

	} else if (!test_bit(A_VBUS_VLD, &motg->inputs)) {
		pr_debug("!a_vbus_vld\n");
		/* Clear BIDL_ADIS timer */
		msm_otg_del_timer(motg);
		otg->phy->state = OTG_STATE_A_VBUS_ERR;
		msm_otg_start_peripheral(otg, 0);
		otg->gadget->is_a_peripheral = 0;
		msm_otg_start_host(otg, 0);

	} else if (test_bit(A_BIDL_ADIS, &motg->tmouts)) {
		pr_debug("a_bidl_adis_tmout\n");
		msm_otg_start_peripheral(otg, 0);
		otg->gadget->is_a_peripheral = 0;
		otg->phy->state = OTG_STATE_A_WAIT_BCON;
		set_bit(A_BUS_REQ, &motg->inputs);
		msm_otg_host_hnp_enable(otg, 0);

		if (TA_WAIT_BCON > 0)
			msm_otg_start_timer(motg, TA_WAIT_BCON,
					A_WAIT_BCON);

	} else if (test_bit(ID_A, &motg->inputs)) {
		msm_hsusb_vbus_power(motg, 0);
		msm_otg_notify_charger(motg,
				IDEV_CHG_MIN - motg->mA_port);

	} else if (!test_bit(ID, &motg->inputs)) {
		msm_otg_notify_charger(motg, 0);
		msm_hsusb_vbus_power(motg, 1);
	}
	return 0;
}

static int sm_work_OTG_STATE_A_WAIT_VFALL(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	if (test_bit(A_WAIT_VFALL, &motg->tmouts)) {
		clear_bit(A_VBUS_VLD, &motg->inputs);
		otg->phy->state = OTG_STATE_A_IDLE;
		work = 1;
	}
	return work;
}

static int sm_work_OTG_STATE_A_VBUS_ERR(struct msm_otg *motg)
{
	struct usb_otg *otg = motg->phy.otg;

	if ((test_bit(ID, &motg->inputs) &&
		!test_bit(ID_A, &motg->inputs)) ||
		test_bit(A_BUS_DROP, &motg->inputs) ||
		test_bit(A_CLR_ERR, &motg->inputs)) {

		otg->phy->state = OTG_STATE_A_WAIT_VFALL;

		if (!test_bit(ID_A, &motg->inputs))
			msm_hsusb_vbus_power(motg, 0);

		msm_otg_start_timer(motg, TA_WAIT_VFALL, A_WAIT_VFALL);
		motg->chg_state = USB_CHG_STATE_UNDEFINED;
		motg->chg_type = USB_INVALID_CHARGER;
		msm_otg_notify_charger(motg, 0);
	}
	return 0;
}

static void msm_otg_sm_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg, sm_work);
	struct usb_otg *otg = motg->phy.otg;
	int work = 0;

	pm_runtime_resume(otg->phy->dev);
	pr_debug("%s work\n", otg_state_string(otg->phy->state));

	switch (otg->phy->state) {
	case OTG_STATE_UNDEFINED:
		if (sm_work_OTG_STATE_UNDEFINED(motg))
			break;
		/* FALL THROUGH */

	case OTG_STATE_B_IDLE:
		work = sm_work_OTG_STATE_B_IDLE(motg);
		break;

	case OTG_STATE_B_SRP_INIT:
		work = sm_work_OTG_STATE_B_SRP_INIT(motg);
		break;

	case OTG_STATE_B_PERIPHERAL:
		work = sm_work_OTG_STATE_B_PERIPHERAL(motg);
		break;

	case OTG_STATE_B_WAIT_ACON:
		work = sm_work_OTG_STATE_B_WAIT_ACON(motg);
		break;

	case OTG_STATE_B_HOST:
		work = sm_work_OTG_STATE_B_HOST(motg);
		break;

	case OTG_STATE_A_IDLE:
		work = sm_work_OTG_STATE_A_IDLE(motg);
		break;

	case OTG_STATE_A_WAIT_VRISE:
		work = sm_work_OTG_STATE_A_WAIT_VRISE(motg);
		break;

	case OTG_STATE_A_WAIT_BCON:
		work = sm_work_OTG_STATE_A_WAIT_BCON(motg);
		break;

	case OTG_STATE_A_HOST:
		work = sm_work_OTG_STATE_A_HOST(motg);
		break;

	case OTG_STATE_A_SUSPEND:
		sm_work_OTG_STATE_A_SUSPEND(motg);
		break;

	case OTG_STATE_A_PERIPHERAL:
		sm_work_OTG_STATE_A_PERIPHERAL(motg);
		break;

	case OTG_STATE_A_WAIT_VFALL:
		work = sm_work_OTG_STATE_A_WAIT_VFALL(motg);
		break;

	case OTG_STATE_A_VBUS_ERR:
		sm_work_OTG_STATE_A_VBUS_ERR(motg);
		break;

	default:
		break;
	}

	if (work)
		queue_work(system_nrt_wq, &motg->sm_work);
}
#endif /* ifndef USE_ORIGINAL_SM_WORK */
