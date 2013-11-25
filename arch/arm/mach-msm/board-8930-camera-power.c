
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
#include <mach/board.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include <mach/msm8930-gpio.h>
#include "devices.h"
#include "board-8930.h"
#include <mach/socinfo.h>

#undef CAM_DEBUG
#if defined(CAM_DEBUG_HIGH)
#define CAM_DEBUG(fmt, arg...)	\
	do {					\
		printk(KERN_DEBUG "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)
#else
#define CAM_DEBUG(fmt, arg...)
#endif

#undef cam_err
#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[%s:%d] " fmt,		\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

#if defined(CONFIG_ISX012)
void msm_camera_gpio_install(void)
{
	CAM_DEBUG("Camera GPIO install!!\n");

	/*Flash Set : GPIO 3*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);

	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*GPIO_CAM_CORE_EN  : GPIO 6*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Front cam stby : GPIO 18*/
	gpio_tlmm_config(GPIO_CFG(GPIO_VT_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam stby : GPIO 54*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);
	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	CAM_DEBUG("Finish!!\n");
}

static struct regulator *l11, *l29, *l32, *l34, *l12, *l30;
#define MAX_CAM_POWER_PIN 5
int error_gpio_buf[MAX_CAM_POWER_PIN];
int error_gpio_cnt;

char cam_power_pin[MAX_CAM_POWER_PIN][32] = {
	"GPIO_CAM_CORE_EN",
	"GPIO_CAM_IO_EN",
	"GPIO_CAM_A_EN",
	"8038_l12",
	"8038_l8",
};

void search_error_pin(char *name)
{
	int i = 0;

	for (i = 0; i < MAX_CAM_POWER_PIN; i++) {
		if (!strncmp(cam_power_pin[i], name, strnlen(name, 50))) {
			error_gpio_buf[error_gpio_cnt++] = i;
			break;
		}
	}
	if (i == MAX_CAM_POWER_PIN)
		cam_err("Can't find error pin : %s", name);
}
void cam_ldo_power_on(int mode)
{
	int ret = 0;
	int power_on_cnt = 0;
	int i = 0;

	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
	ret = gpio_get_value(GPIO_CAM_CORE_EN);
	if (ret) {
		CAM_DEBUG("check CAM_CORE_EN : %d\n", ret);
		power_on_cnt++;
	} else
		search_error_pin("GPIO_CAM_CORE_EN");

	udelay(10);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		l34 = regulator_get(NULL, "8917_l34");
		ret = regulator_set_voltage(l34, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l34);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l34");
		} else
			power_on_cnt++;

		udelay(1000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		l32 = regulator_get(NULL, "8917_l32");
		ret = regulator_set_voltage(l32, 2800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l32);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l32");
		} else
			power_on_cnt++;

		udelay(1000);

/*VT core 1.8V - CAM_DVDD_1P8V*/

#if defined(CONFIG_SR130PC20)
		l29 = regulator_get(NULL, "8921_l29");
		ret = regulator_set_voltage(l29, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l29);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l29");
		} else
			power_on_cnt++;
#else/*CONFIG_SR030PC50*/
		l30 = regulator_get(NULL, "8917_l30");
		ret = regulator_set_voltage(l30, 1800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l30);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l30");
		} else
			power_on_cnt++;
#endif
		udelay(10);


/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
		l11 = regulator_get(NULL, "8917_l11");
		ret = regulator_set_voltage(l11, 2800000, 2850000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l11);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l11");
		} else
			power_on_cnt++;
	}
		udelay(200);

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}

void cam_ldo_power_off(int mode)
{
	int ret = 0;

	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");

/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
				if (l11) {
					ret = regulator_disable(l11);
			if (ret)
				cam_err("error disabling regulator\n");
		}
	}

	usleep(10);

/*VT core 1.2 - CAM_DVDD_1P5V*/
#if defined(CONFIG_SR130PC20)
	if (l29) {
		ret = regulator_disable(l29);
		if (ret)
			cam_err("error disabling regulator\n");
	}
#else/*CONFIG_SR030PC50*/
			if (l30) {
				ret = regulator_disable(l30);
		if (ret)
			cam_err("error disabling regulator\n");
	}
#endif
	usleep(8000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
			if (l32) {
				ret = regulator_disable(l32);
				if (ret)
					cam_err("error disabling regulator\n");
			}

	usleep(8000);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
			if (l34) {
					ret = regulator_disable(l34);
					if (ret)
						cam_err("error disabling regulator\n");
			}

	usleep(10);

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}
#elif defined CONFIG_IMX175
void msm_camera_gpio_install(void)
{
	cam_err("[Start]\n");
	int32_t rc = 0;

	/* init Camera GPIOs */
	/*Flash Set : GPIO 3*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);

	/*Flash UNLOCK : GPIO 64*/
	gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);

	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*GPIO_CAM_CORE_EN  : GPIO 6*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
}
#else /*#if defined(CONFIG_MACH_BOOST)*/
void msm_camera_gpio_install(void)
{
	cam_err("Camera GPIO install!!\n");
	cam_err("Invalid function call.\n");
}
void cam_ldo_power_on(int mode)
{
	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

	cam_err("Invalid function call.\n");
}
void cam_ldo_power_off(int mode)
{
	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");
	cam_err("Invalid function call.\n");
}

#endif
