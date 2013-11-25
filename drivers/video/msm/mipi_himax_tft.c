/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/lcd.h>
#include <linux/wakelock.h>
#include <linux/gpio.h>
#include <mach/msm8930-gpio.h>
#include "mipi_himax_tft.h"
#include "mdp4.h"
#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT)
#include "mdp4_video_enhance.h"
#endif
#define USE_READ_ID
static struct mipi_samsung_driver_data msd;
static unsigned int recovery_boot_mode;

#define WA_FOR_FACTORY_MODE
#define READ_MTP_ONCE

int is_lcd_connected = 1;

static char WRDISBV[] = {
	0x51,
	0xC3
};

static struct dsi_cmd_desc himax_video_backlight_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 1,
		sizeof(WRDISBV), WRDISBV},
};

#ifdef USE_READ_ID
static char manufacture_id1[2] = {0xDA, 0x00}; /* DTYPE_DCS_READ */
static char manufacture_id2[2] = {0xDB, 0x00}; /* DTYPE_DCS_READ */
static char manufacture_id3[2] = {0xDC, 0x00}; /* DTYPE_DCS_READ */

static struct dsi_cmd_desc samsung_manufacture_id1_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id1), manufacture_id1};
static struct dsi_cmd_desc samsung_manufacture_id2_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id2), manufacture_id2};
static struct dsi_cmd_desc samsung_manufacture_id3_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id3), manufacture_id3};


static uint32 mipi_samsung_manufacture_id(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	uint32 id, id1;

	tp = &msd.samsung_tx_buf;
	rp = &msd.samsung_rx_buf;

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	/*read Id1 two time since fist time id read is always returning 0*/
	cmd = &samsung_manufacture_id1_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	id1 = *((uint8 *)rp->data);

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	cmd = &samsung_manufacture_id1_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	id = *((uint8 *)rp->data) | id1;
	pr_info("%s: manufacture_id1=%x\n", __func__, *rp->data);
	id <<= 8;

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	cmd = &samsung_manufacture_id2_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	pr_info("%s: manufacture_id2=%x\n", __func__, *rp->data);
	id |= *((uint8 *)rp->data);
	id <<= 8;

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	cmd = &samsung_manufacture_id3_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	pr_info("%s: manufacture_id3=%x\n", __func__, *rp->data);
	id |= *((uint8 *)rp->data);

	pr_info("%s: manufacture_id=%x\n", __func__, id);

#ifdef FACTORY_TEST
	if (id == 0x00) {
		pr_info("Lcd is not connected\n");
		is_lcd_connected = 0;
	}
#endif
	return id;
}
#endif



static int mipi_samsung_disp_send_cmd(struct msm_fb_data_type *mfd,
				       enum mipi_samsung_cmd_list cmd,
				       unsigned char lock)
{
	struct dsi_cmd_desc *cmd_desc;
	int cmd_size = 0;

/*	wake_lock(&idle_wake_lock);*//*temp*/

	if (lock)
		mutex_lock(&mfd->dma->ov_mutex);
	pr_info("%s cmd = 0x%x\n", __func__, cmd);

	switch (cmd) {
	case PANEL_READY_TO_ON:
#if CONFIG_MACH_GODIVA
		if (system_rev < BOARD_REV05) {
			cmd_desc = msd.mpd->ready_to_on.cmd;
			cmd_size = msd.mpd->ready_to_on.size;
		} else {
#else
		if (system_rev < BOARD_REV09) {
			cmd_desc = msd.mpd->ready_to_on.cmd;
			cmd_size = msd.mpd->ready_to_on.size;
		} else {
#endif
			cmd_desc = msd.mpd->ready_to_on_t05.cmd;
			cmd_size = msd.mpd->ready_to_on_t05.size;
		}
		break;
	case PANEL_READY_TO_OFF:
		cmd_desc = msd.mpd->ready_to_off.cmd;
		cmd_size = msd.mpd->ready_to_off.size;
		break;
	case PANEL_ON:
		cmd_desc = msd.mpd->on.cmd;
		cmd_size = msd.mpd->on.size;
		break;
	case PANEL_OFF:
		cmd_desc = msd.mpd->off.cmd;
		cmd_size = msd.mpd->off.size;
		break;
	case PANEL_LATE_ON:
		cmd_desc = msd.mpd->late_on.cmd;
		cmd_size = msd.mpd->late_on.size;
		break;
	case PANEL_EARLY_OFF:
		cmd_desc = msd.mpd->early_off.cmd;
		cmd_size = msd.mpd->early_off.size;
		break;
	case MTP_READ_ENABLE:
		cmd_desc = msd.mpd->mtp_read_enable.cmd;
		cmd_size = msd.mpd->mtp_read_enable.size;
		break;

	default:
		goto unknown_command;
		;
	}

	if (!cmd_size)
		goto unknown_command;

	if (lock) {

		mipi_dsi_mdp_busy_wait();
		/* Added to resolved cmd loss during dimming factory test */
		mdelay(1);

		mipi_dsi_cmds_tx(&msd.samsung_tx_buf, cmd_desc, cmd_size);

		mutex_unlock(&mfd->dma->ov_mutex);
	} else {

		mipi_dsi_mdp_busy_wait();
		/* Added to resolved cmd loss during dimming factory test */
		mdelay(1);
		mipi_dsi_cmds_tx(&msd.samsung_tx_buf, cmd_desc, cmd_size);
	}

/*	wake_unlock(&idle_wake_lock);*//*temp*/

	return 0;

unknown_command:
	if (lock)
		mutex_unlock(&mfd->dma->ov_mutex);

/*	wake_unlock(&idle_wake_lock);*//*temp*/

	return 0;
}

static unsigned char first_on;


static int mipi_samsung_disp_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	struct mipi_panel_info *mipi;
	static int boot_on;


	mfd = platform_get_drvdata(pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	mipi = &mfd->panel_info.mipi;

#ifdef USE_READ_ID
	if (unlikely(!boot_on)) {
		msd.mpd->manufacture_id = mipi_samsung_manufacture_id(mfd);
		boot_on = 1;
		/*Display was initialized in bootloader,same settings
			are carried when splash is enabled*/
		return 0;
		}
#endif
	is_negativeMode_on();
	mipi_samsung_disp_send_cmd(mfd, PANEL_READY_TO_ON, false);
	pr_debug("%s: DISP_BL_CONT_GPIO High\n", __func__);
	gpio_set_value(DISP_BL_CONT_GPIO, 1);

#if !defined(CONFIG_HAS_EARLYSUSPEND)
	mipi_samsung_disp_send_cmd(mfd, PANEL_LATE_ON, false);
#endif

	return 0;
}

static int mipi_samsung_disp_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	mfd = platform_get_drvdata(pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;
	pr_debug("%s: DISP_BL_CONT_GPIO low\n", __func__);
	gpio_set_value(DISP_BL_CONT_GPIO, 0);

	mipi_samsung_disp_send_cmd(mfd, PANEL_READY_TO_OFF, false);
	mipi_samsung_disp_send_cmd(mfd, PANEL_OFF, false);

	return 0;
}

static void __devinit mipi_samsung_disp_shutdown(struct platform_device *pdev)
{
	static struct mipi_dsi_platform_data *mipi_dsi_pdata;

	if (pdev->id != 0)
		return;

	 mipi_dsi_pdata = pdev->dev.platform_data;
	if (mipi_dsi_pdata == NULL) {
		pr_err("LCD Power off failure: No Platform Data\n");
		return;
	}
}

static void mipi_samsung_disp_backlight(struct msm_fb_data_type *mfd)
{
	struct mipi_panel_info *mipi;
	static int bl_level_old;

	pr_info("%s Back light level:%d\n", __func__, mfd->bl_level);

	mipi  = &mfd->panel_info.mipi;
	if (bl_level_old == mfd->bl_level)
		goto end;
	if (!msd.mpd->set_brightness_level ||  !mfd->panel_power_on)
		goto end;
	mutex_lock(&mfd->dma->ov_mutex);
	/* mdp4_dsi_cmd_busy_wait: will turn on dsi clock also */
	mipi_dsi_mdp_busy_wait();

	WRDISBV[1] = (unsigned char)
		(msd.mpd->set_brightness_level(mfd->bl_level));

	mipi_dsi_cmds_tx(&msd.samsung_tx_buf, himax_video_backlight_cmds,
		ARRAY_SIZE(himax_video_backlight_cmds));

	bl_level_old = mfd->bl_level;
	mutex_unlock(&mfd->dma->ov_mutex);

end:
	return;
}



#if defined(CONFIG_HAS_EARLYSUSPEND)
static void mipi_samsung_disp_early_suspend(struct early_suspend *h)
{
	struct msm_fb_data_type *mfd;
	pr_info("%s", __func__);

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd)) {
		pr_info("%s NO PDEV.\n", __func__);
		return;
	}
	if (unlikely(mfd->key != MFD_KEY)) {
		pr_info("%s MFD_KEY is not matched.\n", __func__);
		return;
	}

	mipi_samsung_disp_send_cmd(mfd, PANEL_EARLY_OFF, true);

	mfd->resume_state = MIPI_SUSPEND_STATE;
}

static void mipi_samsung_disp_late_resume(struct early_suspend *h)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd)) {
		pr_info("%s NO PDEV.\n", __func__);
		return;
	}
	if (unlikely(mfd->key != MFD_KEY)) {
		pr_info("%s MFD_KEY is not matched.\n", __func__);
		return;
	}

	mfd->resume_state = MIPI_RESUME_STATE;
	pr_info("%s", __func__);
}
#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
#ifdef WA_FOR_FACTORY_MODE
static ssize_t mipi_samsung_disp_get_power(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct msm_fb_data_type *mfd;
	int rc;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	rc = snprintf((char *)buf, sizeof(buf), "%d\n", mfd->panel_power_on);
	pr_info("mipi_samsung_disp_get_power(%d)\n", mfd->panel_power_on);

	return rc;
}

static ssize_t mipi_samsung_disp_set_power(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct msm_fb_data_type *mfd;
	unsigned int power;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (sscanf(buf, "%u", &power) != 1)
		return -EINVAL;

	if (power == mfd->panel_power_on)
		return 0;

	if (power) {
		mfd->fbi->fbops->fb_blank(FB_BLANK_UNBLANK, mfd->fbi);
		mfd->fbi->fbops->fb_pan_display(&mfd->fbi->var, mfd->fbi);
		mipi_samsung_disp_send_cmd(mfd, PANEL_LATE_ON, true);
		mipi_samsung_disp_backlight(mfd);
	} else {
		mfd->fbi->fbops->fb_blank(FB_BLANK_POWERDOWN, mfd->fbi);
	}

	pr_info("mipi_samsung_disp_set_power\n");

	return size;
}
#else
static int mipi_samsung_disp_get_power(struct lcd_device *dev)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	pr_info("mipi_samsung_disp_get_power(%d)\n", mfd->panel_power_on);

	return mfd->panel_power_on;
}

static int mipi_samsung_disp_set_power(struct lcd_device *dev, int power)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (power == mfd->panel_power_on)
		return 0;

	if (power) {
		mfd->fbi->fbops->fb_blank(FB_BLANK_UNBLANK, mfd->fbi);
		mfd->fbi->fbops->fb_pan_display(&mfd->fbi->var, mfd->fbi);
		mipi_samsung_disp_send_cmd(mfd, PANEL_LATE_ON, true);
		mipi_samsung_disp_backlight(mfd);
	} else {
		mfd->fbi->fbops->fb_blank(FB_BLANK_POWERDOWN, mfd->fbi);
	}

	pr_info("mipi_samsung_disp_set_power\n");
	return 0;
}
#endif

static ssize_t mipi_samsung_disp_lcdtype_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	char temp[20];

	snprintf(temp, strnlen(msd.mpd->panel_name, 20) + 1,
						msd.mpd->panel_name);
	strlcat(buf, temp, 20);
	return strnlen(buf, 20);
}

static ssize_t mipi_samsung_disp_gamma_mode_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = snprintf((char *)buf, sizeof(buf), "%d\n", msd.dstat.gamma_mode);
	pr_info("gamma_mode: %d\n", *buf);

	return rc;
}

static ssize_t mipi_samsung_disp_gamma_mode_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	if (sysfs_streq(buf, "1") && !msd.dstat.gamma_mode) {
		/* 1.9 gamma */
		msd.dstat.gamma_mode = GAMMA_1_9;
	} else if (sysfs_streq(buf, "0") && msd.dstat.gamma_mode) {
		/* 2.2 gamma */
		msd.dstat.gamma_mode = GAMMA_2_2;
	} else {
		pr_info("%s: Invalid argument!!", __func__);
	}

	return size;
}

static ssize_t mipi_samsung_disp_acl_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = snprintf((char *)buf, sizeof(buf), "%d\n", msd.dstat.acl_on);
	pr_info("acl status: %d\n", *buf);

	return rc;
}

static ssize_t mipi_samsung_disp_acl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (!mfd->panel_power_on) {
		pr_info("%s: panel is off state. updating state value.\n",
			__func__);
		if (sysfs_streq(buf, "1") && !msd.dstat.acl_on)
			msd.dstat.acl_on = true;
		else if (sysfs_streq(buf, "0") && msd.dstat.acl_on)
			msd.dstat.acl_on = false;
		else
			pr_info("%s: Invalid argument!!", __func__);
	} else {

#ifdef CONFIG_ACL_FUNCTION /*temp*/
		if (sysfs_streq(buf, "1") && !msd.dstat.acl_on) {
			if (msd.mpd->set_acl(mfd->bl_level))
				mipi_samsung_disp_send_cmd(
					mfd, PANEL_ACL_OFF, true);
			else {
				mipi_samsung_disp_send_cmd(
					mfd, PANEL_ACL_ON, true);
				mipi_samsung_disp_send_cmd(
					mfd, PANEL_ACL_UPDATE, true);
			}
			msd.dstat.acl_on = true;
		} else if (sysfs_streq(buf, "0") && msd.dstat.acl_on) {
			mipi_samsung_disp_send_cmd(mfd, PANEL_ACL_OFF, true);
			msd.dstat.acl_on = false;
		} else {
			pr_info("%s: Invalid argument!!", __func__);
		}
#endif
	}

	return size;
}

static ssize_t mipi_samsung_auto_brightness_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = snprintf((char *)buf, sizeof(buf), "%d\n",
					msd.dstat.auto_brightness);
	pr_info("auot_brightness: %d\n", *buf);

	return rc;
}

static ssize_t mipi_samsung_auto_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	if (sysfs_streq(buf, "1"))
		msd.dstat.auto_brightness = 1;
	else if (sysfs_streq(buf, "0"))
		msd.dstat.auto_brightness = 0;
	else
		pr_info("%s: Invalid argument!!", __func__);

	return size;
}


static struct lcd_ops mipi_samsung_disp_props = {
#ifdef WA_FOR_FACTORY_MODE
	.get_power = NULL,
	.set_power = NULL,
#else
	.get_power = mipi_samsung_disp_get_power,
	.set_power = mipi_samsung_disp_set_power,
#endif
};

#ifdef WA_FOR_FACTORY_MODE
static DEVICE_ATTR(lcd_power, S_IRUGO | S_IWUSR,
			mipi_samsung_disp_get_power,
			mipi_samsung_disp_set_power);
#endif
static DEVICE_ATTR(lcd_type, S_IRUGO, mipi_samsung_disp_lcdtype_show, NULL);
static DEVICE_ATTR(gamma_mode, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_samsung_disp_gamma_mode_show,
			mipi_samsung_disp_gamma_mode_store);
static DEVICE_ATTR(power_reduce, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_samsung_disp_acl_show,
			mipi_samsung_disp_acl_store);
static DEVICE_ATTR(auto_brightness, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_samsung_auto_brightness_show,
			mipi_samsung_auto_brightness_store);

#endif


#ifdef READ_REGISTER_ESD
#define ID_05H_IDLE 0x0
#define ID_E5H_IDLE 0x80
#define ID_0AH_IDLE 0x9c

static char error_id1[2] = {0x05, 0x00}; /* DTYPE_DCS_READ */
static char error_id2[2] = {0xE5, 0x00}; /* DTYPE_DCS_READ */
static char error_id3[2] = {0x0A, 0x00}; /* DTYPE_DCS_READ */

static struct dsi_cmd_desc error_id1_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 0, sizeof(error_id1), error_id1};
static struct dsi_cmd_desc error_id2_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 0, sizeof(error_id2), error_id2};
static struct dsi_cmd_desc error_id3_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 0, sizeof(error_id3), error_id3};

static char error_buf[3];

static void read_error_register(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;

	mipi_samsung_disp_send_cmd(mfd, MTP_READ_ENABLE, false);

/*	wake_lock(&idle_wake_lock);*//*temp*/
	mutex_lock(&mfd->dma->ov_mutex);

	mdp4_dsi_cmd_dma_busy_wait(mfd);
	mdp4_dsi_blt_dmap_busy_wait(mfd);
	mipi_dsi_mdp_busy_wait();

	tp = &msd.samsung_tx_buf;
	rp = &msd.samsung_rx_buf;

	cmd = &error_id2_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	error_buf[1] = *rp->data;

	cmd = &error_id3_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	error_buf[2] = *rp->data;

	mutex_unlock(&mfd->dma->ov_mutex);

/*	wake_unlock(&idle_wake_lock);*//*temp*/
}

static void esd_test_work_func(struct work_struct *work)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (unlikely(!mfd)) {
		pr_info("%s NO PDEV.\n", __func__);
		return;
	}

	pr_debug("%s start", __func__);
	read_error_register(mfd);
	pr_debug("%s end E5H=0x%x 0AH=%x\n", __func__,
					error_buf[1], error_buf[2]);

	if ((ID_E5H_IDLE ^ error_buf[1]) || (ID_0AH_IDLE ^ error_buf[2])) {

		pr_info("%s: E5H=%x 0AH=%x\n", __func__,
				error_buf[1], error_buf[2]);

		esd_execute();
	}

/*	if (mfd->resume_state != MIPI_SUSPEND_STATE)//temp
		queue_delayed_work(msd.mpd->esd_workqueue,
				&(msd.mpd->esd_work), ESD_INTERVAL * HZ); */

}
#endif

static int __devinit mipi_samsung_disp_probe(struct platform_device *pdev)
{

	struct platform_device *msm_fb_added_dev;
#if defined(CONFIG_LCD_CLASS_DEVICE)
	struct lcd_device *lcd_device;
	int ret;
#endif
#if defined(CONFIG_BACKLIGHT_CLASS_DEVICE)
	struct backlight_device *bd;
#endif
	msd.dstat.acl_on = false;

	if (pdev->id == 0) {
		msd.mipi_samsung_disp_pdata = pdev->dev.platform_data;

		first_on = false;
		return 0;
	}

	msm_fb_added_dev = msm_fb_add_device(pdev);

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_LCD_CLASS_DEVICE)
	msd.msm_pdev = msm_fb_added_dev;
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND)
	msd.early_suspend.suspend = mipi_samsung_disp_early_suspend;
	msd.early_suspend.resume = mipi_samsung_disp_late_resume;
	msd.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	register_early_suspend(&msd.early_suspend);

#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
	lcd_device = lcd_device_register("panel", &pdev->dev, NULL,
					&mipi_samsung_disp_props);

	if (IS_ERR(lcd_device)) {
		ret = PTR_ERR(lcd_device);
		printk(KERN_ERR "lcd : failed to register device\n");
		return ret;
	}

#ifdef WA_FOR_FACTORY_MODE
	sysfs_remove_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_power.attr);

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_power.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_lcd_power.attr.name);
	}
#endif

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_type.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_lcd_type.attr.name);
	}

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_gamma_mode.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_gamma_mode.attr.name);
	}

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_power_reduce.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_power_reduce.attr.name);
	}
#if defined(CONFIG_BACKLIGHT_CLASS_DEVICE)
	bd = backlight_device_register("panel", &lcd_device->dev,
						NULL, NULL, NULL);
	if (IS_ERR(bd)) {
		ret = PTR_ERR(bd);
		pr_info("backlight : failed to register device\n");
		return ret;
	}

	ret = sysfs_create_file(&bd->dev.kobj,
					&dev_attr_auto_brightness.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_auto_brightness.attr.name);
	}
#endif
#endif
#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT)
		/*	mdnie sysfs create */
		init_mdnie_class();
#endif

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_samsung_disp_probe,
	.driver = {
		.name   = "mipi_samsung_oled",
	},
	.shutdown = mipi_samsung_disp_shutdown
};

static struct msm_fb_panel_data samsung_panel_data = {
	.on		= mipi_samsung_disp_on,
	.off		= mipi_samsung_disp_off,
	.set_backlight	= mipi_samsung_disp_backlight,
};

static int ch_used[3];

int mipi_samsung_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel,
					struct mipi_panel_data *mpd)
{
	struct platform_device *pdev = NULL;
	int ret = 0;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_samsung_oled",
					   (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	samsung_panel_data.panel_info = *pinfo;
	msd.mpd = mpd;
	if (!msd.mpd) {
		printk(KERN_ERR
		  "%s: get mipi_panel_data failed!\n", __func__);
		goto err_device_put;
	}
	mpd->msd = &msd;
	ret = platform_device_add_data(pdev, &samsung_panel_data,
		sizeof(samsung_panel_data));
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_add_data failed!\n", __func__);
		goto err_device_put;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_register failed!\n", __func__);
		goto err_device_put;
	}

	return ret;

err_device_put:
	platform_device_put(pdev);
	return ret;
}


static int __init current_boot_mode(char *mode)
{
	/*
	*	1 is recovery booting
	*	0 is normal booting
	*/

	if (strncmp(mode, "1", 1) == 0)
		recovery_boot_mode = 1;
	else
		recovery_boot_mode = 0;

	pr_debug("%s %s", __func__, recovery_boot_mode == 1 ?
						"recovery" : "normal");
	return 1;
}
__setup("androidboot.boot_recovery=", current_boot_mode);

static int __init mipi_samsung_disp_init(void)
{
	mipi_dsi_buf_alloc(&msd.samsung_tx_buf, DSI_BUF_SIZE);
	mipi_dsi_buf_alloc(&msd.samsung_rx_buf, DSI_BUF_SIZE);

/*	wake_lock_init(&idle_wake_lock,
WAKE_LOCK_IDLE, "MIPI idle lock");*/ /*temp*/

	return platform_driver_register(&this_driver);
}

module_init(mipi_samsung_disp_init);
