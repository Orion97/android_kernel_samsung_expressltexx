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

#include <mach/gpio.h>
#include "lcd_backlight_ic.h"
#include <linux/delay.h>

static DEFINE_SPINLOCK(bl_ctrl_lock);
static int lcd_brightness = -1;

#define MAX_BRT_STAGE_AAT (int)(sizeof(brt_table_aat)/sizeof(struct brt_value))

struct brt_value brt_table_aat[] = {
		{ 255, 7 }, /* Max */
		{ 244, 8 },
		{ 233, 9 },
		{ 222, 10 },
		{ 211, 11 },
		{ 200, 12 },
		{ 189, 13 },
		{ 178, 14 },
		{ 166, 15 },
		{ 154, 17 },
		{ 141, 19 }, /* default */
		{ 127, 21 },
		{ 113, 23 },
		{ 99,  24 },
		{ 85,  26 },
		{ 71,  28 },
		{ 57,  29 },
		{ 43,  30 },
		{ 30,  31 }, /* Min */
		{ 20,  31 }, /* Dimming */
		{ 0,   32 }, /* Off */
};


#define MAX_BRIGHTNESS_VALUE	255
#define MIN_BRIGHTNESS_VALUE	31
#define AAT_DIMMING_VALUE	    31
#define MAX_BRIGHTNESS_IN_BLU	32


void ktd253_set_brightness(int level)
{
	int pulse;
	int tune_level = 0;
	int i;

	spin_lock(&bl_ctrl_lock);
	if (level > 0) {
		if (level < MIN_BRIGHTNESS_VALUE) {
			tune_level = AAT_DIMMING_VALUE; /* DIMMING */
		} else {
			for (i = 0; i < MAX_BRT_STAGE_AAT - 1; i++) {
				if (level <= brt_table_aat[i].level
					&& level > brt_table_aat[i+1].level) {
					tune_level =  \
					brt_table_aat[i].tune_level;
					break;
				}
			}
		}
	} /*  BACKLIGHT is KTD model */



	if (!tune_level) {
		gpio_set_value(GPIO_BL_CTRL, 0);
		mdelay(3);
		lcd_brightness = tune_level;
	} else {
		if (unlikely(lcd_brightness < 0)) {
			int val = gpio_get_value(GPIO_BL_CTRL);
			if (val) {
				lcd_brightness = 0;
				gpio_set_value(GPIO_BL_CTRL, 0);
				mdelay(3);
				printk(KERN_INFO "backlight boot init\n");
			}
		}


		if (!lcd_brightness) {
			gpio_set_value(GPIO_BL_CTRL, 1);
			udelay(3);
			lcd_brightness = MAX_BRIGHTNESS_IN_BLU;
		}

		pulse = (tune_level - lcd_brightness + MAX_BRIGHTNESS_IN_BLU)
						% MAX_BRIGHTNESS_IN_BLU;

		for (; pulse > 0; pulse--) {
			gpio_set_value(GPIO_BL_CTRL, 0);
			udelay(3);
			gpio_set_value(GPIO_BL_CTRL, 1);
			udelay(3);
		}

		lcd_brightness = tune_level;
	}
	mdelay(1);
	spin_unlock(&bl_ctrl_lock);
}
