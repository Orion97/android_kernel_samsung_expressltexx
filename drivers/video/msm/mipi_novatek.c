/* Copyright (c) 2010-2012, Code Aurora Forum. All rights reserved.
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

#include <linux/lcd.h>
#ifdef CONFIG_SPI_QUP
#include <linux/spi/spi.h>
#endif

#if defined(CONFIG_MACH_INGRAHAM2)
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#endif
#include <linux/leds.h>
#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_novatek.h"
#include "lcd_backlight_ic.h"
#include "mdp4.h"
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)
#include "mdp4_video_enhance.h"
#endif
#if defined(CONFIG_MACH_INGRAHAM2)
static struct delayed_work  det_work;
static void mipi_novatek_disp_set_backlight(struct msm_fb_data_type *mfd);
static void blenable_work_func(struct work_struct *work);
#endif

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)
static struct mipi_novatek_driver_data msd;
int is_lcd_connected = 1;
struct dcs_cmd_req cmdreq;


static char manufacture_id1[2] = {0xDA, 0x00}; /* DTYPE_DCS_READ */
static char manufacture_id2[2] = {0xDB, 0x00}; /* DTYPE_DCS_READ */
static char manufacture_id3[2] = {0xDC, 0x00}; /* DTYPE_DCS_READ */

static struct dsi_cmd_desc novatek_manufacture_id1_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id1), manufacture_id1};
static struct dsi_cmd_desc novatek_manufacture_id2_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id2), manufacture_id2};
static struct dsi_cmd_desc novatek_manufacture_id3_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id3), manufacture_id3};
static uint32 mipi_novatek_disp_manufacture_id(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	uint32 id;

	tp = &msd.novatek_tx_buf;
	rp = &msd.novatek_rx_buf;
	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);

	cmd = &novatek_manufacture_id1_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	pr_info("%s: manufacture_id1=%x\n", __func__, *rp->data);
	id = *((unsigned char *)rp->data);
	id <<= 8;

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	cmd = &novatek_manufacture_id2_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	pr_info("%s: manufacture_id2=%x\n", __func__, *rp->data);
	id |= *((unsigned char *)rp->data);
	id <<= 8;

	mipi_dsi_buf_init(rp);
	mipi_dsi_buf_init(tp);
	cmd = &novatek_manufacture_id3_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 1);
	pr_info("%s: manufacture_id3=%x\n", __func__, *rp->data);
	id |= *((unsigned char *)rp->data);

	pr_info("%s: manufacture_id=%x\n", __func__, id);
	return id;
}

static int mipi_novatek_disp_send_cmd(struct msm_fb_data_type *mfd,
				       enum mipi_novatek_cmd_list cmd,
				       unsigned char lock)
{
	struct dsi_cmd_desc *cmd_desc;
	int cmd_size = 0;

	if (lock)
		mutex_lock(&mfd->dma->ov_mutex);
		pr_info("%s cmd = 0x%x\n", __func__, cmd);

	switch (cmd) {
	case PANEL_READY_TO_ON:
		cmd_desc = msd.mpd->ready_to_on_boe.cmd;
		cmd_size = msd.mpd->ready_to_on_boe.size;
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

	default:
		goto unknown_command;
		;
	}

	if (!cmd_size)
		goto unknown_command;

	if (lock) {
		cmdreq.cmds = cmd_desc;
		cmdreq.cmds_cnt = cmd_size;
		cmdreq.flags = CMD_REQ_COMMIT;
		cmdreq.rlen = 0;
		cmdreq.cb = NULL;
		mipi_dsi_cmdlist_put(&cmdreq);

		mutex_unlock(&mfd->dma->ov_mutex);
	} else {
		cmdreq.cmds = cmd_desc;
		cmdreq.cmds_cnt = cmd_size;
		cmdreq.flags = CMD_REQ_COMMIT;
		cmdreq.rlen = 0;
		cmdreq.cb = NULL;
		mipi_dsi_cmdlist_put(&cmdreq);
	}

	return 0;

unknown_command:
	if (lock)
		mutex_unlock(&mfd->dma->ov_mutex);

	return 0;
}
#endif

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
void mipi_novatek_disp_on_cmd(struct msm_fb_data_type *mfd)
{
	struct mipi_panel_info *mipi;

	mipi = &mfd->panel_info.mipi;
	if (mfd->display_on_status == MIPI_SUSPEND_STATE) {
		if (mipi->mode == DSI_CMD_MODE) {
			mipi_novatek_disp_send_cmd(mfd, PANEL_ON, true);
			mfd->display_on_status = MIPI_RESUME_STATE;
			pr_info("%s status : %d ",
					__func__, mfd->display_on_status);
		}
	}
}
#endif

static int mipi_novatek_disp_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	struct mipi_panel_info *mipi;
	static int first_power_on;
	int id_count = 0;

	pr_info("****** %s *******\n", __func__);

	mfd = platform_get_drvdata(pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	mipi = &mfd->panel_info.mipi;
#if defined(CONFIG_MACH_INGRAHAM2)
	if (first_power_on == 0) {
		do {
			msd.mpd->manufacture_id =
				mipi_novatek_disp_manufacture_id(mfd);
			id_count++;
		} while (id_count < 3 && !msd.mpd->manufacture_id);
		if (id_count >= 3 && !msd.mpd->manufacture_id) {
			pr_info("Lcd is not connected\n");
			is_lcd_connected = 0;
		}
		first_power_on++;
	}
#endif
	mipi_novatek_disp_send_cmd(mfd, PANEL_READY_TO_ON, false);
#if defined(CONFIG_MACH_INGRAHAM2)
	INIT_DELAYED_WORK(&det_work, blenable_work_func);
	schedule_delayed_work(&det_work, msecs_to_jiffies(100));
#endif
	if (mipi->mode == DSI_VIDEO_MODE)
		mipi_novatek_disp_send_cmd(mfd, PANEL_ON, false);

#if !defined(CONFIG_HAS_EARLYSUSPEND)
	mipi_novatek_disp_send_cmd(mfd, PANEL_LATE_ON, false);
#endif/* CONFIG_HAS_EARLYSUSPEND */
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_enable();
#endif
	pr_info("%s:Display on completed\n", __func__);
return 0;
}
static int mipi_novatek_disp_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_disable();
#endif
	mfd = platform_get_drvdata(pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	mipi_novatek_disp_send_cmd(mfd, PANEL_READY_TO_OFF, false);
	mipi_novatek_disp_send_cmd(mfd, PANEL_OFF, false);
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
	mfd->display_on_status = MIPI_SUSPEND_STATE;
#endif
	pr_info("%s:Display off completed\n", __func__);
	return 0;
}
#if defined(CONFIG_HAS_EARLYSUSPEND)
static void mipi_novatek_disp_early_suspend(struct early_suspend *h)
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

	mipi_novatek_disp_send_cmd(mfd, PANEL_EARLY_OFF, true);
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_disable();
#endif
	pr_info("%s:Display suspend completed\n", __func__);
}
static void mipi_novatek_disp_late_resume(struct early_suspend *h)
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

#if !defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
	mipi_novatek_disp_send_cmd(mfd, PANEL_LATE_ON, true);
#endif
	pr_info("%s:Display resume completed\n", __func__);
}
#endif
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
void set_esd_refresh(boolean stat)
{
	msd.esd_refresh = stat;
}
#endif
static void mipi_novatek_disp_set_backlight(struct msm_fb_data_type *mfd)
{
	static int bl_level_old;
	pr_info("%s Back light level:%d\n", __func__, mfd->bl_level);


	if (bl_level_old == mfd->bl_level)
		return;

	ktd253_set_brightness(mfd->bl_level);
	bl_level_old = mfd->bl_level;
	return;
}
#if defined(CONFIG_MACH_INGRAHAM2)
static void blenable_work_func(struct work_struct *work)
{
	struct msm_fb_data_type *mfd;
	pr_info("%s :backlight made on after 100msec\n", __func__);
	mfd = platform_get_drvdata(msd.msm_pdev);
	if (!mfd->bl_level)
		mfd->bl_level = 0x6C;
	mipi_novatek_disp_set_backlight(mfd);
}
#endif
#if defined(CONFIG_LCD_CLASS_DEVICE)
static ssize_t mipi_novatek_lcdtype_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	char temp[20];

	snprintf(temp, strnlen(msd.mpd->panel_name, 20) + 1,
						msd.mpd->panel_name);
	strlcat(buf, temp, 20);
	return strnlen(buf, 20);
}
static struct lcd_ops mipi_novatek_disp_props;

static DEVICE_ATTR(lcd_type, S_IRUGO, mipi_novatek_lcdtype_show, NULL);
#endif
static int __devinit mipi_novatek_disp_probe(struct platform_device *pdev)
{
	struct platform_device *msm_fb_added_dev;
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
	struct msm_fb_data_type *mfd;
#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
	int ret;
	struct lcd_device *lcd_device;
#endif

	msd.dstat.acl_on = false;
	if (pdev->id == 0) {
		msd.mipi_novatek_disp_pdata = pdev->dev.platform_data;
		return 0;
	}

	msm_fb_added_dev = msm_fb_add_device(pdev);

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_LCD_CLASS_DEVICE)
	msd.msm_pdev = msm_fb_added_dev;
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND)
	msd.early_suspend.suspend = mipi_novatek_disp_early_suspend;
	msd.early_suspend.resume = mipi_novatek_disp_late_resume;
	msd.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	register_early_suspend(&msd.early_suspend);

#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
	lcd_device = lcd_device_register("panel", &pdev->dev, NULL,
					&mipi_novatek_disp_props);

	if (IS_ERR(lcd_device)) {
		ret = PTR_ERR(lcd_device);
		printk(KERN_ERR "lcd : failed to register device\n");
		return ret;
	}

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_type.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_lcd_type.attr.name);
	}
#endif
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)
	/*init_mdnie_class(); */
#endif

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
	mfd = platform_get_drvdata(msm_fb_added_dev);
	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;

	mfd->cmd_panel_disp_on = mipi_novatek_disp_on_cmd;
	mfd->display_on_status = MIPI_SUSPEND_STATE;
#endif
	pr_info("%s:Display probe completed\n", __func__);
	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_novatek_disp_probe,
	.driver = {
		.name   = "mipi_novatek_nt35510",
	},
};

static struct msm_fb_panel_data novatek_panel_data = {
	.on		= mipi_novatek_disp_on,
	.off		= mipi_novatek_disp_off,
	.set_backlight	= mipi_novatek_disp_set_backlight,
};

static int ch_used[3];

int mipi_novatek_disp_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel,
					struct mipi_panel_data *mpd)
{
	struct platform_device *pdev = NULL;
	int ret = 0;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_novatek_nt35510",
					   (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	novatek_panel_data.panel_info = *pinfo;
	msd.mpd = mpd;
	if (!msd.mpd) {
		printk(KERN_ERR
		  "%s: get mipi_panel_data failed!\n", __func__);
		goto err_device_put;
	}
	mpd->msd = &msd;
	ret = platform_device_add_data(pdev, &novatek_panel_data,
		sizeof(novatek_panel_data));
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

static int __init mipi_novatek_disp_init(void)
{
	mipi_dsi_buf_alloc(&msd.novatek_tx_buf, DSI_BUF_SIZE);
	mipi_dsi_buf_alloc(&msd.novatek_rx_buf, DSI_BUF_SIZE);

	return platform_driver_register(&this_driver);
}
module_init(mipi_novatek_disp_init);

