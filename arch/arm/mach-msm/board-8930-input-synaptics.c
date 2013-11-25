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
#ifdef CONFIG_TOUCHSCREEN_S7301
#include <linux/platform_data/synaptics_s7301.h>
#endif

#define MSM_8960_GSBI3_QUP_I2C_BUS_ID	3

static void synaptics_request_gpio(void)
{
	int ret;
	pr_info("[TSP] request gpio\n");

	ret = gpio_request(GPIO_TOUCH_IRQ, "tsp_int");
	if (ret != 0) {
		pr_err("tsp int request failed, ret=%d", ret);
		return;
	}
	ret = gpio_request(GPIO_TSP_LDO_ON,
						"GPIO_TSP_LDO_ON");
	if (ret != 0) {
		pr_err("tsp int request failed, ret=%d", ret);
		return;
	}
}

static void synaptics_power_onoff(int onoff)
{
	int ret;

	pr_info("[TSP] power %s\n", onoff ? "on" : "off");

	ret = gpio_direction_output(GPIO_TSP_LDO_ON, onoff);

	if (ret) {
		pr_err("[TSP]%s: unable to set_direction for mxt_tsp_a_en [%d]\n",
				__func__, GPIO_TSP_LDO_ON);
		goto err_irq_gpio_tsp_ldo_on;
	}
	msleep(30);

	return;

err_irq_gpio_tsp_ldo_on:
	gpio_free(GPIO_TSP_LDO_ON);
}

static struct synaptics_platform_data synaptics_ts_pdata = {
	.model_name = "NOTE_8",
	.rx_channel_no = 41,    /* Y channel line */
	.tx_channel_no = 26,    /* X channel line */
	.x_pixel_size = 1279,
	.y_pixel_size = 799,
	.ta_state = CABLE_TYPE_NONE,
	.set_power = synaptics_power_onoff,
};

static struct i2c_board_info __initdata synaptics_ts_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("synaptics-ts", 0x20),
		.platform_data  = &synaptics_ts_pdata,
		.irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ),
	},
};

void __init input_touchscreen_init(void)
{

	synaptics_request_gpio();
	synaptics_power_onoff(1);

	i2c_register_board_info(MSM_8960_GSBI3_QUP_I2C_BUS_ID,
			synaptics_ts_i2c_boardinfo,
			ARRAY_SIZE(synaptics_ts_i2c_boardinfo));
}
