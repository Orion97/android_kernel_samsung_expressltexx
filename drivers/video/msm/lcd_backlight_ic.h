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

#define GPIO_BL_CTRL 34

struct brt_value {
int level;		/* Platform setting values*/
int tune_level;		 /* Chip Setting values*/
};
#define MAX_BRIGHTNESS_VALUE	255
#define MIN_BRIGHTNESS_VALUE	31
#define AAT_DIMMING_VALUE	    31
#define MAX_BRIGHTNESS_IN_BLU	32

void ktd253_set_brightness(int level);
