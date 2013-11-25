/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/gpio.h>
#include <asm/mach-types.h>
#include <mach/msm_bus_board.h>
#include <mach/msm_memtypes.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include <linux/msm_ion.h>
#include <mach/ion.h>

#include "devices.h"
#include "board-8930.h"

#if defined(CONFIG_FB_MSM_TRIPLE_BUFFER)
/* prim = 540 x 960 x 4(bpp) x 3(pages) */
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
#define MSM_FB_PRIM_BUF_SIZE (1280 * 800 * 4 * 3)
#elif defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL)
#define MSM_FB_PRIM_BUF_SIZE (1280 * 736 * 4 * 3)
#elif defined(CONFIG_FB_MSM_MIPI_MAGNA_OLED_VIDEO_WVGA_PT_PANEL) || \
		defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
#define MSM_FB_PRIM_BUF_SIZE (800 * 480 * 4 * 3)
#else
#define MSM_FB_PRIM_BUF_SIZE \
		(roundup((1920 * 1088 * 4), 4096) * 3) /* 4 bpp x 3 pages */
#endif
#else
/* prim = 540 x 960 x 4(bpp) x 2(pages) */
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
#define MSM_FB_PRIM_BUF_SIZE (1280 * 800 * 4 * 2)
#else
#define MSM_FB_PRIM_BUF_SIZE \
		(roundup((1920 * 1088 * 4), 4096) * 2) /* 4 bpp x 2 pages */
#endif
#endif
/* Note: must be multiple of 4096 */
#define MSM_FB_SIZE roundup(MSM_FB_PRIM_BUF_SIZE, 4096)

#ifdef CONFIG_FB_MSM_OVERLAY0_WRITEBACK
#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL)
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((1280 * 736 * 3 * 2), 4096)
#elif defined(CONFIG_FB_MSM_MIPI_MAGNA_OLED_VIDEO_WVGA_PT_PANEL) || \
		defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((800 * 480 * 3 * 2), 4096)
#elif defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((1280 * 800 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((1376 * 768 * 3 * 2), 4096)
#endif
#else
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY0_WRITEBACK */

#ifdef CONFIG_FB_MSM_OVERLAY1_WRITEBACK
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE roundup((1920 * 1088 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY1_WRITEBACK */

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL)
#define MDP_VSYNC_GPIO 0
#else
#define MDP_VSYNC_GPIO -1
#endif
#define MIPI_VIDEO_NOVATEK_WXGA_PANEL_NAME "mipi_novatek_tft_video_wxga"
#define MIPI_CMD_NOVATEK_QHD_PANEL_NAME	"mipi_cmd_novatek_qhd"
#define MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME	"mipi_video_novatek_qhd"
#define MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME	"mipi_video_toshiba_wsvga"
#define MIPI_VIDEO_CHIMEI_WXGA_PANEL_NAME	"mipi_video_chimei_wxga"
#define MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME	"mipi_video_simulator_vga"
#define MIPI_CMD_RENESAS_FWVGA_PANEL_NAME	"mipi_cmd_renesas_fwvga"
#define HDMI_PANEL_NAME	"hdmi_msm"
#define MHL_PANEL_NAME "hdmi_msm,mhl_8334"
#define TVOUT_PANEL_NAME	"tvout_msm"

#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
unsigned char hdmi_is_primary = 1;
#else
unsigned char hdmi_is_primary;
#endif

static unsigned char mhl_display_enabled;

unsigned char msm8930_mhl_display_enabled(void)
{
	return mhl_display_enabled;
}

static struct resource msm_fb_resources[] = {
	{
		.flags = IORESOURCE_DMA,
	}
};

static int msm_fb_detect_panel(const char *name)
{
	if (!strncmp(name, MIPI_CMD_NOVATEK_QHD_PANEL_NAME,
			strnlen(MIPI_CMD_NOVATEK_QHD_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

#if !defined(CONFIG_FB_MSM_LVDS_MIPI_PANEL_DETECT) && \
	!defined(CONFIG_FB_MSM_MIPI_PANEL_DETECT)
	if (!strncmp(name, MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME,
			strnlen(MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;
	if (!strncmp(name, MIPI_VIDEO_NOVATEK_WXGA_PANEL_NAME,
			strnlen(MIPI_VIDEO_NOVATEK_WXGA_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	if (!strncmp(name, MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME,
			strnlen(MIPI_VIDEO_TOSHIBA_WSVGA_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	if (!strncmp(name, MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME,
			strnlen(MIPI_VIDEO_SIMULATOR_VGA_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	if (!strncmp(name, MIPI_CMD_RENESAS_FWVGA_PANEL_NAME,
			strnlen(MIPI_CMD_RENESAS_FWVGA_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;
#endif

	if (!strncmp(name, HDMI_PANEL_NAME,
			strnlen(HDMI_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	if (!strncmp(name, TVOUT_PANEL_NAME,
			strnlen(TVOUT_PANEL_NAME,
				PANEL_NAME_MAX_LEN)))
		return 0;

	pr_warning("%s: not supported '%s'", __func__, name);
	return -ENODEV;
}

static struct msm_fb_platform_data msm_fb_pdata = {
	.detect_client = msm_fb_detect_panel,
};

static struct platform_device msm_fb_device = {
	.name   = "msm_fb",
	.id     = 0,
	.num_resources     = ARRAY_SIZE(msm_fb_resources),
	.resource          = msm_fb_resources,
	.dev.platform_data = &msm_fb_pdata,
};

static bool dsi_power_on;

/*
 * TODO: When physical 8930/PM8038 hardware becomes
 * available, replace mipi_dsi_cdp_panel_power with
 * appropriate function.
 */
#define DISP_RST_GPIO 58
#ifdef CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL
#define DISP_BL_CONT_GPIO 10
#define MSM_VGH_DET 19
#endif
#ifdef CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT_PANEL
#define DISP_BL_CONT_GPIO 34
#endif
#ifdef CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL
#define LCD_EN 89
#if defined(CONFIG_MSM_GPIO_BACKLIGHT_PWM)
#define MSM_GPIO_BACKLIGHT_PWM 3
#endif
#define MSM_GPIO_BACKLIGHT_RST 64
#define MSM_GPIO_BL_I2C_SDA 95
#define MSM_GPIO_BL_I2C_SCL 96
#endif

void pull_ldi_reset_down(void)
{
	int rc;
	rc = gpio_request(DISP_RST_GPIO, "disp_rst_n");
	if (rc) {
		pr_err("request gpio DISP_RST_GPIO failed, rc=%d\n",
			rc);
		gpio_free(DISP_RST_GPIO);
		return;
	}
	gpio_tlmm_config(GPIO_CFG(DISP_RST_GPIO,  0, GPIO_CFG_OUTPUT,
			GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);
	msleep(50);
	gpio_set_value(DISP_RST_GPIO, 0);
}

static int mipi_dsi_cdp_panel_power(int on)
{
	static struct regulator *reg_l8, *reg_l23, *reg_l2;
	int rc;
	static int gpio1;	/* BACKLIGHT_PWM */


	pr_debug("%s: state : %d\n", __func__, on);

	if (!dsi_power_on) {

		reg_l8 = regulator_get(&msm_mipi_dsi1_device.dev,
				"dsi_vdc");
		if (IS_ERR(reg_l8)) {
			pr_err("could not get 8038_l8, rc = %ld\n",
				PTR_ERR(reg_l8));
			return -ENODEV;
		}
		reg_l23 = regulator_get(&msm_mipi_dsi1_device.dev,
				"dsi_vddio");
		if (IS_ERR(reg_l23)) {
			pr_err("could not get 8038_l23, rc = %ld\n",
				PTR_ERR(reg_l23));
			return -ENODEV;
		}
		reg_l2 = regulator_get(&msm_mipi_dsi1_device.dev,
				"dsi_vdda");
		if (IS_ERR(reg_l2)) {
			pr_err("could not get 8038_l2, rc = %ld\n",
				PTR_ERR(reg_l2));
			return -ENODEV;
		}
		rc = regulator_set_voltage(reg_l8, 3000000, 3000000);
		if (rc) {
			pr_err("set_voltage l8 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_voltage(reg_l23, 1800000, 1800000);
		if (rc) {
			pr_err("set_voltage l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_voltage(reg_l2, 1200000, 1200000);
		if (rc) {
			pr_err("set_voltage l2 failed, rc=%d\n", rc);
			return -EINVAL;
		}
#if !defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
		rc = gpio_request(DISP_RST_GPIO, "disp_rst_n");
		if (rc) {
			pr_err("request gpio DISP_RST_GPIO failed, rc=%d\n",
				rc);
			gpio_free(DISP_RST_GPIO);
			return -ENODEV;
		}

		gpio_tlmm_config(GPIO_CFG(DISP_RST_GPIO,  0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
				GPIO_CFG_ENABLE);
#endif
#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL) \
	|| defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)

		rc = gpio_request(DISP_BL_CONT_GPIO, "disp_BL_n");
		if (rc) {
			pr_err("request gpio DISP_RST_GPIO failed, rc=%d\n",
				rc);
			gpio_free(DISP_BL_CONT_GPIO);
			return -ENODEV;
		}
		usleep(5000);

		gpio_tlmm_config(GPIO_CFG(DISP_BL_CONT_GPIO, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
				GPIO_CFG_ENABLE);
#elif defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
		rc = gpio_request(LCD_EN, "LCD_EN");
		if (rc) {
			pr_err("request gpio LCD_EN failed, rc=%d\n",
					rc);
			gpio_free(LCD_EN);
			return -ENODEV;
		}

		gpio_tlmm_config(GPIO_CFG(LCD_EN,  0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
				GPIO_CFG_ENABLE);

	    rc = gpio_request(MSM_GPIO_BACKLIGHT_RST, "MSM_GPIO_BACKLIGHT_RST");
		if (rc) {
			pr_err("request gpio MSM_GPIO_BACKLIGHT_RST failed, rc=%d\n",
					rc);
			gpio_free(MSM_GPIO_BACKLIGHT_RST);
			return -ENODEV;
		}

		gpio_tlmm_config(GPIO_CFG(MSM_GPIO_BACKLIGHT_RST, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
				GPIO_CFG_ENABLE);
		rc = gpio_request(MSM_GPIO_BL_I2C_SDA,
		"MSM_GPIO_BACKLIGHT_I2C_SDA");
		if (rc) {
			pr_err("request gpio MSM_GPIO_BL_I2C_SDA failed, rc=%d\n",
					rc);
			gpio_free(MSM_GPIO_BL_I2C_SDA);
			return -ENODEV;
		}
		  rc = gpio_request(MSM_GPIO_BL_I2C_SCL,
		  "MSM_GPIO_BACKLIGHT_I2C_SCL");
		if (rc) {
			pr_err("request gpio MSM_GPIO_BL_I2C_SCL failed, rc=%d\n",
					rc);
			gpio_free(MSM_GPIO_BL_I2C_SCL);
			return -ENODEV;
		}
#if defined(CONFIG_MSM_GPIO_BACKLIGHT_PWM)
		rc = gpio_request(MSM_GPIO_BACKLIGHT_PWM,
		"MSM_GPIO_BACKLIGHT_PWM");
		if (rc) {
			pr_err("request gpio MSM_GPIO_BACKLIGHT_PWM faled,rc=%d\n",
					rc);
			gpio_free(MSM_GPIO_BACKLIGHT_PWM);
			return -ENODEV;
		}

		gpio_tlmm_config(GPIO_CFG(MSM_GPIO_BACKLIGHT_PWM,  0,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
					GPIO_CFG_ENABLE);
#endif
#endif
#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL)
		rc = gpio_request(MSM_VGH_DET, "VGH_DET");
		if (rc) {
			pr_err("request gpio MSM_VGH_DET failed, rc=%d\n",
				rc);
			gpio_free(MSM_VGH_DET);
			return -ENODEV;
		}

		gpio_tlmm_config(GPIO_CFG(MSM_VGH_DET, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
				GPIO_CFG_ENABLE);
#endif
		dsi_power_on = true;
	}
	if (on) {
		rc = regulator_set_optimum_mode(reg_l8, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l8 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_optimum_mode(reg_l23, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_optimum_mode(reg_l2, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l2 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_enable(reg_l2);
		if (rc) {
			pr_err("enable l2 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_enable(reg_l23); /*1.8V*/
		if (rc) {
			pr_err("enable l23 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		usleep(5000);
		rc = regulator_enable(reg_l8); /*3V*/
		if (rc) {
			pr_err("enable l8 failed, rc=%d\n", rc);
			return -ENODEV;
		}

#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL) \
	|| defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)
		usleep(5000);/*HW engr request*/
		gpio_set_value(DISP_RST_GPIO, 1);
		pr_debug("%s: DISP_RST_GPIO high\n", __func__);
		msleep(120);
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_BOE_CMD_WVGA_PT)
		gpio_set_value(DISP_BL_CONT_GPIO, 1); /*backlight signal*/
		pr_debug("%s: DISP_BL_CONT_GPIO high\n", __func__);
#endif
#elif defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
		msleep(20);
		gpio_set_value(MSM_GPIO_BL_I2C_SDA, 0);
		msleep(20);
		gpio_set_value(MSM_GPIO_BL_I2C_SCL, 0);
		msleep(20);
		gpio_set_value(MSM_GPIO_BACKLIGHT_RST, 1);
		msleep(20);
		gpio_set_value(MSM_GPIO_BACKLIGHT_RST, 0);
		msleep(20);
		gpio_set_value(MSM_GPIO_BACKLIGHT_RST, 1);
		usleep(100);
		gpio_set_value(LCD_EN, 1);
		msleep(20);
#if defined(CONFIG_MSM_GPIO_BACKLIGHT_PWM)
		gpio_set_value(MSM_GPIO_BACKLIGHT_PWM, 1);
		msleep(20);
#endif

#else
		msleep(50);
		gpio_set_value(DISP_RST_GPIO, 0);
		usleep(5000);
		gpio_set_value(DISP_RST_GPIO, 1);
		msleep(50);
#endif
	} else {

#if defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL)
		msleep(120);
		pr_debug("%s: DISP_RST_GPIO low\n", __func__);
		gpio_set_value(DISP_RST_GPIO, 0);

#elif defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
		msleep(20);
		pr_debug("%s: LCD_EN_GPIO low\n", __func__);
		gpio_set_value(LCD_EN, 0);
		msleep(20);
		pr_debug("%s: MSM_GPIO_BACKLIGHT_RESET low\n", __func__);
		gpio_set_value(MSM_GPIO_BACKLIGHT_RST, 0);
		msleep(20);
#if defined(CONFIG_MSM_GPIO_BACKLIGHT_PWM)
		gpio_set_value(MSM_GPIO_BACKLIGHT_PWM, 0);
		msleep(20);
#endif
#else
		pr_debug("%s: DISP_RST_GPIO low\n", __func__);
		gpio_set_value(DISP_RST_GPIO, 0);
#endif

		rc = regulator_disable(reg_l8);
		if (rc) {
			pr_err("disable reg_l8 failed, rc=%d\n", rc);
			return -ENODEV;
		}
#ifdef CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT_PANEL
		usleep(5000);
#endif
		rc = regulator_disable(reg_l23);
		if (rc) {
			pr_err("disable reg_l23 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_disable(reg_l2);
		if (rc) {
			pr_err("disable reg_l2 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_set_optimum_mode(reg_l8, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l8 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_optimum_mode(reg_l23, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_set_optimum_mode(reg_l2, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l2 failed, rc=%d\n", rc);
			return -EINVAL;
		}
	}
	return 0;
}

static char mipi_dsi_splash_is_enabled(void);
static int mipi_dsi_panel_power(int on)
{
	pr_debug("%s: on=%d\n", __func__, on);

	return mipi_dsi_cdp_panel_power(on);
}

static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.vsync_gpio = MDP_VSYNC_GPIO,
	.dsi_power_save = mipi_dsi_panel_power,
	.splash_is_enabled = mipi_dsi_splash_is_enabled,
	.lcd_rst_down = pull_ldi_reset_down,
};

#ifdef CONFIG_MSM_BUS_SCALING

static struct msm_bus_vectors mdp_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
static struct msm_bus_vectors hdmi_as_primary_vectors[] = {
	/* If HDMI is used as primary */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 2000000000,
		.ib = 2000000000,
	},
};
static struct msm_bus_paths mdp_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(mdp_init_vectors),
		mdp_init_vectors,
	},
	{
		ARRAY_SIZE(hdmi_as_primary_vectors),
		hdmi_as_primary_vectors,
	},
	{
		ARRAY_SIZE(hdmi_as_primary_vectors),
		hdmi_as_primary_vectors,
	},
	{
		ARRAY_SIZE(hdmi_as_primary_vectors),
		hdmi_as_primary_vectors,
	},
	{
		ARRAY_SIZE(hdmi_as_primary_vectors),
		hdmi_as_primary_vectors,
	},
	{
		ARRAY_SIZE(hdmi_as_primary_vectors),
		hdmi_as_primary_vectors,
	},
};
#else
static struct msm_bus_vectors mdp_ui_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_vga_vectors[] = {
	/* VGA and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_720p_vectors[] = {
	/* 720p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 304128000 * 2,
		.ib = 380160000 * 2,
	},
};

static struct msm_bus_vectors mdp_1080p_vectors[] = {
	/* 1080p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 407808000 * 4,
		.ib = 509760000 * 4,
	},
};

static struct msm_bus_paths mdp_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(mdp_init_vectors),
		mdp_init_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_vga_vectors),
		mdp_vga_vectors,
	},
	{
		ARRAY_SIZE(mdp_720p_vectors),
		mdp_720p_vectors,
	},
	{
		ARRAY_SIZE(mdp_1080p_vectors),
		mdp_1080p_vectors,
	},
};
#endif

static struct msm_bus_scale_pdata mdp_bus_scale_pdata = {
	mdp_bus_scale_usecases,
	ARRAY_SIZE(mdp_bus_scale_usecases),
	.name = "mdp",
};

#endif

static struct msm_panel_common_pdata mdp_pdata = {
	.gpio = MDP_VSYNC_GPIO,
	.mdp_max_clk = 200000000,
#ifdef CONFIG_MSM_BUS_SCALING
	.mdp_bus_scale_table = &mdp_bus_scale_pdata,
#endif
	.mdp_rev = MDP_REV_43,
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	.mem_hid = BIT(ION_CP_MM_HEAP_ID),
#else
	.mem_hid = MEMTYPE_EBI1,
#endif
	.mdp_iommu_split_domain = 0,
	.cont_splash_enabled = 0x01,
	.splash_screen_addr = 0x00,
	.splash_screen_size = 0x00,
};

void __init msm8930_mdp_writeback(struct memtype_reserve* reserve_table)
{
	mdp_pdata.ov0_wb_size = MSM_FB_OVERLAY0_WRITEBACK_SIZE;
	mdp_pdata.ov1_wb_size = MSM_FB_OVERLAY1_WRITEBACK_SIZE;
#if defined(CONFIG_ANDROID_PMEM) && !defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov0_wb_size;
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov1_wb_size;
#endif
}
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)

static struct platform_device mipi_dsi_novatek_nt71391_panel_device = {
	.name = "mipi_novatek_nt71391",
	.id = 0,
	.dev.platform_data = &mipi_dsi_pdata,
};

#endif

#define LPM_CHANNEL0 0
static int toshiba_gpio[] = {LPM_CHANNEL0};

static struct mipi_dsi_panel_platform_data toshiba_pdata = {
	.gpio = toshiba_gpio,
};

static struct platform_device mipi_dsi_toshiba_panel_device = {
	.name = "mipi_toshiba",
	.id = 0,
	.dev = {
		.platform_data = &toshiba_pdata,
	}
};

#define FPGA_3D_GPIO_CONFIG_ADDR	0xB5

static char mipi_dsi_splash_is_enabled(void)
{
	return mdp_pdata.cont_splash_enabled;
}

static struct mipi_dsi_phy_ctrl dsi_novatek_cmd_mode_phy_db = {

/* DSI_BIT_CLK at 500MHz, 2 lane, RGB888 */
	{0x09, 0x08, 0x05, 0x00, 0x20},	/* regulator */
	/* timing   */
	{0xab, 0x8a, 0x18, 0x00, 0x92, 0x97, 0x1b, 0x8c,
	0x0c, 0x03, 0x04, 0xa0},
	{0x5f, 0x00, 0x00, 0x10},	/* phy ctrl */
	{0xff, 0x00, 0x06, 0x00},	/* strength */
	/* pll control */
	{0x0, 0xe, 0x30, 0xda, 0x00, 0x10, 0x0f, 0x61,
	0x40, 0x07, 0x03,
	0x00, 0x1a, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00, 0x02},
};

static struct mipi_dsi_panel_platform_data novatek_pdata = {
	.fpga_3d_config_addr  = FPGA_3D_GPIO_CONFIG_ADDR,
	.fpga_ctrl_mode = FPGA_SPI_INTF,
	.phy_ctrl_settings = &dsi_novatek_cmd_mode_phy_db,
	.dlane_swap = 0x1,
	.enable_wled_bl_ctrl = 0x1,
};

static struct platform_device mipi_dsi_novatek_panel_device = {
	.name = "mipi_novatek",
	.id = 0,
	.dev = {
		.platform_data = &novatek_pdata,
	}
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct resource hdmi_msm_resources[] = {
	{
		.name  = "hdmi_msm_qfprom_addr",
		.start = 0x00700000,
		.end   = 0x007060FF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_hdmi_addr",
		.start = 0x04A00000,
		.end   = 0x04A00FFF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_irq",
		.start = HDMI_IRQ,
		.end   = HDMI_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

static int hdmi_enable_5v(int on);
static int hdmi_core_power(int on, int show);
static int hdmi_cec_power(int on);

static struct msm_hdmi_platform_data hdmi_msm_data = {
	.irq = HDMI_IRQ,
	.enable_5v = hdmi_enable_5v,
	.core_power = hdmi_core_power,
	.cec_power = hdmi_cec_power,
};

static struct platform_device hdmi_msm_device = {
	.name = "hdmi_msm",
	.id = 0,
	.num_resources = ARRAY_SIZE(hdmi_msm_resources),
	.resource = hdmi_msm_resources,
	.dev.platform_data = &hdmi_msm_data,
};
#endif /* CONFIG_FB_MSM_HDMI_MSM_PANEL */

#ifdef CONFIG_FB_MSM_WRITEBACK_MSM_PANEL
static struct platform_device wfd_panel_device = {
	.name = "wfd_panel",
	.id = 0,
	.dev.platform_data = NULL,
};

static struct platform_device wfd_device = {
	.name          = "msm_wfd",
	.id            = -1,
};
#endif

#ifdef CONFIG_MSM_BUS_SCALING
static struct msm_bus_vectors dtv_bus_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
static struct msm_bus_vectors dtv_bus_def_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 2000000000,
		.ib = 2000000000,
	},
};
#else
static struct msm_bus_vectors dtv_bus_def_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 566092800 * 2,
		.ib = 707616000 * 2,
	},
};
#endif

static struct msm_bus_paths dtv_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(dtv_bus_init_vectors),
		dtv_bus_init_vectors,
	},
	{
		ARRAY_SIZE(dtv_bus_def_vectors),
		dtv_bus_def_vectors,
	},
};
static struct msm_bus_scale_pdata dtv_bus_scale_pdata = {
	dtv_bus_scale_usecases,
	ARRAY_SIZE(dtv_bus_scale_usecases),
	.name = "dtv",
};

static struct lcdc_platform_data dtv_pdata = {
	.bus_scale_table = &dtv_bus_scale_pdata,
};
#endif

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static int hdmi_enable_5v(int on)
{
	static struct regulator *reg_ext_5v;	/* HDMI_5V */
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (!reg_ext_5v) {
		reg_ext_5v = regulator_get(&hdmi_msm_device.dev, "hdmi_mvs");
		if (IS_ERR(reg_ext_5v)) {
			pr_err("'%s' regulator not found, rc=%ld\n",
				"hdmi_mvs", IS_ERR(reg_ext_5v));
			reg_ext_5v = NULL;
			return -ENODEV;
		}
	}

	if (on) {
		rc = regulator_enable(reg_ext_5v);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"reg_ext_5v", rc);
			return rc;
		}
		pr_debug("%s(on): success\n", __func__);
	} else {
		rc = regulator_disable(reg_ext_5v);
		if (rc)
			pr_warning("'%s' regulator disable failed, rc=%d\n",
				"reg_ext_5v", rc);
		pr_debug("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
}

static int hdmi_core_power(int on, int show)
{
	/* Both HDMI "avdd" and "vcc" are powered by 8038_l23 regulator */
	static struct regulator *reg_8038_l23;
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (!reg_8038_l23) {
		reg_8038_l23 = regulator_get(&hdmi_msm_device.dev, "hdmi_avdd");
		if (IS_ERR(reg_8038_l23)) {
			pr_err("could not get reg_8038_l23, rc = %ld\n",
				PTR_ERR(reg_8038_l23));
			return -ENODEV;
		}
		rc = regulator_set_voltage(reg_8038_l23, 1800000, 1800000);
		if (rc) {
			pr_err("set_voltage failed for 8921_l23, rc=%d\n", rc);
			return -EINVAL;
		}
	}

	if (on) {
		rc = regulator_set_optimum_mode(reg_8038_l23, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_enable(reg_8038_l23);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"hdmi_avdd", rc);
			return rc;
		}
		rc = gpio_request(100, "HDMI_DDC_CLK");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_CLK", 100, rc);
			goto error1;
		}
		rc = gpio_request(101, "HDMI_DDC_DATA");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_DATA", 101, rc);
			goto error2;
		}
		rc = gpio_request(102, "HDMI_HPD");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_HPD", 102, rc);
			goto error3;
		}
		pr_debug("%s(on): success\n", __func__);
	} else {
		gpio_free(100);
		gpio_free(101);
		gpio_free(102);

		rc = regulator_disable(reg_8038_l23);
		if (rc) {
			pr_err("disable reg_8038_l23 failed, rc=%d\n", rc);
			return -ENODEV;
		}
		rc = regulator_set_optimum_mode(reg_8038_l23, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l23 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		pr_debug("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;

error3:
	gpio_free(101);
error2:
	gpio_free(100);
error1:
	regulator_disable(reg_8038_l23);
	return rc;
}

static int hdmi_cec_power(int on)
{
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (on) {
		rc = gpio_request(99, "HDMI_CEC_VAR");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_CEC_VAR", 99, rc);
			goto error;
		}
		pr_debug("%s(on): success\n", __func__);
	} else {
		gpio_free(99);
		pr_debug("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
error:
	return rc;
}
#endif /* CONFIG_FB_MSM_HDMI_MSM_PANEL */

void __init msm8930_init_fb(void)
{
	platform_device_register(&msm_fb_device);

#ifdef CONFIG_FB_MSM_WRITEBACK_MSM_PANEL
	platform_device_register(&wfd_panel_device);
	platform_device_register(&wfd_device);
#endif

	platform_device_register(&mipi_dsi_novatek_panel_device);

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
	platform_device_register(&hdmi_msm_device);
#endif

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
	platform_device_register(&mipi_dsi_novatek_nt71391_panel_device);
#endif
	platform_device_register(&mipi_dsi_toshiba_panel_device);

	msm_fb_register_device("mdp", &mdp_pdata);
	msm_fb_register_device("mipi_dsi", &mipi_dsi_pdata);
#ifdef CONFIG_MSM_BUS_SCALING
	msm_fb_register_device("dtv", &dtv_pdata);
#endif
}

void __init msm8930_allocate_fb_region(void)
{
	void *addr;
	unsigned long size;

	size = MSM_FB_SIZE;
	addr = alloc_bootmem_align(size, 0x1000);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
			size, addr, __pa(addr));
}

void __init msm8930_set_display_params(char *prim_panel, char *ext_panel)
{
	if (strnlen(prim_panel, PANEL_NAME_MAX_LEN)) {
		strlcpy(msm_fb_pdata.prim_panel_name, prim_panel,
			PANEL_NAME_MAX_LEN);
		pr_debug("msm_fb_pdata.prim_panel_name %s\n",
			msm_fb_pdata.prim_panel_name);
	}
	if (strnlen(ext_panel, PANEL_NAME_MAX_LEN)) {
		strlcpy(msm_fb_pdata.ext_panel_name, ext_panel,
			PANEL_NAME_MAX_LEN);
		pr_debug("msm_fb_pdata.ext_panel_name %s\n",
			msm_fb_pdata.ext_panel_name);

		if (!strncmp((char *)msm_fb_pdata.ext_panel_name,
			MHL_PANEL_NAME, strnlen(MHL_PANEL_NAME,
				PANEL_NAME_MAX_LEN))) {
			pr_debug("MHL is external display by boot parameter\n");
			mhl_display_enabled = 1;
		}
	}

	hdmi_msm_data.is_mhl_enabled = mhl_display_enabled;
}
