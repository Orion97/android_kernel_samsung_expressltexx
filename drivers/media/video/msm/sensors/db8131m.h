/*
 * Copyright (c) 2008 QUALCOMM USA, INC.
 * Author: Haibo Jeff Zhong <hzhong@qualcomm.com>
 *
 * All source code in this file is licensed under the following license
 * except where indicated.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can find it at http://www.fsf.org
 *
 */


#ifndef DB8131M_H
#define DB8131M_H

#include "msm_sensor.h"
#define SENSOR_NAME "db8131m"
#define PLATFORM_DRIVER_NAME "msm_camera_db8131m"
#define db8131m_obj db8131m_##obj

#undef CONFIG_LOAD_FILE
/*#define CONFIG_LOAD_FILE*/

#undef DEBUG_LEVEL_HIGH
#undef DEBUG_LEVEL_MID
#define DEBUG_LEVEL_HIGH
/*#define DEBUG_LEVEL_MID */

#if defined(DEBUG_LEVEL_HIGH)
#define CAM_DEBUG(fmt, arg...)	\
	do {					\
		printk(KERN_DEBUG "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

#define cam_info(fmt, arg...)			\
	do {					\
		printk(KERN_INFO "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)
#elif defined(DEBUG_LEVEL_MID)
#define CAM_DEBUG(fmt, arg...)
#define cam_info(fmt, arg...)			\
	do {					\
		printk(KERN_INFO "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)
#else
#define CAM_DEBUG(fmt, arg...)
#define cam_info(fmt, arg...)
#endif

#undef DEBUG_CAM_I2C
#define DEBUG_CAM_I2C

#if defined(DEBUG_CAM_I2C)
#define cam_i2c_dbg(fmt, arg...)		\
	do {					\
		printk(KERN_DEBUG "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)
#else
#define cam_i2c_dbg(fmt, arg...)
#endif


#define cam_err(fmt, arg...)	\
	do {					\
		printk(KERN_ERR "[%s:%d] " fmt,		\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

#define DB8131M_DELAY		0xE700

#define DB8_WRT_LIST(A)		\
		db8131m_i2c_write_list(A, (sizeof(A) / sizeof(A[0])), #A);

struct db8131m_userset {
	unsigned int focus_mode;
	unsigned int focus_status;
	unsigned int continuous_af;

	unsigned int	metering;
	unsigned int	exposure;
	unsigned int		wb;
	unsigned int		iso;
	int	contrast;
	int	saturation;
	int	sharpness;
	int	brightness;
	int	scene;
	unsigned int zoom;
	unsigned int effect;	/* Color FX (AKA Color tone) */
	unsigned int scenemode;
	unsigned int detectmode;
	unsigned int antishake;
	unsigned int fps;
	unsigned int flash_mode;
	unsigned int flash_state;
	unsigned int stabilize;	/* IS */
	unsigned int strobe;
	unsigned int jpeg_quality;
/*unsigned int preview_size;*/
/*struct m5mo_preview_size preview_size;*/
	unsigned int preview_size_idx;
	unsigned int capture_size;
	unsigned int thumbnail_size;
};

enum db8131m_setting {
	RES_PREVIEW,
	RES_CAPTURE
};

enum db8131m_reg_update {
	/* Sensor egisters that need to be updated during initialization */
	REG_INIT,
	/* Sensor egisters that needs periodic I2C writes */
	UPDATE_PERIODIC,
	/* All the sensor Registers will be updated */
	UPDATE_ALL,
	/* Not valid update */
	UPDATE_INVALID
};

struct db8131m_work {
	struct work_struct work;
};

static struct  i2c_client *db8131m_client;
static struct msm_sensor_ctrl_t db8131m_s_ctrl;
static struct device db8131m_dev;

struct db8131m_ctrl {
	const struct msm_camera_sensor_info *sensordata;
	struct db8131m_userset settings;
	struct msm_camera_i2c_client *sensor_i2c_client;
	struct msm_sensor_ctrl_t *s_ctrl;
	struct v4l2_subdev *sensor_dev;
	struct v4l2_subdev sensor_v4l2_subdev;
	struct v4l2_subdev_info *sensor_v4l2_subdev_info;
	uint8_t sensor_v4l2_subdev_info_size;
	struct v4l2_subdev_ops *sensor_v4l2_subdev_ops;

	int op_mode;
	int dtp_mode;
	int cam_mode;
	int vtcall_mode;
	int started;
	int dtpTest;
	int isCapture;
	int mirror_mode;
};

static unsigned int config_csi2;
static struct db8131m_ctrl *db8131m_ctrl;

struct db8131m_format {
	enum v4l2_mbus_pixelcode code;
	enum v4l2_colorspace colorspace;
	u16 fmt;
	u16 order;
};

static struct db8131m_format db8131m_subdev_info[] = {
	{
	.code   = V4L2_MBUS_FMT_YUYV8_2X8,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt    = 1,
	.order    = 0,
	},
	/* more can be supported, to be added later */
};

/* preview size idx*/
enum db8131m_preview_size_t {
	PREVIEW_SIZE_VGA = 0,	/* 640x480*/
	PREVIEW_SIZE_D1,		/* 720x480*/
	PREVIEW_SIZE_WVGA,	/* 800x480*/
	PREVIEW_SIZE_XGA,		/* 1024x768*/
	PREVIEW_SIZE_HD,		/* 1280x720*/
	PREVIEW_SIZE_FHD,		/* 1920x1080*/
	PREVIEW_SIZE_MMS,		/*176x144*/
};

enum db8131m_camera_mode_t {
	PREVIEW_MODE = 0,
	MOVIE_MODE,
};

static struct db8131m_exif_data
{
	unsigned short iso;
	unsigned short shutterspeed;
};

static struct db8131m_exif_data *db8131m_exif;

static int db8131m_sensor_config(void __user *argp);
static void db8131m_set_flip(int flip);
static DECLARE_WAIT_QUEUE_HEAD(db8131m_wait_queue);

DEFINE_MUTEX(db8131m_mut);

#ifdef CONFIG_LOAD_FILE
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct test {
	u8 data;
	struct test *nextBuf;
};
static struct test *testBuf;
static s32 large_file;

static char *db8131m_regs_table;
static int db8131m_regs_table_size;
static int db8131m_write_regs_from_sd(char *name);
static int db8131m_i2c_write_multi(unsigned short addr, unsigned int w_data);
#endif

#endif /* DB8131M_H */

