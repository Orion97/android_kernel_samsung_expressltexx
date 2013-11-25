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
 */

#include <linux/mfd/pm8xxx/batterydata-lib.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, 0, 25, 40, 60},
	.y		= {2073, 2079, 2071, 2071, 2061},
	.cols	= 5
};

static struct single_row_lut fcc_sf = {
	.x		= {0},
	.y		= {100},
	.cols	= 1
};


static struct sf_lut rbatt_sf = {
	.rows		= 28,
	.cols			= 5,
	/* row_entries are temperature */
	.row_entries	= {-20, 0, 25, 40, 60},
	.percent		= {100, 95, 90, 85, 80, 75, 70, 65, 60,
		55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6,
		5, 4, 3, 2, 1},
	.sf			= {
		{741, 246, 100, 83, 78},
		{705, 246, 103, 85, 80},
		{679, 244, 105, 87, 82},
		{680, 239, 108, 89, 84},
		{647, 247, 114, 93, 85},
		{643, 224, 110, 93, 88},
		{645, 225, 116, 97, 90},
		{648, 228, 113, 97, 92},
		{659, 225, 100, 88, 87},
		{677, 224, 98, 83, 79},
		{702, 225, 98, 84, 81},
		{731, 225, 100, 85, 83},
		{766, 229, 102, 87, 86},
		{820, 238, 103, 88, 87},
		{902, 253, 106, 90, 83},
		{1006, 274, 108, 89, 82},
		{1142, 308, 108, 89, 84},
		{1339, 366, 103, 86, 81},
		{806, 274, 97, 82, 81},
		{904, 280, 98, 84, 83},
		{998, 291, 101, 86, 86},
		{1109, 306, 104, 89, 88},
		{1248, 319, 107, 92, 91},
		{1419, 333, 110, 94, 91},
		{1672, 350, 107, 88, 85},
		{2114, 384, 107, 89, 87},
		{4411, 837, 117, 97, 95},
		{77064, 61443, 188, 132, 125},
	}
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 29,
	.cols		= 5,
	.temp		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40,
		35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4327, 4325, 4321, 4316, 4309},
				{4228, 4250, 4252, 4250, 4246},
				{4161, 4193, 4196, 4195, 4191},
				{4098, 4140, 4143, 4141, 4138},
				{4051, 4086, 4093, 4090, 4087},
				{3960, 4038, 4050, 4044, 4039},
				{3913, 3958, 3989, 3994, 3996},
				{3876, 3920, 3957, 3959, 3957},
				{3842, 3891, 3916, 3919, 3919},
				{3816, 3860, 3868, 3872, 3876},
				{3796, 3832, 3838, 3838, 3838},
				{3781, 3807, 3815, 3815, 3814},
				{3767, 3786, 3797, 3796, 3796},
				{3754, 3770, 3782, 3780, 3780},
				{3741, 3757, 3771, 3768, 3763},
				{3725, 3743, 3760, 3754, 3741},
				{3706, 3728, 3740, 3732, 3719},
				{3680, 3714, 3708, 3699, 3688},
				{3642, 3696, 3686, 3679, 3668},
				{3630, 3691, 3685, 3678, 3667},
				{3616, 3686, 3684, 3677, 3666},
				{3598, 3678, 3681, 3675, 3664},
				{3576, 3663, 3676, 3669, 3655},
				{3547, 3636, 3656, 3647, 3627},
				{3506, 3591, 3613, 3601, 3579},
				{3446, 3522, 3550, 3538, 3517},
				{3350, 3418, 3465, 3456, 3435},
				{3218, 3252, 3338, 3328, 3301},
				{3000, 3000, 3000, 3000, 3000}
	}
};


struct bms_battery_data Samsung_8930_Express_2000mAh_data = {
	.fcc				= 2000,
	.fcc_temp_lut		= &fcc_temp,
	.fcc_sf_lut			= &fcc_sf,
	.pc_temp_ocv_lut	= &pc_temp_ocv,
	.rbatt_sf_lut		= &rbatt_sf,
	.default_rbatt_mohm		= 166,
};

static struct single_row_lut palladium_1500_fcc_temp = {
	.x	= {-30, -20, -10, 0, 10, 25, 40, 60},
	.y	= {1103, 1179, 1284, 1330, 1420, 1511, 1541, 1571},
	.cols	= 8,
};

static struct single_row_lut palladium_1500_fcc_sf = {
	.x	= {100, 200, 300, 400, 500},
	.y	= {97, 93, 93, 90, 87},
	.cols	= 5,
};

static struct sf_lut palladium_1500_pc_sf = {
	.rows		= 10,
	.cols		= 5,
	/* row_entries are chargecycles */
	.row_entries	= {100, 200, 300, 400, 500},
	.percent	= {100, 90, 80, 70, 60, 50, 40, 30, 20, 10},
	.sf		= {
			{97, 93, 93, 90, 87},
			{97, 93, 93, 90, 87},
			{98, 94, 92, 89, 86},
			{98, 94, 92, 89, 86},
			{99, 94, 92, 88, 86},
			{99, 95, 92, 88, 87},
			{99, 95, 92, 88, 87},
			{99, 95, 92, 88, 87},
			{99, 95, 92, 88, 87},
			{99, 95, 92, 88, 87}
	},
};

static struct sf_lut palladium_1500_rbatt_sf = {
	.rows		= 19,
	.cols		= 5,
	/* row_entries are temperature */
	.row_entries	= {-20, 0, 20, 40, 65},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50,
				45, 40, 35, 30, 25, 20, 15, 10
	},
	.sf		= {
					{645, 301, 100, 80, 69},
					{616, 290, 100, 79, 69},
					{586, 279, 100, 78, 68},
					{564, 270, 100, 78, 68},
					{546, 262, 100, 78, 68},
					{537, 256, 100, 79, 68},
					{536, 253, 100, 79, 69},
					{552, 258, 100, 81, 71},
					{618, 284, 100, 80, 72},
					{643, 290, 100, 77, 68},
					{673, 294, 100, 77, 68},
					{720, 296, 100, 77, 69},
					{769, 294, 100, 76, 68},
					{821, 288, 100, 74, 67},
					{892, 284, 100, 74, 61},
					{1003, 290, 100, 71, 58},
					{1192, 307, 100, 70, 58},
					{1579, 345, 100, 68, 57},
					{1261, 324, 100, 68, 57},
	}
};
static struct pc_temp_ocv_lut palladium_1500_pc_temp_ocv = {
	.rows		= 29,
	.cols		= 8,
	.temp		= {-30, -20, -10, 0, 10, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55,
				50, 45, 40, 35, 30, 25, 20, 15, 10, 9,
				8, 7, 6, 5, 4, 3, 2, 1, 0
	},
	.ocv		= {
			{3673, 3814, 3945, 4025, 4106, 4176, 4218, 4260},
			{3613, 3751, 3880, 3959, 4038, 4107, 4149, 4190},
			{3573, 3710, 3837, 3916, 3994, 4062, 4103, 4144},
			{3534, 3670, 3796, 3873, 3951, 4019, 4059, 4099},
			{3491, 3625, 3749, 3826, 3902, 3969, 4009, 4049},
			{3464, 3597, 3721, 3796, 3872, 3939, 3978, 4018},
			{3436, 3568, 3691, 3766, 3841, 3907, 3946, 3985},
			{3407, 3537, 3659, 3733, 3808, 3873, 3912, 3951},
			{3377, 3507, 3627, 3701, 3775, 3840, 3878, 3917},
			{3355, 3484, 3604, 3677, 3751, 3815, 3853, 3891},
			{3339, 3467, 3586, 3659, 3732, 3796, 3834, 3872},
			{3324, 3452, 3570, 3643, 3716, 3780, 3818, 3855},
			{3312, 3440, 3558, 3630, 3703, 3766, 3804, 3842},
			{3303, 3430, 3548, 3620, 3692, 3756, 3793, 3831},
			{3297, 3424, 3541, 3614, 3686, 3749, 3787, 3824},
			{3288, 3414, 3531, 3603, 3675, 3738, 3776, 3813},
			{3272, 3398, 3514, 3586, 3658, 3720, 3757, 3795},
			{3240, 3365, 3480, 3551, 3622, 3684, 3721, 3758},
			{3224, 3348, 3463, 3533, 3604, 3666, 3702, 3739},
			{3221, 3344, 3459, 3530, 3600, 3662, 3695, 3728},
			{3216, 3340, 3454, 3525, 3595, 3657, 3686, 3715},
			{3212, 3335, 3449, 3520, 3590, 3652, 3677, 3703},
			{3203, 3326, 3440, 3510, 3580, 3642, 3664, 3686},
			{3185, 3307, 3420, 3490, 3560, 3621, 3639, 3657},
			{3176, 3298, 3411, 3481, 3550, 3611, 3626, 3640},
			{3151, 3272, 3384, 3453, 3522, 3583, 3593, 3604},
			{3106, 3225, 3335, 3446, 3472, 3531, 3538, 3545},
			{3021, 3217, 3245, 3417, 3429, 3435, 3439, 3442},
			{3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000}
	},
};

struct bms_battery_data palladium_1500_data = {
	.fcc			= 1500,
	.fcc_temp_lut		= &palladium_1500_fcc_temp,
	.fcc_sf_lut		= &palladium_1500_fcc_sf,
	.pc_temp_ocv_lut	= &palladium_1500_pc_temp_ocv,
	.pc_sf_lut		= &palladium_1500_pc_sf,
	.rbatt_sf_lut		= &palladium_1500_rbatt_sf,
	.default_rbatt_mohm	= 254,
	.delta_rbatt_mohm	= 60,
};
