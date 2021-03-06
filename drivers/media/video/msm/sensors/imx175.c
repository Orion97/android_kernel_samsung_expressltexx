/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
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

#include "msm_sensor.h"
#include "imx175.h"
#define SENSOR_NAME "imx175"
#define PLATFORM_DRIVER_NAME "msm_camera_imx175"
#define imx175_obj imx175_##obj
#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[CAMERA]][%s:%d] " fmt,		\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

#include <linux/gpio.h>
#include <mach/gpiomux.h>
#include <mach/msm8930-gpio.h>
#include <mach/socinfo.h>

DEFINE_MUTEX(imx175_mut);

static struct msm_sensor_ctrl_t imx175_s_ctrl;

static struct v4l2_subdev_info imx175_subdev_info[] = {
	{
	.code = V4L2_MBUS_FMT_SBGGR10_1X10,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt = 1,
	.order = 0,
	},
	/* more can be supported, to be added later */
};

static struct regulator *l11, *l32, *l34;

static struct msm_camera_i2c_conf_array imx175_init_conf[] = {
	{&imx175_recommend_settings[0],
	ARRAY_SIZE(imx175_recommend_settings), 0, MSM_CAMERA_I2C_BYTE_DATA}
};

static struct msm_camera_i2c_conf_array imx175_confs[] = {
	{&imx175_res_settings[0],
	ARRAY_SIZE(imx175_res_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx175_res_settings[0],
	ARRAY_SIZE(imx175_res_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
};

static struct msm_sensor_output_info_t imx175_dimensions[] = {
/*Sony 3268x2454, 30.5fps*/
	{
		.x_output = 3268,
		.y_output = 2454,
		.line_length_pclk = 3412,
		.frame_length_lines = 2494,
		.vt_pixel_clk = 259200000,
		.op_pixel_clk = 259200000,
		.binning_factor = 1,
	},
/*Sony 3268x2454, 30.5fps*/
	{
		.x_output = 3268,
		.y_output = 2454,
		.line_length_pclk = 3412,
		.frame_length_lines = 2494,
		.vt_pixel_clk = 259200000,
		.op_pixel_clk = 259200000,
		.binning_factor = 1,
	},
};

static struct msm_sensor_output_reg_addr_t imx175_reg_addr = {
	.x_output = 0x034c,
	.y_output = 0x034e,
	.line_length_pclk = 0x0342,
	.frame_length_lines = 0x0340,
};

static struct msm_sensor_id_info_t imx175_id_info = {
	.sensor_id_reg_addr = 0x0,
	.sensor_id = 0x0175,
};

static struct msm_sensor_exp_gain_info_t imx175_exp_gain_info = {
	.coarse_int_time_addr = 0x0202,
	.global_gain_addr = 0x0205,
	.vert_offset = 4,
};

static const struct i2c_device_id imx175_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&imx175_s_ctrl},
	{ }
};

static struct i2c_driver imx175_i2c_driver = {
	.id_table = imx175_i2c_id,
	.probe  = msm_sensor_i2c_probe,
	.driver = {
		.name = SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client imx175_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};


#define REG_DIGITAL_GAIN_GREEN_R  0x020E
#define REG_DIGITAL_GAIN_RED  0x0210
#define REG_DIGITAL_GAIN_BLUE  0x0212
#define REG_DIGITAL_GAIN_GREEN_B  0x0214
#define QC_TEST	0

int32_t imx175_sensor_write_exp_gain(struct msm_sensor_ctrl_t *s_ctrl,
		uint32_t gain, uint32_t line)
{
	uint32_t fl_lines;
	uint8_t offset;
#if QC_TEST
	static uint16_t max_analog_gain  = 0x00E0;	/*upto 8x A gain*/
#else
	static uint16_t max_analog_gain  = 0x00F0;	/*upto 16x A gain*/
#endif
	uint16_t digital_gain = 0x0100;
	/*Max reg = 131040*/

	pr_err("[shchang1] Gain = %d, Line = %d, current_frame_length = %d\n",
		gain, line, s_ctrl->curr_frame_length_lines);
	/*shchang@qualcomm.com : 1009*/

	fl_lines = s_ctrl->curr_frame_length_lines;
	fl_lines = (fl_lines * s_ctrl->fps_divider) / Q10;
	offset = s_ctrl->sensor_exp_gain_info->vert_offset;
	if (line > (fl_lines - offset))
		fl_lines = line + offset;

	/*pr_err("[shchang2] Gain = %d, Line = %d, fl_line = %d\n",
		gain, line, fl_lines);*/	/*shchang@qualcomm.com :1009*/
	/*pr_err("[shchang3] s_ctrl->curr_line_length_pclk = %d\n",
		s_ctrl->curr_line_length_pclk);*/

	digital_gain = ((gain & 0xFFF00) >> 8);
	gain = (gain & 0x00FF);

	if (gain > max_analog_gain)
		gain = max_analog_gain;

	if (digital_gain < 0x0100)
		digital_gain = 0x0100;

	pr_err("[shchang]digital_gain =0x%X Analog gain = %d\n",
		digital_gain, gain);

	s_ctrl->func_tbl->sensor_group_hold_on(s_ctrl);
	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		s_ctrl->sensor_output_reg_addr->frame_length_lines, fl_lines,
		MSM_CAMERA_I2C_WORD_DATA);
	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		s_ctrl->sensor_exp_gain_info->coarse_int_time_addr, line,
		MSM_CAMERA_I2C_WORD_DATA);
	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		s_ctrl->sensor_exp_gain_info->global_gain_addr, gain,
		MSM_CAMERA_I2C_BYTE_DATA);
	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		REG_DIGITAL_GAIN_GREEN_R, digital_gain,
		MSM_CAMERA_I2C_WORD_DATA);

	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		REG_DIGITAL_GAIN_RED, digital_gain,
		MSM_CAMERA_I2C_WORD_DATA);

	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		REG_DIGITAL_GAIN_BLUE, digital_gain,
		MSM_CAMERA_I2C_WORD_DATA);

	msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
		REG_DIGITAL_GAIN_GREEN_B, digital_gain,
		MSM_CAMERA_I2C_WORD_DATA);

	s_ctrl->func_tbl->sensor_group_hold_off(s_ctrl);
	return 0;
}

static int __init imx175_sensor_init_module(void)
{
	/*Start : shchang@qualcomm.com : 1104 -FROM*/
	msm_sensor_enable_debugfs(NULL);
	/*End : shchang@qualcomm.com : 1104 - FROM*/

	return i2c_add_driver(&imx175_i2c_driver);
}

static struct msm_cam_clk_info imx_175_cam_clk_info[] = {
	{"cam_clk", MSM_SENSOR_MCLK_24HZ},
};

void sensor_native_control(void __user *arg)
{
	struct ioctl_native_cmd ctrl_info;
	struct msm_camera_v4l2_ioctl_t *ioctl_ptr = arg;

	if (copy_from_user(&ctrl_info,
		(void __user *)ioctl_ptr->ioctl_ptr,
		sizeof(ctrl_info))) {
		cam_err("fail copy_from_user!");
		goto FAIL_END;
	}

	cam_err("mode : %d", ctrl_info.mode);
	if (copy_to_user((void __user *)ioctl_ptr->ioctl_ptr,
		  (const void *)&ctrl_info,
			sizeof(ctrl_info))) {
		cam_err("fail copy_to_user!");
		goto FAIL_END;
	}

	return;

FAIL_END:
	cam_err("Error : can't handle native control");

}

/*Start : shchang@qualcomm.com*/
int32_t imx175_FROM_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{

	int32_t rc = 0;
	/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	l34 = regulator_get(NULL, "8917_l34");
	if (!l34)
		pr_err("Error getting l34\n");
	else {
		rc = regulator_set_voltage(l34, 1800000, 1800000);
		if (rc) {
			pr_err("error setting voltage\n");
			regulator_put(l34);
		} else {
			rc = regulator_enable(l34);
			if (rc) {
				pr_err("error enabling regulator\n");
				regulator_set_voltage(l34, 0, 1800000);
				regulator_put(l34);
			} else
				udelay(1000);
		}
	}
	return rc;
}


int32_t imx175_FROM_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{

	int32_t rc = 0;
	if (l34) {
		rc = regulator_disable(l34);
		if (rc)
			pr_err("error disabling regulator l34\n");
		udelay(1000);
	}
	regulator_set_voltage(l34, 0, 1800000);
	regulator_put(l34);
	l34 = NULL;
	return rc;
}
/*Start : shchang@qualcomm.com*/

int32_t imx175_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{
	int32_t rc = 0;
	int ret = 0;
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;
	struct device *dev = NULL;
	if (s_ctrl->sensor_device_type == MSM_SENSOR_PLATFORM_DEVICE)
		dev = &s_ctrl->pdev->dev;
	else
		dev = &s_ctrl->sensor_i2c_client->client->dev;
	if (s_ctrl->sensor_device_type == MSM_SENSOR_PLATFORM_DEVICE) {
		msm_sensor_cci_util(s_ctrl->sensor_i2c_client,
			MSM_CCI_RELEASE);
	}

	rc = msm_camera_request_gpio_table(data, 1);
	if (rc < 0) {
		cam_err("%s: request gpio failed\n", __func__);
		goto request_gpio_failed;
	}
	/*****************************************/
	/* test code for camera power control */
	cam_err("---- [QCT src] Camera power on ----\n");

	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917) {
		cam_err("PMIC_MODEL_PM8917");
		/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		l32 = regulator_get(NULL, "8917_l32");
		ret = regulator_set_voltage(l32 , 2800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l32);
		if (ret)
			cam_err("error enabling regulator\n");

		cam_err("check GPIO_CAM_A_EN : %d\n", ret);
		mdelay(5);

		/* CAM_ISP_CORE_1P2 */
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		cam_err("request GPIO_CAM_CORE_EN\n");

		gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
		ret = gpio_get_value(GPIO_CAM_CORE_EN);
		if (ret)
			cam_err("check GPIO_CAM_CORE_EN : %d\n", ret);

		mdelay(5);

		/* enable MCLK */
		gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);
		if (s_ctrl->clk_rate != 0)
			imx_175_cam_clk_info->clk_rate = s_ctrl->clk_rate;

		rc = msm_cam_clk_enable(dev, imx_175_cam_clk_info,
			s_ctrl->cam_clk, ARRAY_SIZE(imx_175_cam_clk_info), 1);

		if (rc < 0) {
			cam_err("%s: clk enable failed\n", __func__);
			goto enable_clk_failed;
		}
		mdelay(5);

		/* AF_28 - CAM_AF_2P8 */
		l11 = regulator_get(NULL, "8917_l11");
		ret = regulator_set_voltage(l11 , 2800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l11);
		if (ret)
			cam_err("error enabling regulator\n");

		cam_err("check GPIO_CAM_AF_EN : %d\n", ret);
		mdelay(5);

		/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		l34 = regulator_get(NULL, "8917_l34");
		ret = regulator_set_voltage(l34 , 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l34);
		if (ret)
			cam_err("error enabling regulator\n");

		cam_err("check GPIO_CAM_A_EN : %d\n", ret);
		mdelay(5);

		cam_err("set GPIO_CAM_MAIN_RST to 1\n");
		gpio_set_value_cansleep(GPIO_CAM1_RST_N, 1);
		ret = gpio_get_value(GPIO_CAM1_RST_N);
		if (ret)
			cam_err("check GPIO_CAM_MAIN_RST : %d\n", ret);

		mdelay(5);
	} else	{
		/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		gpio_set_value_cansleep(GPIO_CAM_A_EN, 1);
		ret = gpio_get_value(GPIO_CAM_A_EN);
		if (ret)
			cam_err("check GPIO_CAM_A_EN : %d\n", ret);

		mdelay(5);
		/* CAM_ISP_CORE_1P2 */
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

		gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
		ret = gpio_get_value(GPIO_CAM_CORE_EN);
		if (ret)
			cam_err("check GPIO_CAM_CORE_EN : %d\n", ret);

		mdelay(5);

		/* enable MCLK */
		gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);
		if (s_ctrl->clk_rate != 0)
			imx_175_cam_clk_info->clk_rate = s_ctrl->clk_rate;

		rc = msm_cam_clk_enable(dev, imx_175_cam_clk_info,
			s_ctrl->cam_clk, ARRAY_SIZE(imx_175_cam_clk_info), 1);

		if (rc < 0) {
			cam_err("%s: clk enable failed\n", __func__);
			goto enable_clk_failed;
		}
		mdelay(5);

		/* AF_28 - CAM_AF_2P8 */
		gpio_set_value_cansleep(GPIO_CAM_AF_EN, 1);
		ret = gpio_get_value(GPIO_CAM_AF_EN);
		if (ret)
			cam_err("check GPIO_CAM_AF_EN : %d\n", ret);

		mdelay(5);
		/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_IO_EN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);

		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 1);
		ret = gpio_get_value(GPIO_CAM_IO_EN);
		if (ret)
			cam_err("check GPIO_CAM_IO_EN : %d\n", ret);

		cam_err("set GPIO_CAM_MAIN_RST to 1\n");
		mdelay(5);
		gpio_set_value_cansleep(GPIO_CAM1_RST_N, 1);
		ret = gpio_get_value(GPIO_CAM1_RST_N);
		if (ret)
			cam_err("check GPIO_CAM_MAIN_RST : %d\n", ret);

		mdelay(5);
	}

	/*****************************************/
	usleep_range(1000, 2000);

	rc = msm_camera_config_gpio_table(data, 1);
	if (rc < 0) {
		cam_err("%s: config gpio failed\n", __func__);
		goto config_gpio_failed;
	}

	if (data->sensor_platform_info->ext_power_ctrl != NULL)
		data->sensor_platform_info->ext_power_ctrl(1);

	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_enable_i2c_mux(data->sensor_platform_info->i2c_conf);

	if (s_ctrl->sensor_device_type == MSM_SENSOR_PLATFORM_DEVICE) {
		rc = msm_sensor_cci_util(s_ctrl->sensor_i2c_client,
			MSM_CCI_INIT);
		if (rc < 0) {
			pr_err("%s cci_init failed\n", __func__);
			goto cci_init_failed;
		}
	}
	s_ctrl->curr_res = MSM_SENSOR_INVALID_RES;
	return rc;

cci_init_failed:
	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_disable_i2c_mux(
			data->sensor_platform_info->i2c_conf);
enable_clk_failed:
	msm_camera_config_gpio_table(data, 0);
config_gpio_failed:
enable_vreg_failed:
config_vreg_failed:
	msm_camera_request_gpio_table(data, 0);
request_gpio_failed:
	kfree(s_ctrl->reg_ptr);
	return rc;
}

int32_t imx175_sensor_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;
	int ret = 0;
	struct device *dev = NULL;
	if (s_ctrl->sensor_device_type == MSM_SENSOR_PLATFORM_DEVICE)
		dev = &s_ctrl->pdev->dev;
	else
		dev = &s_ctrl->sensor_i2c_client->client->dev;
	if (s_ctrl->sensor_device_type == MSM_SENSOR_PLATFORM_DEVICE) {
		msm_sensor_cci_util(s_ctrl->sensor_i2c_client,
			MSM_CCI_RELEASE);
	}


	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_disable_i2c_mux(
			data->sensor_platform_info->i2c_conf);

	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917) {
		cam_err("set GPIO_CAM_MAIN_RST to 0\n");
		gpio_set_value_cansleep(GPIO_CAM1_RST_N, 0);
		ret = gpio_get_value(GPIO_CAM1_RST_N);
		if (ret)
			cam_err("check GPIO_CAM1_RST_N: %d\n", ret);

		/* AF_28 - CAM_AF_2P8 */
		if (l11) {
			ret = regulator_disable(l11);
			if (ret)
				cam_err("error disabling regulator\n");
		}

		mdelay(5);

		/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		if (l34) {
			ret = regulator_disable(l34);
			if (ret)
				cam_err("error disabling regulator\n");
		}

		mdelay(5);

		/* CAM_ISP_CORE_1P2 */
		gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);
		ret = gpio_get_value(GPIO_CAM_CORE_EN);
		if (ret)
			cam_err("check GPIO_CAM_CORE_EN : %d\n", ret);

		mdelay(5);

		/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		if (l32) {
			ret = regulator_disable(l32);
			if (ret)
				cam_err("error disabling regulator\n");
		}

		mdelay(5);
	} else {
		cam_err("set GPIO_CAM_MAIN_RST to 0\n");
		gpio_set_value_cansleep(GPIO_CAM1_RST_N, 0);
		ret = gpio_get_value(GPIO_CAM1_RST_N);
		if (ret)
			cam_err("check GPIO_CAM1_RST_N: %d\n", ret);

		udelay(1000);

		gpio_set_value_cansleep(GPIO_CAM_AF_EN, 0);
		ret = gpio_get_value(GPIO_CAM_AF_EN);
		if (ret)
			cam_err("check GPIO_CAM_AF_EN : %d\n", ret);

		udelay(1000);

		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 0);
		ret = gpio_get_value(GPIO_CAM_IO_EN);
		if (ret)
			cam_err("check GPIO_CAM_IO_EN : %d\n", ret);

		udelay(1000);

		gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);
		ret = gpio_get_value(GPIO_CAM_CORE_EN);
		if (ret)
			cam_err("check GPIO_CAM_CORE_EN : %d\n", ret);

		udelay(1000);

		gpio_set_value_cansleep(GPIO_CAM_A_EN, 0);
		ret = gpio_get_value(GPIO_CAM_A_EN);
		if (ret)
			cam_err("check GPIO_CAM_A_EN : %d\n", ret);

		udelay(1000);
	}

	if (data->sensor_platform_info->ext_power_ctrl != NULL)
		data->sensor_platform_info->ext_power_ctrl(0);
	msm_cam_clk_enable(dev, imx_175_cam_clk_info,
		s_ctrl->cam_clk, ARRAY_SIZE(imx_175_cam_clk_info), 0);
	msm_camera_config_gpio_table(data, 0);
	msm_camera_request_gpio_table(data, 0);
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	kfree(s_ctrl->reg_ptr);
	return 0;
}


static struct v4l2_subdev_core_ops imx175_subdev_core_ops = {
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_video_ops imx175_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct v4l2_subdev_ops imx175_subdev_ops = {
	.core = &imx175_subdev_core_ops,
	.video  = &imx175_subdev_video_ops,
};

static struct msm_sensor_fn_t imx175_func_tbl = {
	.sensor_start_stream = msm_sensor_start_stream,
	.sensor_stop_stream = msm_sensor_stop_stream,
	.sensor_group_hold_on = msm_sensor_group_hold_on,
	.sensor_group_hold_off = msm_sensor_group_hold_off,
	.sensor_set_fps = msm_sensor_set_fps,
	.sensor_write_exp_gain = imx175_sensor_write_exp_gain,
	.sensor_write_snapshot_exp_gain = imx175_sensor_write_exp_gain,
	.sensor_setting = msm_sensor_setting,
	.sensor_csi_setting = msm_sensor_setting,
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = imx175_sensor_power_up,
	.sensor_power_down = imx175_sensor_power_down,
	.sensor_adjust_frame_lines = msm_sensor_adjust_frame_lines1,
	.sensor_get_csi_params = msm_sensor_get_csi_params,
	.eeprom_power_up = imx175_FROM_power_up,
	.eeprom_power_down = imx175_FROM_power_down,
};

static struct msm_sensor_reg_t imx175_regs = {
	.default_data_type = MSM_CAMERA_I2C_BYTE_DATA,
	.start_stream_conf = imx175_start_settings,
	.start_stream_conf_size = ARRAY_SIZE(imx175_start_settings),
	.stop_stream_conf = imx175_stop_settings,
	.stop_stream_conf_size = ARRAY_SIZE(imx175_stop_settings),
	.group_hold_on_conf = imx175_groupon_settings,
	.group_hold_on_conf_size = ARRAY_SIZE(imx175_groupon_settings),
	.group_hold_off_conf = imx175_groupoff_settings,
	.group_hold_off_conf_size = ARRAY_SIZE(imx175_groupoff_settings),
	.init_settings = &imx175_init_conf[0],
	.init_size = ARRAY_SIZE(imx175_init_conf),
	.mode_settings = &imx175_confs[0],
	.output_settings = &imx175_dimensions[0],
	.num_conf = ARRAY_SIZE(imx175_confs),
};

static struct msm_sensor_ctrl_t imx175_s_ctrl = {
	.msm_sensor_reg = &imx175_regs,
	.sensor_i2c_client = &imx175_sensor_i2c_client,
	.sensor_i2c_addr = 0x34,
	.sensor_output_reg_addr = &imx175_reg_addr,
	.sensor_id_info = &imx175_id_info,
	.sensor_exp_gain_info = &imx175_exp_gain_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
	/*.csi_params = &imx175_csi_params_array[0],*/
	.msm_sensor_mutex = &imx175_mut,
	.sensor_i2c_driver = &imx175_i2c_driver,
	.sensor_v4l2_subdev_info = imx175_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(imx175_subdev_info),
	.sensor_v4l2_subdev_ops = &imx175_subdev_ops,
	.func_tbl = &imx175_func_tbl,
	.clk_rate = MSM_SENSOR_MCLK_24HZ,
};

module_init(imx175_sensor_init_module);
MODULE_DESCRIPTION("Sony 8MP Bayer sensor driver");
MODULE_LICENSE("GPL v2");
