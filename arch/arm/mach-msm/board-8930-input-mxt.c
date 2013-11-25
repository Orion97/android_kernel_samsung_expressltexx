/*
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/gpio_event.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <asm/mach-types.h>
#include <linux/regulator/consumer.h>
#include <mach/msm8930-gpio.h>
#include <linux/delay.h>
#include "board-8930.h"
#include "devices-msm8x60.h"

#ifdef CONFIG_TOUCHSCREEN_MXT224S
#include <linux/i2c/mxt224e.h>
#include <linux/i2c/mxt224s.h>
#endif

#define MSM_8960_GSBI3_QUP_I2C_BUS_ID	3

#ifdef CONFIG_TOUCHSCREEN_MXT224S
/* atmel mxt224s config value */
#define MXT224S_CHRGTIME_BATT		25
#define MXT224S_CHRGTIME_CHRG		60
#define MXT224S_THRESHOLD_BATT		0x1E
#define MXT224S_THRESHOLD_CHRG		0x28
#define MXT224S_CALCFG_BATT		210
#define MXT224S_CALCFG_CHRG		210

static u8 t7_config_s[] = { GEN_POWERCONFIG_T7,
	0x20, 0xFF, 0x32, 0x03
};


static u8 t8_config_s[] = { GEN_ACQUISITIONCONFIG_T8,
	0x1D, 0x00, 0x05, 0x01, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00
};

/*
static u8 t8_config_go_s[] = { GEN_ACQUISITIONCONFIG_T8,
	0x1D, 0x00, 0x05, 0x01, 0x00, 0x00, 0x04, 0x19, 0x00, 0x00
};
*/

static u8 t9_config_s[] = { TOUCH_MULTITOUCHSCREEN_T9,
	0x83, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x70, 0x32, 0x02, 0x01,
	MXT224S_MAX_MT_FINGERS,
	0x0A, 0x01, 0x3F, 0x0A, 0x0F, 0x1E, 0x0A, 0x1F, 0x03,
	0xDF, 0x01, 0x00, 0x0a, 0x14, 0x14, 0x80, 0x00, 0xc0, 0x00,
	0x14, 0x0F, 0x00, 0x00, 0x00, 0x00
};

static u8 t15_config_s[] = { TOUCH_KEYARRAY_T15,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0
};

static u8 t18_config_s[] = { SPT_COMCONFIG_T18,
	0, 0
};

static u8 t19_config_s[] = { SPT_GPIOPWM_T19,
	0, 0, 0, 0, 0, 0
};

static u8 t23_config_s[] = { TOUCH_PROXIMITY_T23,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};

static u8 t25_config_s[] = { SPT_SELFTEST_T25,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};

static u8 t40_config_s[] = { PROCI_GRIPSUPPRESSION_T40,
	0, 0, 0, 0, 0
};

static u8 t42_config_s[] = { PROCI_TOUCHSUPPRESSION_T42,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


static u8 t46_config_s[] = { SPT_CTECONFIG_T46,
	0x04, 0x00, 0x10, 0x1E, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01
};

static u8 t46_config_s_ta[] = { SPT_CTECONFIG_T46,
	0x04, 0x00, 0x10, 0x24, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01
};

static u8 t47_config_s[] = { PROCI_STYLUS_T47,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00
};

static u8 t55_config_s[] = { PROCI_ADAPTIVETHRESHOLD_T55,
	0, 50, 7, 10, 20, 1
};

static u8 t56_config_s[] = { PROCI_SHIELDLESS_T56,
	0x03, 0x00, 0x01, 0x35, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

static u8 t57_config_s[] = { PROCI_EXTRATOUCHSCREENDATA_T57,
	0xE3, 0x37, 0x00
};

static u8 t61_config_s[] = {SPT_TIMER_T61,
	0, 0, 0, 0, 0
};

static u8 t62_config_s[] = { PROCG_NOISESUPPRESSION_T62,
	0x03, 0x03, 0x00, 0x01, 0x08, 0x00, 0x10, 0x13, 0x02, 0x01,
	0x02, 0x00, 0x00, 0x01, 0x05, 0x00, 0x0A, 0x05, 0x05, 0x50,
	0x0C, 0x14, 0x30, 0x14, 0x3F, 0x18, 0x10, 0x10, 0x64, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x50, MXT224S_THRESHOLD_BATT,
	0x02, 0x05, 0x01, 0x30, MXT224S_MAX_MT_FINGERS,
	0x0F, 0x0F, 0xF0, 0xF2, 0xEC, 0xF0, 0x35, 0x28, 0x00,
	0x00, 0x12, 0x0A, 0x00
};

static u8 t62_config_s_ta[] = { PROCG_NOISESUPPRESSION_T62,
		0x03, 0x03, 0x00, 0x01, 0x08, 0x00, 0x10, 0x0A, 0x02, 0x00,
		0x01, 0x00, 0x01, 0x00, 0x05, 0x00, 0x0A, 0x05, 0x05, 0x50,
		0x0C, 0x14, 0x30, 0x14, 0x3F, 0x18, 0x10, 0x10, 0x64, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x50, MXT224S_THRESHOLD_CHRG,
		0x02, 0x05, 0x01, 0x30, MXT224S_MAX_MT_FINGERS,
		0x0F, 0x0F, 0xF0, 0xF2, 0xEC, 0xF0, 0x35, 0x28, 0x00,
		0x00, 0x12, 0x0A, 0x00
	};



static u8 end_config_s[] = { RESERVED_T255 };

static const u8 *mxt224s_config[] = {
	t7_config_s,
	t8_config_s,
	t9_config_s,
	t15_config_s,
	t18_config_s,
	t19_config_s,
	t23_config_s,
	t25_config_s,
	t40_config_s,
	t42_config_s,
	t46_config_s,
	t47_config_s,
	t55_config_s,
	t56_config_s,
	t57_config_s,
	t61_config_s,
	t62_config_s,
	end_config_s
};

void mxt224_register_callback(struct tsp_callbacks *cb)
{
	charger_callbacks = cb;
	pr_debug("[TSP]mxt224_register_callback\n");
}

static void mxt224_request_gpio(void)
{
	int ret;
	pr_info("[TSP] request gpio\n");

	ret = gpio_request(GPIO_TSP_D_EN, "mxt_tsp_d_en");
	if (ret) {
		pr_err("[TSP]%s: unable to request mxt_tsp_d_en [%d]\n",
				__func__, GPIO_TSP_D_EN);
	}
	ret = gpio_request(GPIO_TSP_A_EN, "mxt_tsp_a_en");
	if (ret) {
		pr_err("[TSP]%s: unable to request mxt_tsp_a_en [%d]\n",
				__func__, GPIO_TSP_A_EN);
	}
}

static void mxt224_power_onoff(int onoff)
{
	int ret = 0;

	static struct regulator *reg_l31;
	static struct regulator *reg_lvs6;

	pr_info("[TSP] power %s\n", onoff ? "on" : "off");

	if (system_rev > BOARD_REV02) {
		if (!reg_lvs6) {
			reg_lvs6 = regulator_get(NULL, "8917_lvs6");
			if (IS_ERR(reg_lvs6)) {
				pr_err("%s: could not get 8917_lvs6, rc = %ld\n",
					__func__, PTR_ERR(reg_lvs6));
				return;
			}
		}

		if (onoff) {
			if (regulator_is_enabled(reg_lvs6))
				printk(KERN_ERR
					"%s: rugulator LVS6(1.8V) is enabled\n",
						__func__);
			else
				ret = regulator_enable(reg_lvs6);
			
			if (ret) {
				pr_err("%s: enable lvs6 failed, rc=%d\n",
					__func__, ret);
				return;
			}

			pr_info("%s: tsp 1.8V on is finished.\n", __func__);

			 ret = gpio_request(GPIO_TOUCH_IRQ, "mxt_touch_irq");
		        if (ret) {
		                pr_err("[TSP]%s: unable to request mxt_touch_irq [%d]\n",
                                __func__, GPIO_TOUCH_IRQ);
		        }


		} else {
			if (regulator_is_enabled(reg_lvs6))
				ret = regulator_disable(reg_lvs6);
			else
				printk(KERN_ERR
					"%s: rugulator LVS6(1.8V) is disabled\n",
						__func__);

			if (ret) {
				pr_err("%s: disable lvs6 failed, rc=%d\n",
					__func__, ret);
				return;
			}
		
			pr_info("%s: tsp 1.8V off is finished.\n", __func__);
			gpio_free(GPIO_TOUCH_IRQ);
		}


		if (!reg_l31) {
			reg_l31 = regulator_get(NULL, "8917_l31");
			if (IS_ERR(reg_l31)) {
				pr_err("%s: could not get 8917_l31, rc = %ld\n",
					__func__, PTR_ERR(reg_l31));
				return;
			}
			ret = regulator_set_voltage(reg_l31, 3300000, 3300000);
			if (ret) {
				pr_err("%s: unable to set ldo31 voltage to 3.3V\n",
					__func__);
				return;
			}
		}

		if (onoff) {
			if (regulator_is_enabled(reg_l31))
				printk(KERN_ERR
					"%s: rugulator is(L31(3.3V) enabled\n",
						__func__);
			else
				ret = regulator_enable(reg_l31);

			if (ret) {
				pr_err("%s: enable l31 failed, rc=%d\n",
					__func__, ret);
				return;
			}
			pr_info("%s: tsp 3.3V on is finished.\n", __func__);
			
		} else {
			if (regulator_is_enabled(reg_l31))
				ret = regulator_disable(reg_l31);
			else
				printk(KERN_ERR
					"%s: rugulator is(L31(3.3V) disabled\n",
						__func__);
			if (ret) {
				pr_err("%s: disable l31 failed, rc=%d\n",
					__func__, ret);
				return;
			}
			pr_info("%s: tsp 3.3V off is finished.\n", __func__);
		}
		
	} else {
		ret = gpio_direction_output(GPIO_TSP_D_EN, onoff);
		if (ret) {
			pr_err("[TSP]%s: unable to set mxt_tsp_d_en [%d]\n",
					__func__, GPIO_TSP_D_EN);
		}

		ret = gpio_direction_output(GPIO_TSP_A_EN, onoff);
		if (ret) {
			pr_err("[TSP]%s: unable to set mxt_tsp_a_en [%d]\n",
					__func__, GPIO_TSP_A_EN);
		}
	}

	msleep(30);

	return;
}

struct mxt224s_platform_data mxt224s_data = {
	.max_finger_touches = MXT224S_MAX_MT_FINGERS,
	.gpio_read_done = GPIO_TOUCH_IRQ,
	.min_x = 0,
	.max_x = 479,
	.min_y = 0,
	.max_y = 799,
	.min_z = 0,
	.max_z = 255,
	.min_w = 0,
	.max_w = 30,
	.config = mxt224s_config,
	.config_s = mxt224s_config,
	.tchthr_batt = MXT224S_THRESHOLD_BATT,
	.tchthr_charging = MXT224S_THRESHOLD_CHRG,
	.t62_config_batt_s = t62_config_s,
	.t62_config_chrg_s = t62_config_s_ta,
	.t46_config_batt_s = t46_config_s,
	.t46_config_chrg_s = t46_config_s_ta,
	.request_gpio = mxt224_request_gpio,
	.power_onoff = mxt224_power_onoff,
	.config_fw_version = "I8730_AT_1226",
	.register_cb = mxt224_register_callback,
};

static struct i2c_board_info mxt224s_info[] __initdata = {
	{
		I2C_BOARD_INFO(MXT224S_DEV_NAME, 0x4a),
		.platform_data = &mxt224s_data,
		.irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ),
	},
};

void __init input_touchscreen_init(void)
{
	mxt224_request_gpio();
	mxt224_power_onoff(1);
	i2c_register_board_info(MSM_8960_GSBI3_QUP_I2C_BUS_ID,
				mxt224s_info,
				ARRAY_SIZE(mxt224s_info));

}
#endif
