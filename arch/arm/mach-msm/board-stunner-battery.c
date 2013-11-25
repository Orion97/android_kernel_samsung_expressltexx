/*
 * Copyright (C) 2012 Samsung Electronics, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/gpio.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/mfd/pm8xxx/pm8xxx-adc.h>
#include <linux/mfd/pm8xxx/pm8921-sec-charger.h>

#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/msm8930-gpio.h>

#include "devices-msm8x60.h"
#include "board-8930.h"

#if defined(CONFIG_BATTERY_SAMSUNG)
#include <linux/battery/sec_battery.h>
#include <linux/battery/sec_fuelgauge.h>
#include <linux/battery/sec_charger.h>

#define SEC_BATTERY_PMIC_NAME ""

static unsigned int sec_bat_recovery_mode;

static bool sec_bat_adc_none_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_none_exit(void) {return true; }
static int sec_bat_adc_none_read(unsigned int channel) {return 0; }

static bool sec_bat_adc_ap_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_ap_exit(void) {return true; }

/*
Function : sec_bat_adc_ap_read
Description :
*/
static int sec_bat_adc_ap_read(unsigned int channel)
{
	int rc = 0;
	int data = 0;
	struct pm8xxx_adc_chan_result result;

	switch (channel) {
	case SEC_BAT_ADC_CHANNEL_TEMP:
		rc = pm8xxx_adc_mpp_config_read(
			PM8XXX_AMUX_MPP_7, ADC_MPP_1_AMUX6, &result);
		if (rc) {
			pr_err("error reading mpp %d, rc = %d\n",
				PM8XXX_AMUX_MPP_7, rc);
			return rc;
		}

		/* use measurement, no need to scale */
		data = (int)result.measurement;
		break;
	case SEC_BAT_ADC_CHANNEL_TEMP_AMBIENT:
		/* jmru.lee , temporarily blocked */
/*
		rc = pm8xxx_adc_mpp_config_read(
			PM8XXX_AMUX_MPP_6, ADC_MPP_1_AMUX6, &result);
		if (rc) {
			pr_err("error reading mpp %d, rc = %d\n",
				PM8XXX_AMUX_MPP_6, rc);
			return rc;
		}

		data = (int)result.measurement;
*/
		break;
	}

	return data;
}

static bool sec_bat_adc_ic_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_ic_exit(void) {return true; }
static int sec_bat_adc_ic_read(unsigned int channel) {return 0; }

/*
Function : sec_bat_gpio_init
Description :
*/
static bool sec_bat_gpio_init(void)
{
	/* [120821] jmru.lee - for OTG  */
	msm_otg_pmic_gpio_config(PMIC_GPIO_OTG_EN,
		PM_GPIO_DIR_IN, PM_GPIO_PULL_NO, 0);

	return true;
}

/*
Function : sec_fg_gpio_init
Description :
*/
static bool sec_fg_gpio_init(void)
{
	return true;
}

/*
Function : sec_chg_gpio_init
Description :
*/
static bool sec_chg_gpio_init(void)
{
	return true;
}


#ifdef CONFIG_SAMSUNG_LPM_MODE
static bool sec_bat_is_lpm(void) {return (bool)poweroff_charging; }
#endif

/*
Function : sec_bat_initial_check
Description :
*/
static void sec_bat_initial_check(void)
{
	union power_supply_propval value;

	switch (msm8930_get_cable_status()) {
	case CABLE_TYPE_AC:
		value.intval = POWER_SUPPLY_TYPE_MAINS;
		break;
	case CABLE_TYPE_MISC:
		value.intval = POWER_SUPPLY_TYPE_MISC;
		break;
	case CABLE_TYPE_USB:
		value.intval = POWER_SUPPLY_TYPE_USB;
		break;
	case CABLE_TYPE_CARDOCK:
		value.intval = POWER_SUPPLY_TYPE_CARDOCK;
		break;
	case CABLE_TYPE_NONE:
		value.intval = POWER_SUPPLY_TYPE_BATTERY;
		break;
	default:
		pr_err("%s: invalid cable :%d\n",
			__func__, msm8930_get_cable_status());
		return;
	}

	psy_do_property("battery", set,
		POWER_SUPPLY_PROP_ONLINE, value);
}

static bool sec_bat_check_jig_status(void) {return false; }
static void sec_bat_switch_to_check(void) {}
static void sec_bat_switch_to_normal(void) {}

/*
Function : sec_bat_check_cable_callback
Description :
*/
static int current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
static int sec_bat_check_cable_callback(void)
{
	return current_cable_type;
}

/*
Function : sec_bat_check_cable_result_callback
Description :
*/
static bool sec_bat_check_cable_result_callback(
				int cable_type)
{
	current_cable_type = cable_type;

	switch (cable_type) {
	case POWER_SUPPLY_TYPE_USB:
		pr_info("%s set vbus applied\n",
			__func__);
		break;

	case POWER_SUPPLY_TYPE_BATTERY:
		pr_info("%s set vbus cut\n",
			__func__);
		break;
	case POWER_SUPPLY_TYPE_MAINS:
		break;
	default:
		pr_err("%s cable type (%d)\n",
			__func__, cable_type);
		return false;
	}
	return true;
}
/* callback for battery check
 * return : bool
 * true - battery detected, false battery NOT detected
 */
static bool sec_bat_check_callback(void) {return true; }
static bool sec_bat_check_result_callback(void) {return true; }

/* callback for OVP/UVLO check
 * return : int
 * battery health
 */
static int sec_bat_ovp_uvlo_callback(void)
{
	int health;
	health = POWER_SUPPLY_HEALTH_GOOD;

	return health;
}

static bool sec_bat_ovp_uvlo_result_callback(int health) {return true; }

/*
 * val.intval : temperature
 */
static bool sec_bat_get_temperature_callback(
		enum power_supply_property psp,
		union power_supply_propval *val) {return true; }
static bool sec_fg_fuelalert_process(bool is_fuel_alerted) {return true; }

static const sec_bat_adc_table_data_t temp_table[] = {
	{27592,	650},
	{27999,	600},
	{28563,	550},
	{29211,	500},
	{29948,	450},
	{30769,	400},
	{36990,	100},
	{37991,	50},
	{38910,	0},
	{39658,	-50},
	{40443,	-100},
	{41034,	-150},
	{41523,	-200},
	{41825,	-250},
	{42158,	-300},
};

/* ADC region should be exclusive */
static sec_bat_adc_region_t cable_adc_value_table[] = {
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
};

static sec_charging_current_t charging_current_table[] = {
	{0,	0,	0,	0},
	{0,	0,	0,	0},
	{1000,	1050,	150,	0},
	{1500,	1500,	150,	0},	/* AC */
	{1000,	500,	150,	0},		/* USB */
	{1000,	500,	150,	0},
	{1000,	500,	150,	0},
	{1000,	700,	150,	0},
	{0,	0,	0,	0},
	{1000,	1050,	150,	0},
	{0,	0,	0,	0},
	{0,	0,	0,	0},
};

static int polling_time_table[] = {
	10,	/* BASIC */
	30,	/* CHARGING */
	30,	/* DISCHARGING */
	30,	/* NOT_CHARGING */
	300,	/* SLEEP */
};

/* for MAX17048 */
static struct battery_data_t apexq_battery_data[] = {
	/* SDI battery data (High voltage 4.35V) */
	{
		.RCOMP0 = 0x57,
		.RCOMP_charging = 0x67,
		.temp_cohot = -100,
		.temp_cocold = -4500,
		.is_using_model_data = true,
		.type_str = "SDI",
	}
};

static sec_battery_platform_data_t sec_battery_pdata = {
	/* NO NEED TO BE CHANGED */
	.initial_check = sec_bat_initial_check,
	.bat_gpio_init = sec_bat_gpio_init,
	.fg_gpio_init = sec_fg_gpio_init,
	.chg_gpio_init = sec_chg_gpio_init,
#ifdef CONFIG_SAMSUNG_LPM_MODE
	.is_lpm = sec_bat_is_lpm,
#endif
	.check_jig_status = sec_bat_check_jig_status,
	.check_cable_callback =
		sec_bat_check_cable_callback,
	.cable_switch_check = sec_bat_switch_to_check,
	.cable_switch_normal = sec_bat_switch_to_normal,
	.check_cable_result_callback =
		sec_bat_check_cable_result_callback,
	.check_battery_callback =
		sec_bat_check_callback,
	.check_battery_result_callback =
		sec_bat_check_result_callback,
	.ovp_uvlo_callback = sec_bat_ovp_uvlo_callback,
	.ovp_uvlo_result_callback =
		sec_bat_ovp_uvlo_result_callback,
	.fuelalert_process = sec_fg_fuelalert_process,
	.get_temperature_callback =
		sec_bat_get_temperature_callback,

	.adc_api[SEC_BATTERY_ADC_TYPE_NONE] = {
		.init = sec_bat_adc_none_init,
		.exit = sec_bat_adc_none_exit,
		.read = sec_bat_adc_none_read
		},
	.adc_api[SEC_BATTERY_ADC_TYPE_AP] = {
		.init = sec_bat_adc_ap_init,
		.exit = sec_bat_adc_ap_exit,
		.read = sec_bat_adc_ap_read
		},
	.adc_api[SEC_BATTERY_ADC_TYPE_IC] = {
		.init = sec_bat_adc_ic_init,
		.exit = sec_bat_adc_ic_exit,
		.read = sec_bat_adc_ic_read
		},
	.cable_adc_value = cable_adc_value_table,
	.charging_current = charging_current_table,
	.polling_time = polling_time_table,
	/* NO NEED TO BE CHANGED */

	.pmic_name = SEC_BATTERY_PMIC_NAME,

	.adc_check_count = 7,
	.adc_type = {
		SEC_BATTERY_ADC_TYPE_NONE,	/* CABLE_CHECK */
		SEC_BATTERY_ADC_TYPE_NONE,	/* BAT_CHECK */
		SEC_BATTERY_ADC_TYPE_AP,	/* TEMP */
		SEC_BATTERY_ADC_TYPE_AP,	/* TEMP_AMB */
		SEC_BATTERY_ADC_TYPE_AP,	/* FULL_CHECK */
	},

	/* Battery */
	.vendor = "SDI SDI",
	.technology = POWER_SUPPLY_TECHNOLOGY_LION,
	.battery_data = (void *)0,
	.bat_gpio_ta_nconnected = 0,
	.bat_polarity_ta_nconnected = 0,
	.bat_irq = 0,
	.bat_irq_attr = 0,
	.cable_check_type =
		SEC_BATTERY_CABLE_CHECK_PSY,
	.cable_source_type =
		SEC_BATTERY_CABLE_SOURCE_EXTERNAL,

	.event_check = true,
	.event_waiting_time = 600,

	/* Monitor setting */
	.polling_type = SEC_BATTERY_MONITOR_ALARM,
	.monitor_initial_count = 3,

	/* Battery check */
	.battery_check_type = SEC_BATTERY_CHECK_CALLBACK,
	.check_count = 0,
	/* Battery check by ADC */
	.check_adc_max = 0,
	.check_adc_min = 0,

	/* OVP/UVLO check */
	.ovp_uvlo_check_type = SEC_BATTERY_OVP_UVLO_CHGPOLLING,

	/* Temperature check */
	.thermal_source = SEC_BATTERY_THERMAL_SOURCE_ADC,
	.temp_adc_table = temp_table,
	.temp_adc_table_size =
		sizeof(temp_table)/sizeof(sec_bat_adc_table_data_t),
	.temp_amb_adc_table = temp_table,
	.temp_amb_adc_table_size =
		sizeof(temp_table)/sizeof(sec_bat_adc_table_data_t),

	.temp_check_type = SEC_BATTERY_TEMP_CHECK_TEMP,
	.temp_check_count = 2,
	.temp_high_threshold_event = 630,
	.temp_high_recovery_event = 450,
	.temp_low_threshold_event = -30,
	.temp_low_recovery_event = 0,
	.temp_high_threshold_normal = 470,
	.temp_high_recovery_normal = 430,
	.temp_low_threshold_normal = -30,
	.temp_low_recovery_normal = 0,
	.temp_high_threshold_lpm = 470,
	.temp_high_recovery_lpm = 440,
	.temp_low_threshold_lpm = -40,
	.temp_low_recovery_lpm = -10,

	.full_check_type = SEC_BATTERY_FULLCHARGED_CHGPSY,
	.full_check_count = 3,
	.full_check_adc_1st = 20000,
	.full_check_adc_2nd = 20000,
	.chg_gpio_full_check = 0,
	.chg_polarity_full_check = 1,
	.full_condition_type = 0,

	.recharge_condition_type = SEC_BATTERY_RECHARGE_CONDITION_VCELL,
	.recharge_condition_vcell = 4170,

	.charging_total_time = 6 * 60 * 60,
	.recharging_total_time = 90 * 60,
	.charging_reset_time = 10 * 60,

	/* Fuel Gauge */
	.fg_irq = 0,
	.fg_irq_attr = 0,
	.fuel_alert_soc = 0,
	.repeated_fuelalert = false,
	.capacity_calculation_type = 0,
	.capacity_max = 1000,
	.capacity_min = 0,

	/* Charger */
	.chg_gpio_en = 0,
	.chg_polarity_en = 0,
	.chg_gpio_status = 0,
	.chg_polarity_status = 0,
	.chg_irq = 0,
	.chg_irq_attr = 0,
	.chg_float_voltage = 4350,
};

static struct platform_device sec_device_battery = {
	.name = "sec-battery",
	.id = -1,
	.dev.platform_data = &sec_battery_pdata,
};

static struct platform_device *msm8960_battery_devices[] __initdata = {
	&sec_device_battery,
};

static int __init sec_bat_current_boot_mode(char *mode)
{
	/*
	*	1 is recovery booting
	*	0 is normal booting
	*/

	if (strncmp(mode, "1", 1) == 0)
		sec_bat_recovery_mode = 1;
	else
		sec_bat_recovery_mode = 0;

	pr_info("%s : %s", __func__, sec_bat_recovery_mode == 1 ?
				"recovery" : "normal");

	return 1;
}
__setup("androidboot.boot_recovery=", sec_bat_current_boot_mode);

void __init msm8930_init_battery(void)
{
	/* board dependent changes in booting */

	platform_add_devices(
		msm8960_battery_devices,
		ARRAY_SIZE(msm8960_battery_devices));
}

#endif

