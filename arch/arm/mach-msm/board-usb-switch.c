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
#include <asm/mach-types.h>
#include <linux/regulator/consumer.h>
#include <mach/msm8930-gpio.h>
#include "board-8960.h"

void usb_switch_vcc_on(bool onoff)
{
	int ret = 0;
	/* 2.8V */
	static struct regulator *reg_l17;

	if (!reg_l17) {
		reg_l17 = regulator_get(NULL, "8038_l17");
		if (IS_ERR(reg_l17)) {
			pr_err("could not get 8038_l17, rc = %ld\n",
					PTR_ERR(reg_l17));
			return;
		}
		ret = regulator_set_voltage(reg_l17, 2950000, 2950000);
		if (ret) {
			pr_err("%s: not to set L17 to 2.8V\n",
					__func__);
			return;
		}
	}

	if (onoff) {
		ret = regulator_enable(reg_l17);
		if (ret) {
			pr_err("enable l17 failed, rc=%d\n", ret);
			return;
		}
		pr_info("%s: usb_switch 2.8V on is finished.\n",
					__func__);
	} else {
		if (regulator_is_enabled(reg_l17))
			ret = regulator_disable(reg_l17);

		if (ret) {
			pr_err("disable l17 failed, rc=%d\n", ret);
			return;
		}
		pr_info("%s: usb_switch 3.0V off is finished.\n",
					__func__);
	}
}

void __init usb_switch_init(void)
{
#ifdef CONFIG_ADC_STMPE811
	int ret = 0;
	printk(KERN_ERR "%s: stmpe811 gpio_config\n", __func__);

	gpio_tlmm_config(GPIO_CFG(GPIO_ADC_INT, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 0);

	gpio_tlmm_config(GPIO_CFG(GPIO_ADC_SDA, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_ADC_SCL, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
#endif
	gpio_tlmm_config(GPIO_CFG(GPIO_USB_SEL, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 0);
	gpio_set_value(GPIO_USB_SEL, 0);

	gpio_tlmm_config(GPIO_CFG(GPIO_IF_LDO_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 0);
	gpio_set_value(GPIO_IF_LDO_EN, 1);

	usb_switch_vcc_on(1);
}
