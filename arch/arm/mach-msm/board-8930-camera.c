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

#include <asm/mach-types.h>
#include <linux/gpio.h>
#include <mach/camera.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include <mach/msm8930-gpio.h>
#include "devices.h"
#include "board-8930.h"
#include "devices-msm8x60.h"
#include <linux/ctype.h>
#include <linux/module.h>

#undef cam_err
#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[CAMERA][%s:%d] " fmt,		\
		       __func__, __LINE__, ##arg);	\
	}						\
	while (0)

#if (defined(CONFIG_GPIO_SX150X) || defined(CONFIG_GPIO_SX150X_MODULE)) && \
	defined(CONFIG_I2C)

static struct i2c_board_info cam_expander_i2c_info[] = {
	{
		I2C_BOARD_INFO("sx1508q", 0x22),
		.platform_data = &msm8930_sx150x_data[SX150X_CAM]
	},
};

static struct msm_cam_expander_info cam_expander_info[] = {
	{
		cam_expander_i2c_info,
		MSM_8930_GSBI4_QUP_I2C_BUS_ID,
	},
};
#endif

static struct gpiomux_setting cam_settings[] = {
	{
		.func = GPIOMUX_FUNC_GPIO, /*suspend*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 1*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 2*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 3*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_5, /*active 4*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_6, /*active 5*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_2, /*active 6*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_3, /*active 7*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*i2c suspend*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_KEEPER,
	},
	{
		.func = GPIOMUX_FUNC_2, /*active 9*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

};

#if (defined(CONFIG_MACH_EXPRESS) || defined(CONFIG_IMX175)\
	|| defined(CONFIG_MACH_INGRAHAM2))
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
	{
		.gpio = GPIO_FLASH_LED_UNLOCK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_MSM_FLASH_NOW,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#if defined(CONFIG_IMX175)
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_SUB_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM_CORE_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#else
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#endif
	{
		.gpio = GPIO_CAM2_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM1_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_VT_STBY,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	#ifndef CONFIG_IMX175
	{
		.gpio = GPIO_MAIN_STBY,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	#endif
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = GPIO_I2C_DATA_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
#ifdef CONFIG_IMX175
	{
		.gpio = GPIO_I2C_DATA_AF,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_AF,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
#endif
};
#else
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
	{
		.gpio = 2,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 3,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 4,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 5,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 76,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 107,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = 54,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = 18,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = 19,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = 20,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = 21,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
};
#endif

#ifdef CONFIG_MSM_CAMERA
#define VFE_CAMIF_TIMER1_GPIO 2
#define VFE_CAMIF_TIMER2_GPIO 3
#define VFE_CAMIF_TIMER3_GPIO_INT 4
static struct msm_camera_sensor_strobe_flash_data strobe_flash_xenon = {
	.flash_trigger = VFE_CAMIF_TIMER2_GPIO,
	.flash_charge = VFE_CAMIF_TIMER1_GPIO,
	.flash_charge_done = VFE_CAMIF_TIMER3_GPIO_INT,
	.flash_recharge_duration = 50000,
	.irq = MSM_GPIO_TO_INT(VFE_CAMIF_TIMER3_GPIO_INT),
};

#ifdef CONFIG_MSM_CAMERA_FLASH
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_EXT,
	._fsrc.ext_driver_src.led_en = GPIO_CAM_GP_LED_EN1,
	._fsrc.ext_driver_src.led_flash_en = GPIO_CAM_GP_LED_EN2,
#if defined(CONFIG_I2C) && (defined(CONFIG_GPIO_SX150X) || \
			defined(CONFIG_GPIO_SX150X_MODULE))
	._fsrc.ext_driver_src.expander_info = cam_expander_info,
#endif
};
#endif

#if defined(CONFIG_IMX175)
static struct msm_camera_sensor_flash_src imx175_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
	._fsrc.ext_driver_src.led_en = GPIO_FLASH_LED_UNLOCK,
	._fsrc.ext_driver_src.led_flash_en = GPIO_MSM_FLASH_NOW,
};
#endif

static struct msm_bus_vectors cam_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

/*Start : shchang@qualcomm.com : 1101*/
static struct msm_bus_vectors cam_preview_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 27648000,
		.ib  = 2656000000UL,	/*110592000,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};
/*End : shchang@qualcomm.com : 1101*/

static struct msm_bus_vectors cam_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 274406400,
		.ib  = 2656000000UL,	/*561807360,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_snapshot_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 600000000,	/*274423680,*/
		.ib  = 2656000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_zsl_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		/*.ab  = 302071680,*/
		.ab  = 600000000,
		.ib  = 2656000000UL, /*1208286720,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_video_ls_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 600000000,	/*348192000,*/
		.ib  = 4264000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_dual_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 302071680,
		.ib  = 2656000000UL,	/*1208286720,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_adv_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 274406400,
		.ib  = 2656000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};



static struct msm_bus_paths cam_bus_client_config[] = {
	{
		ARRAY_SIZE(cam_init_vectors),
		cam_init_vectors,
	},
	{
		ARRAY_SIZE(cam_preview_vectors),
		cam_preview_vectors,
	},
	{
		ARRAY_SIZE(cam_video_vectors),
		cam_video_vectors,
	},
	{
		ARRAY_SIZE(cam_snapshot_vectors),
		cam_snapshot_vectors,
	},
	{
		ARRAY_SIZE(cam_zsl_vectors),
		cam_zsl_vectors,
	},
	{
		ARRAY_SIZE(cam_video_ls_vectors),
		cam_video_ls_vectors,
	},
	{
		ARRAY_SIZE(cam_dual_vectors),
		cam_dual_vectors,
	},
	{
		ARRAY_SIZE(cam_adv_video_vectors),
		cam_adv_video_vectors,
	},

};
/*End : shchang@qualcomm.com : 1101*/

static struct msm_bus_scale_pdata cam_bus_client_pdata = {
		cam_bus_client_config,
		ARRAY_SIZE(cam_bus_client_config),
		.name = "msm_camera",
};

static struct msm_camera_device_platform_data msm_camera_csi_device_data[] = {
	{
		.csid_core = 0,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
		.cam_bus_scale_table = &cam_bus_client_pdata,
	},
	{
		.csid_core = 1,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
		.cam_bus_scale_table = &cam_bus_client_pdata,
	},
};



/*temp build error fix by DPI*/
static struct camera_vreg_t msm_8930_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};
/*temp build error fix by DPI*/

static struct camera_vreg_t msm_8930_back_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};

static struct camera_vreg_t msm_8930_front_cam_vreg[] = {
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
};

#if defined(CONFIG_MACH_EXPRESS) || defined(CONFIG_MACH_INGRAHAM2)
static struct gpio msm8930_common_cam_gpio[] = {
	{GPIO_I2C_DATA_CAM, GPIOF_DIR_IN, "CAMIF_I2C_DATA"},
	{GPIO_I2C_CLK_CAM, GPIOF_DIR_IN, "CAMIF_I2C_CLK"},
};

static struct gpio msm8930_front_cam_gpio[] = {
	{GPIO_MAIN_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
};

static struct gpio msm8930_back_cam_gpio[] = {
	{GPIO_MAIN_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
#if defined(TEMP_REMOVE)
	{GPIO_CAM1_RST_N, GPIOF_DIR_OUT, "CAM_RESET"},
	{GPIO_MAIN_STBY, GPIOF_DIR_OUT, "CAM_STBY_N"},
#endif
};

static struct msm_gpio_set_tbl msm8930_front_cam_gpio_set_tbl[] = {
#if defined(TEMP_REMOVE)
	{GPIO_CAM2_RST_N, GPIOF_OUT_INIT_LOW, 1000},
	{GPIO_CAM2_RST_N, GPIOF_OUT_INIT_HIGH, 4000},
#endif
};

static struct msm_gpio_set_tbl msm8930_back_cam_gpio_set_tbl[] = {
#if defined(TEMP_REMOVE)
	{GPIO_MAIN_STBY, GPIOF_OUT_INIT_LOW, 1000},
	{GPIO_MAIN_STBY, GPIOF_OUT_INIT_HIGH, 4000},
	{GPIO_CAM1_RST_N, GPIOF_OUT_INIT_LOW, 1000},
	{GPIO_CAM1_RST_N, GPIOF_OUT_INIT_HIGH, 4000},
#endif
};
#else
static struct gpio msm8930_common_cam_gpio[] = {
	{20, GPIOF_DIR_IN, "CAMIF_I2C_DATA"},
	{21, GPIOF_DIR_IN, "CAMIF_I2C_CLK"},
};

static struct gpio msm8930_front_cam_gpio[] = {
	{4, GPIOF_DIR_IN, "CAMIF_MCLK"},
	{76, GPIOF_DIR_OUT, "CAM_RESET"},
};

static struct gpio msm8930_back_cam_gpio[] = {
	{5, GPIOF_DIR_IN, "CAMIF_MCLK"},
	{107, GPIOF_DIR_OUT, "CAM_RESET"},
	{54, GPIOF_DIR_OUT, "CAM_STBY_N"},
};

static struct msm_gpio_set_tbl msm8930_front_cam_gpio_set_tbl[] = {
	{76, GPIOF_OUT_INIT_LOW, 1000},
	{76, GPIOF_OUT_INIT_HIGH, 4000},
};

static struct msm_gpio_set_tbl msm8930_back_cam_gpio_set_tbl[] = {
	{54, GPIOF_OUT_INIT_LOW, 1000},
	{54, GPIOF_OUT_INIT_HIGH, 4000},
	{107, GPIOF_OUT_INIT_LOW, 1000},
	{107, GPIOF_OUT_INIT_HIGH, 4000},
};
#endif

static struct msm_camera_gpio_conf msm_8930_front_cam_gpio_conf = {
	.cam_gpiomux_conf_tbl = msm8930_cam_2d_configs,
	.cam_gpiomux_conf_tbl_size = ARRAY_SIZE(msm8930_cam_2d_configs),
	.cam_gpio_common_tbl = msm8930_common_cam_gpio,
	.cam_gpio_common_tbl_size = ARRAY_SIZE(msm8930_common_cam_gpio),
	.cam_gpio_req_tbl = msm8930_front_cam_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(msm8930_front_cam_gpio),
	.cam_gpio_set_tbl = msm8930_front_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(msm8930_front_cam_gpio_set_tbl),
};

static struct msm_camera_gpio_conf msm_8930_back_cam_gpio_conf = {
	.cam_gpiomux_conf_tbl = msm8930_cam_2d_configs,
	.cam_gpiomux_conf_tbl_size = ARRAY_SIZE(msm8930_cam_2d_configs),
	.cam_gpio_common_tbl = msm8930_common_cam_gpio,
	.cam_gpio_common_tbl_size = ARRAY_SIZE(msm8930_common_cam_gpio),
	.cam_gpio_req_tbl = msm8930_back_cam_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(msm8930_back_cam_gpio),
	.cam_gpio_set_tbl = msm8930_back_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(msm8930_back_cam_gpio_set_tbl),
};

static struct i2c_board_info msm_act_main_cam_i2c_info = {
	I2C_BOARD_INFO("msm_actuator", 0x11),
};

static struct msm_actuator_info msm_act_main_cam_0_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_0,
	.bus_id         = MSM_8930_GSBI4_QUP_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};

static struct i2c_board_info msm_act_main_cam1_i2c_info = {
	I2C_BOARD_INFO("msm_actuator", 0x18),
};

static struct msm_actuator_info msm_act_main_cam_1_info = {
	.board_info     = &msm_act_main_cam1_i2c_info,
	.cam_name       = MSM_ACTUATOR_MAIN_CAM_1,
	.bus_id         = MSM_ACTUATOR_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};

#if defined(CONFIG_IMX175)
static struct camera_vreg_t msm_8930_imx175_vreg[] = {
	{"cam_vdig", REG_LDO, 1500000, 1500000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};

/*Start : shchang@qualcomm.com : 1104 -FROM*/
static struct msm_eeprom_info imx175_eeprom_info = {
	.type = MSM_EEPROM_SPI,
};
/*End : shchang@qualcomm.com : 1104 - FROM*/

static struct msm_camera_sensor_flash_data flash_imx175 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
	.flash_src	= &imx175_flash_src
};

static struct msm_camera_csi_lane_params imx175_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_platform_info sensor_board_info_imx175 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_imx175_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_imx175_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &imx175_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_imx175_data = {
	.sensor_name	= "imx175",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_imx175,
	.sensor_platform_info = &sensor_board_info_imx175,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_1_info,
/*Start : shchang@qualcomm.com : 1104 -FROM*/
	.eeprom_info = &imx175_eeprom_info,
/*End : shchang@qualcomm.com : 1104 - FROM*/
};
#endif
#ifdef CONFIG_S5K6A3YX
static struct msm_camera_sensor_flash_data flash_s5k6a3yx = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params s5k6a3yx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k6a3yx = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &s5k6a3yx_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k6a3yx_data = {
	.sensor_name	= "s5k6a3yx",
	.pdata	= &msm_camera_csi_device_data[1],
	.flash_data	= &flash_s5k6a3yx,
	.sensor_platform_info = &sensor_board_info_s5k6a3yx,
	.csi_if	= 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};
#endif

static struct msm_camera_sensor_flash_data flash_imx074 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
#ifdef CONFIG_MSM_CAMERA_FLASH
	.flash_src	= &msm_flash_src
#endif
};

static struct msm_camera_csi_lane_params imx074_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_platform_info sensor_board_info_imx074 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &imx074_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_imx074_data = {
	.sensor_name	= "imx074",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_imx074,
	.strobe_flash_data = &strobe_flash_xenon,
	.sensor_platform_info = &sensor_board_info_imx074,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_0_info,
};

static struct msm_camera_sensor_flash_data flash_mt9m114 = {
	.flash_type = MSM_CAMERA_FLASH_NONE
};

static struct msm_camera_csi_lane_params mt9m114_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_mt9m114 = {
	.mount_angle = 90,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &mt9m114_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9m114_data = {
	.sensor_name = "mt9m114",
	.pdata = &msm_camera_csi_device_data[1],
	.flash_data = &flash_mt9m114,
	.sensor_platform_info = &sensor_board_info_mt9m114,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};

static struct msm_camera_sensor_flash_data flash_ov2720 = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params ov2720_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_ov2720 = {
	.mount_angle	= 0,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &ov2720_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov2720_data = {
	.sensor_name	= "ov2720",
	.pdata	= &msm_camera_csi_device_data[1],
	.flash_data	= &flash_ov2720,
	.sensor_platform_info = &sensor_board_info_ov2720,
	.csi_if	= 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};

static struct msm_camera_sensor_flash_data flash_s5k3l1yx = {
	.flash_type = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params s5k3l1yx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k3l1yx = {
	.mount_angle  = 90,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &s5k3l1yx_csi_lane_params,
};

static struct msm_actuator_info msm_act_main_cam_2_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_2,
	.bus_id         = MSM_8930_GSBI4_QUP_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3l1yx_data = {
	.sensor_name          = "s5k3l1yx",
	.pdata                = &msm_camera_csi_device_data[0],
	.flash_data           = &flash_s5k3l1yx,
	.sensor_platform_info = &sensor_board_info_s5k3l1yx,
	.csi_if               = 1,
	.camera_type          = BACK_CAMERA_2D,
	.sensor_type          = BAYER_SENSOR,
	.actuator_info    = &msm_act_main_cam_2_info,
};

#ifdef CONFIG_ISX012
static struct msm_camera_sensor_flash_data flash_isx012 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
};

static struct msm_camera_csi_lane_params isx012_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_isx012 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &isx012_csi_lane_params,
	.sensor_reset	= GPIO_CAM1_RST_N,
	.sensor_stby	= GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset	= GPIO_CAM2_RST_N,
	.flash_en	= GPIO_MSM_FLASH_NOW,
	.flash_set	= GPIO_FLASH_LED_UNLOCK,
	.mclk	= GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 1,
	.sensor_power_on = cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
};

static struct msm_camera_sensor_info msm_camera_sensor_isx012_data = {
	.sensor_name	= "isx012",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_isx012,
	.sensor_platform_info = &sensor_board_info_isx012,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif

#ifdef CONFIG_SR130PC20
static struct msm_camera_sensor_flash_data flash_sr130pc20 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params sr130pc20_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_sr130pc20 = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &sr130pc20_csi_lane_params,
	.sensor_reset   = GPIO_CAM1_RST_N,
	.sensor_pwd     = GPIO_CAM_CORE_EN,
	.sensor_stby    = GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset        = GPIO_CAM2_RST_N,
	.mclk   = GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd        = 0,
	.vcm_enable     = 1,
	.sensor_power_on =  cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr130pc20_data = {
	.sensor_name    = "sr130pc20",
	.pdata  = &msm_camera_csi_device_data[1],
	.flash_data     = &flash_sr130pc20,
	.sensor_platform_info = &sensor_board_info_sr130pc20,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif

#ifdef CONFIG_SR030PC50
static struct msm_camera_sensor_flash_data flash_sr030pc50 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params sr030pc50_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_sr030pc50 = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &sr030pc50_csi_lane_params,
	.sensor_reset   = GPIO_CAM1_RST_N,
	.sensor_pwd     = GPIO_CAM_CORE_EN,
	.sensor_stby    = GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset        = GPIO_CAM2_RST_N,
	.mclk   = GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd        = 0,
	.vcm_enable     = 1,
	.sensor_power_on =  cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr030pc50_data = {
	.sensor_name    = "sr030pc50",
	.pdata  = &msm_camera_csi_device_data[1],
	.flash_data     = &flash_sr030pc50,
	.sensor_platform_info = &sensor_board_info_sr030pc50,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif


static struct platform_device msm_camera_server = {
	.name = "msm_cam_server",
	.id = 0,
};

static ssize_t back_camera_type_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_ISX012)
	char cam_type[] = "ISX012\n";
#elif defined(CONFIG_IMX175)
	char cam_type[] = "SONY_IMX175\n";
#else
	char cam_type[] = "Rear default camera\n";
#endif

	return snprintf(buf, sizeof(cam_type), "%s", cam_type);
}

static ssize_t front_camera_type_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_S5K6A3YX)
	char cam_type[] = "SLSI_S5K6A3YX\n";
#elif defined(CONFIG_SR130PC20)
	char cam_type[] = "SR130PC20\n";
#elif defined(CONFIG_SR030PC50)
	char cam_type[] = "SILICON_SR030PC50\n";
#else
	char cam_type[] = "Front default camera\n";
#endif

	return snprintf(buf, sizeof(cam_type), "%s", cam_type);
}

static DEVICE_ATTR(rear_camtype, S_IRUGO, back_camera_type_show, NULL);
static DEVICE_ATTR(front_camtype, S_IRUGO, front_camera_type_show, NULL);

static ssize_t back_camera_firmware_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_ISX012)
	char cam_fw[] = "ISX012\n";
#elif defined(CONFIG_IMX175)
	char cam_fw[] = "O08Q00AFL03 O08Q00AFL03\n";
#else
	char cam_fw[] = "Rear default camera\n";
#endif

	return snprintf(buf, sizeof(cam_fw), "%s", cam_fw);

}

static ssize_t front_camera_firmware_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_S5K6A3YX)
	char cam_fw[] = "S5K6A3YX S5K6A3YX\n";
#elif defined(CONFIG_SR130PC20)
	char cam_fw[] = "SR130PC20\n";
#elif defined(CONFIG_SR030PC50)
	char cam_fw[] = "SILICON_SR030PC50\n";
#else
	char cam_fw[] = "Front default camera\n";
#endif

	return snprintf(buf, sizeof(cam_fw), "%s", cam_fw);

}

static DEVICE_ATTR(rear_camfw, 0664, back_camera_firmware_show, NULL);
static DEVICE_ATTR(front_camfw, 0664, front_camera_firmware_show, NULL);
#if defined(CONFIG_ISX012)
u8 torchonoff;
static u8 gpio_flash_en;
static u8 gpio_flash_set;
static u8 pmic_gpio_msm_flash_cntl_en;
static bool isFlashCntlEn;
#endif

#if defined(CONFIG_IMX175)
bool isGodiva()
{
#ifdef CONFIG_MACH_GODIVA
	return true;
#else
	return false;
#endif
}

void gpio_set_ENM(bool bSet)
{
	if ((gpio_get_value_cansleep(97) == 0) &&
		(gpio_get_value_cansleep(93) == 0) &&
		(isGodiva()))
		gpio_set_value_cansleep(GPIO_FLASH_LED_UNLOCK, bSet);
	else
		gpio_set_value_cansleep(GPIO_MSM_FLASH_NOW, bSet);
}
#endif

static ssize_t cameraflash_file_cmd_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t size)
{
	int value;
	int i = 0;
	int err = 1;
	/*int flash_rev = 0;*/

	/*flash_rev = get_flash_led_unlock_rev();*/

	if (strlen(buf) > 2)
		return -err;

	if (isdigit(*buf)) {
		err = kstrtoint(buf, NULL, &value);
		if (err < 0)
			pr_err("%s, kstrtoint failed.", __func__);
	} else
		return -err;


#if defined(CONFIG_ISX012)
#if defined(CONFIG_MACH_EXPRESS) || defined(CONFIG_MACH_INGRAHAM2)
	/*if (flash_rev) {*/
		gpio_set_value_cansleep(GPIO_FLASH_LED_UNLOCK, value ? 1 : 0);
	/*}*/
#endif

#if defined(CONFIG_MACH_EXPRESS) || defined(CONFIG_MACH_INGRAHAM2)
	if (value == 0) {
		pr_err("[Torch flash]OFF\n");

		gpio_set_value_cansleep(gpio_flash_en, 0);

		gpio_set_value_cansleep(gpio_flash_set, 0);

		torchonoff = 0;

	} else {
		pr_err("[Torch flash]ON\n");
			gpio_set_value_cansleep(
				gpio_flash_en, 1);
			udelay(0);
			gpio_set_value_cansleep(
				gpio_flash_set, 1);
			udelay(1);

		torchonoff = 1;
	}

#else
	if (value == 0) {
		pr_err("[Torch flash]OFF\n");
		gpio_set_value_cansleep(gpio_flash_en, 0);
		gpio_set_value_cansleep(gpio_flash_set, 0);
		torchonoff = 0;
	} else {
		pr_err("[Torch flash]ON\n");
		gpio_set_value_cansleep(gpio_flash_en, 0);

		for (i = 5; i > 1; i--) {
			gpio_set_value_cansleep(
				gpio_flash_set, 1);
			udelay(1);
			gpio_set_value_cansleep(
				gpio_flash_set, 0);
			udelay(1);
		}
		gpio_set_value_cansleep(gpio_flash_set, 1);
		usleep(2*1000);
		torchonoff = 1;
	}
#endif
#elif defined(CONFIG_IMX175)
	cam_err("[value::%d]\n", value);
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	if (value == 0) {
		cam_err("[Torch flash]OFF\n");
		gpio_set_ENM(false);
	} else {
		cam_err("[Torch flash]ON\n");
		gpio_set_ENM(true);
	}
#endif

	return size;
}


static DEVICE_ATTR(rear_flash, S_IRUGO | S_IWUSR | S_IWGRP,
		NULL, cameraflash_file_cmd_store);

void msm8930_cam_create_node(void)
{
	struct device *cam_dev_back;
	struct device *cam_dev_front;
	struct device *cam_dev_flash;
	struct class *camera_class;

	camera_class = class_create(THIS_MODULE, "camera");

	if (IS_ERR(camera_class)) {
		pr_err("Failed to create class(camera)!\n");
		return;
	}

	cam_dev_flash = device_create(camera_class, NULL,
		0, NULL, "flash");
	if (IS_ERR(cam_dev_flash)) {
		pr_err("Failed to create cam_dev_flash device!\n");
		goto OUT8;
	}
	if (device_create_file(cam_dev_flash, &dev_attr_rear_flash) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_flash.attr.name);
		goto OUT7;
	}

	cam_dev_back = device_create(camera_class, NULL,
		1, NULL, "rear");
	if (IS_ERR(cam_dev_back)) {
		pr_err("Failed to create cam_dev_back device!\n");
		goto OUT6;
	}
	if (device_create_file(cam_dev_back, &dev_attr_rear_camtype) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_camtype.attr.name);
		goto OUT5;
	}
	if (device_create_file(cam_dev_back, &dev_attr_rear_camfw) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_camfw.attr.name);
		goto OUT4;
	}

	cam_dev_front = device_create(camera_class, NULL,
		2, NULL, "front");
	if (IS_ERR(cam_dev_front)) {
		pr_err("Failed to create cam_dev_front device!");
		goto OUT3;
	}

	if (device_create_file(cam_dev_front, &dev_attr_front_camtype) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_front_camtype.attr.name);
		goto OUT2;
	}
	if (device_create_file(cam_dev_front, &dev_attr_front_camfw) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_front_camfw.attr.name);
		goto OUT1;
	}

	return;

OUT1:
	printk(KERN_ERR "[CAMERA_DEV]OUT1\n");
	device_remove_file(cam_dev_back, &dev_attr_front_camtype);
OUT2:
	printk(KERN_ERR "[CAMERA_DEV]OUT2\n");
	device_destroy(camera_class, 2);
OUT3:
	printk(KERN_ERR "[CAMERA_DEV]OUT3\n");
	device_remove_file(cam_dev_back, &dev_attr_rear_camfw);
OUT4:
	printk(KERN_ERR "[CAMERA_DEV]OUT4\n");
	device_remove_file(cam_dev_back, &dev_attr_rear_camtype);
OUT5:
	printk(KERN_ERR "[CAMERA_DEV]OUT5\n");
	device_destroy(camera_class, 1);
OUT6:
	printk(KERN_ERR "[CAMERA_DEV]OUT6\n");
	device_remove_file(cam_dev_flash, &dev_attr_rear_flash);
OUT7:
	printk(KERN_ERR "[CAMERA_DEV]OUT7\n");
	device_destroy(camera_class, 0);
OUT8:
	printk(KERN_ERR "[CAMERA_DEV]OUT8\n");
	return;
}


void __init msm8930_init_cam(void)
{
	cam_err("Start\n");
#if (defined(CONFIG_MACH_EXPRESS) || defined(CONFIG_IMX175)\
	|| defined(CONFIG_MACH_INGRAHAM2))
	msm8930_cam_create_node();
#endif

	msm_gpiomux_install(msm8930_cam_common_configs,
			ARRAY_SIZE(msm8930_cam_common_configs));

	if (machine_is_msm8930_cdp()) {
		struct msm_camera_sensor_info *s_info;
		s_info = &msm_camera_sensor_s5k3l1yx_data;
		s_info->sensor_platform_info->mount_angle = 0;
	}

/************************************************/
/* EXTERNAL GPIO config

/************************************************/
	msm_camera_gpio_install();

#if defined(CONFIG_ISX012)
	struct msm_camera_sensor_info *s_info;
	s_info = &msm_camera_sensor_isx012_data;

	s_info->sensor_platform_info->flash_en =
		GPIO_MSM_FLASH_NOW;
	s_info->sensor_platform_info->flash_set =
		GPIO_FLASH_LED_UNLOCK;

	gpio_flash_en = s_info->sensor_platform_info->flash_en;
	gpio_flash_set = s_info->sensor_platform_info->flash_set;
#endif

	platform_device_register(&msm_camera_server);
	platform_device_register(&msm8960_device_csiphy0);
	platform_device_register(&msm8960_device_csiphy1);
	platform_device_register(&msm8960_device_csid0);
	platform_device_register(&msm8960_device_csid1);
	platform_device_register(&msm8960_device_ispif);
	platform_device_register(&msm8960_device_vfe);
	platform_device_register(&msm8960_device_vpe);
}

#ifdef CONFIG_I2C
struct i2c_board_info msm8930_camera_i2c_boardinfo[] = {
#if defined(CONFIG_IMX175)
	{
	I2C_BOARD_INFO("imx175", 0x1A),
	.platform_data = &msm_camera_sensor_imx175_data,
	},
#endif
#ifdef CONFIG_S5K6A3YX
	{
		I2C_BOARD_INFO("s5k6a3yx", 0x10),
		.platform_data = &msm_camera_sensor_s5k6a3yx_data,
	},
#endif
#ifdef CONFIG_SR130PC20
	{
		I2C_BOARD_INFO("sr130pc20", 0x20),
		.platform_data = &msm_camera_sensor_sr130pc20_data,
	},
#endif
#ifdef CONFIG_SR030PC50
	{
		I2C_BOARD_INFO("sr030pc50", 0x30),
		.platform_data = &msm_camera_sensor_sr030pc50_data,
	},
#endif
#ifdef CONFIG_ISX012
	{
		I2C_BOARD_INFO("isx012", 0x3D),
		.platform_data = &msm_camera_sensor_isx012_data,
	},
#endif
#if defined(TEMP_REMOVE)
	{
	I2C_BOARD_INFO("imx074", 0x1A),
	.platform_data = &msm_camera_sensor_imx074_data,
	},
	{
	I2C_BOARD_INFO("ov2720", 0x6C),
	.platform_data = &msm_camera_sensor_ov2720_data,
	},
	{
	I2C_BOARD_INFO("mt9m114", 0x48),
	.platform_data = &msm_camera_sensor_mt9m114_data,
	},
	{
	I2C_BOARD_INFO("s5k3l1yx", 0x20),
	.platform_data = &msm_camera_sensor_s5k3l1yx_data,
	},
#ifdef CONFIG_MSM_CAMERA_FLASH_SC628A
	{
	I2C_BOARD_INFO("sc628a", 0x6E),
	},
	{
	},
#endif
#endif
};

struct msm_camera_board_info msm8930_camera_board_info = {
	.board_info = msm8930_camera_i2c_boardinfo,
	.num_i2c_board_info = ARRAY_SIZE(msm8930_camera_i2c_boardinfo),
};
#endif
#endif
