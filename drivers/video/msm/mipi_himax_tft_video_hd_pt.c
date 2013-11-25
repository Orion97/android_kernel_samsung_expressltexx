/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
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

#include "msm_fb.h"
#include "mipi_himax_tft.h"

static struct msm_panel_info pinfo;
static struct mipi_panel_data mipi_pd;

static char SETEXTENSION[] = {
	0xB9,
	0xff, 0x83, 0x94,
};

static char SETDGCLUT[] = {
	0xc1,
	0x01, 0x00, 0x07,
	0x0f, 0x19, 0x1f, 0x28,
	0x30, 0x38, 0x41, 0x49,
	0x52, 0x59, 0x61, 0x69,
	0x72, 0x7a, 0x82, 0x8a,
	0x91, 0x9a, 0xa1, 0xa9,
	0xb0, 0xb9, 0xc2, 0xc9,
	0xd1, 0xd9, 0xe1, 0xe8,
	0xf1, 0xf7, 0xff, 0x1d,
	0xda, 0x23, 0x32, 0x6a,
	0x9c, 0x25, 0x17, 0x00,
	0x00, 0x07, 0x0f, 0x19,
	0x1f, 0x27, 0x30, 0x36,
	0x3f, 0x47, 0x4f, 0x56,
	0x5e, 0x66, 0x6e, 0x76,
	0x7e, 0x86, 0x8d, 0x96,
	0x9e, 0xa5, 0xac, 0xb4,
	0xbd, 0xc6, 0xcd, 0xd5,
	0xde, 0xe5, 0xef, 0xf6,
	0xff, 0x1d, 0x92, 0x07,
	0x4a, 0xcc, 0x1b, 0x22,
	0x4e, 0x00, 0x00, 0x07,
	0x0f, 0x19, 0x20, 0x29,
	0x31, 0x39, 0x42, 0x4b,
	0x54, 0x5b, 0x63, 0x6c,
	0x75, 0x7d, 0x85, 0x8d,
	0x95, 0x9d, 0xa4, 0xac,
	0xb4, 0xbc, 0xc5, 0xcc,
	0xd4, 0xdc, 0xe2, 0xea,
	0xf2, 0xf8, 0xff, 0x1d,
	0x0f, 0xd3, 0xa7, 0x26,
	0xf5, 0x90, 0xb9, 0x00,
};

static char SETDGCLUT_T05[] = {
	0xc1,
	0x01, 0x00, 0x04,
	0x0E, 0x18, 0x1E, 0x26,
	0x2F, 0x36, 0x3E, 0x47,
	0x4E, 0x56, 0x5D, 0x65,
	0x6D, 0x75, 0x7D, 0x84,
	0x8C, 0x94, 0x9C, 0xA4,
	0xAD, 0xB5, 0xBD, 0xC5,
	0xCC, 0xD4, 0xDE, 0xE5,
	0xEE, 0xF7, 0xFF, 0x3F,
	0x9A, 0xCE, 0xD4, 0x21,
	0xA1, 0x26, 0x54, 0x00,
	0x00, 0x04, 0x0E, 0x19,
	0x1f, 0x27, 0x30, 0x37,
	0x40, 0x48, 0x50, 0x58,
	0x60, 0x67, 0x6F, 0x77,
	0x7F, 0x87, 0x8F, 0x97,
	0x9F, 0xA7, 0xB0, 0xB8,
	0xC0, 0xC8, 0xCE, 0xD8,
	0xE0, 0xE7, 0xF0, 0xF7,
	0xFF, 0x3C, 0xEB, 0xFD,
	0x2F, 0x66, 0xA8, 0x2C,
	0x46, 0x00, 0x00, 0x04,
	0x0E, 0x18, 0x1E, 0x26,
	0x30, 0x38, 0x41, 0x4A,
	0x52, 0x5A, 0x62, 0x6B,
	0x73, 0x7B, 0x83, 0x8C,
	0x94, 0x9C, 0xA5, 0xAD,
	0xB6, 0xBD, 0xC5, 0xCC,
	0xD4, 0xDD, 0xE3, 0xEB,
	0xF2, 0xF9, 0xFF, 0x3F,
	0xA4, 0x8A, 0x8F, 0xC7,
	0x33, 0xF5, 0xE9, 0x00,
};

static char EXTRA1[] = {
	0xc6,
	0x0c,
};
static char EXTRA2[] = {
	0xc7,
	0x00, 0x30,
};

static char EXTRA2_T05[] = {
	0xc7,
	0x00, 0x20,
};

static char SETPTBA[] = {
	0xBF,
	0x06,
	0x10,
};

static char GAMSET[] = {
	0x26,
	0x00,
};

static char MADCTL[] = {
	0x36,
	0x00,
};

static char COLMOD[] = {
	0x3a,
	0x70,
};

static char WRDISBV[] = {
	0x51,
	0xC3,
};

static char WRCTLLD[] = {
	0x53,
	0x2C,
};

static char WRCABC[] = {
	0x55,
	0x00,
};

static char WRCABCMB[] = {
	0x5E,
	0x00,
};


static char SETCABC[] = {
	0xc9,
	0x1f, 0x00, 0x1e, 0x1e, 0x00,
	0x00, 0x00, 0x01, 0x3e,
	0x00, 0x00,

};


/*
static char SETMIPI[] = {
	0xBA,
	0x13, 0x82,
	0x00,
};

static char SETCABC[] = {
	0xc9,
	0x2f, 0x2b, 0x1e, 0x1e, 0x00,
	0x00, 0x00, 0x01, 0x3e,
	0x00, 0x00,
};

static char SETCABCGAIN[] = {
	0xca,
	0x40, 0x3c, 0x38, 0x34, 0x33,
	0x32, 0x2b, 0x24, 0x22,
	0x00, 0x00,
};

static char SETPANEL[] = {
	0xcc,
	0x05,
	0x00, 0x00,
};

static char SETGIP[] = {
	0xd5,
	0x4c, 0x01, 0x06, 0x23, 0xcd,
	0x01, 0xef, 0x45, 0x67, 0x89,
	0xab, 0x00, 0x00, 0xdc, 0x32,
	0xfe, 0x10, 0xba, 0x98, 0x76,
	0x54, 0x00, 0x00, 0x40,
	0x00, 0x00, 0x00,
};

static char SETGAMMA[] = {
	0xe0,
	0x21, 0x2d, 0x30, 0x3c, 0x3f,
	0x3f, 0x39, 0x4f, 0x46, 0x4c,
	0x4e, 0xd3, 0x15, 0x13, 0x15,
	0x10, 0x18, 0x21, 0x2d, 0x30,
	0x3c, 0x3f, 0x3f, 0x39, 0x4f,
	0x46, 0x4c, 0x4e, 0xd3, 0x15,
	0x13, 0x15, 0x10, 0x18,
	0x00,
};
*/

static char SLPIN[2] = {
	0x10,
};

static char SLPOUT[2] = {
	0x11,
};
static char DISPON[2] = {
	0x29,
};
static char DISPOFF[2] = {
	0x28,
};

static struct dsi_cmd_desc samsung_display_on_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(SETEXTENSION), SETEXTENSION},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(SETDGCLUT), SETDGCLUT},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(EXTRA1), EXTRA1},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(EXTRA2), EXTRA2},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(SETPTBA), SETPTBA},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(GAMSET), GAMSET},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(MADCTL), MADCTL},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(COLMOD), COLMOD},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRDISBV), WRDISBV},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCTLLD), WRCTLLD},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABC), WRCABC},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABCMB), WRCABCMB},

	{DTYPE_DCS_WRITE, 1, 0, 0, 120,
		sizeof(SLPOUT), SLPOUT},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
		sizeof(DISPON), DISPON},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(SETCABC), SETCABC},

};

static struct dsi_cmd_desc samsung_display_on_cmds_t05[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(SETEXTENSION), SETEXTENSION},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(SETDGCLUT_T05), SETDGCLUT_T05},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(EXTRA1), EXTRA1},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(EXTRA2_T05), EXTRA2_T05},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(SETPTBA), SETPTBA},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(GAMSET), GAMSET},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(MADCTL), MADCTL},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(COLMOD), COLMOD},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRDISBV), WRDISBV},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCTLLD), WRCTLLD},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABC), WRCABC},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABCMB), WRCABCMB},

	{DTYPE_DCS_WRITE, 1, 0, 0, 120,
		sizeof(SLPOUT), SLPOUT},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
		sizeof(DISPON), DISPON},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(SETCABC), SETCABC},

};

static struct dsi_cmd_desc samsung_panel_on_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 0,
		sizeof(DISPON), DISPON},
};
static struct dsi_cmd_desc samsung_panel_ready_to_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 5,
		sizeof(DISPOFF), DISPOFF},
};

static struct dsi_cmd_desc samsung_panel_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 0,
		sizeof(SLPIN), SLPIN},
};

static struct dsi_cmd_desc samsung_panel_late_on_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 0,
		sizeof(DISPON), DISPON},
};

static struct dsi_cmd_desc samsung_panel_early_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 0,
		sizeof(DISPOFF), DISPOFF},
};


enum {
	GAMMA_15CD,
	GAMMA_30CD,
	GAMMA_40CD,
	GAMMA_70CD,
	GAMMA_90CD,
	GAMMA_110CD,
	GAMMA_130CD,
	GAMMA_142CD,
	GAMMA_154CD,
	GAMMA_166CD,
	GAMMA_178CD,
	GAMMA_190CD,
	GAMMA_200CD,
	GAMMA_212CD,
	GAMMA_224CD,
	GAMMA_236CD,
	GAMMA_248CD,
	GAMMA_260CD,
	GAMMA_270CD,
	GAMMA_280CD,
	GAMMA_290CD,
	GAMMA_300CD,
	GAMMA_310CD,
	GAMMA_330CD,
};

static int lux_tbl[] = {
	10, 15, 20, 35, 50, 65, 80, 85, 90, 95, 100, 105,
	115, 125, 133, 140, 147, 155, 161, 168, 175, 180, 185, 195
};

static int get_candela_index(int bl_level)
{
	int backlightlevel;
	int cd;
		switch (bl_level) {
		case 0 ... 29:
			backlightlevel = GAMMA_15CD; /* duty 7 */
			break;
		case 30 ... 39:
			backlightlevel = GAMMA_30CD; /* duty 15 */
			break;
		case 40 ... 49:
			backlightlevel = GAMMA_40CD; /* duty 20 */
			break;
		case 50 ... 59:
			backlightlevel = GAMMA_70CD; /* duty 35 */
			break;
		case 60 ... 69:
			backlightlevel = GAMMA_90CD;  /* duty 50 */
			break;
		case 70 ... 79:
			backlightlevel = GAMMA_110CD;  /* duty 65 */
			break;
		case 80 ... 89:
			backlightlevel = GAMMA_130CD;  /* duty 80 */
			break;
		case 90 ... 99:
			backlightlevel = GAMMA_142CD;  /* duty 86 */
			break;
		case 100 ... 109:
			backlightlevel = GAMMA_154CD;  /* duty 92 */
			break;
		case 110 ... 119:
			backlightlevel = GAMMA_166CD;  /* duty 98 */
			break;
		case 120 ... 129:
			backlightlevel = GAMMA_178CD;  /* duty 104 */
			break;
		case 130 ... 139:
			backlightlevel = GAMMA_190CD;  /* duty 110 */
			break;
		case 140 ... 149:
			backlightlevel = GAMMA_200CD;  /* duty 117 */
			break;
		case 150 ... 159:
			backlightlevel = GAMMA_212CD;  /* duty 125 */
			break;
		case 160 ... 169:
			backlightlevel = GAMMA_224CD;  /* duty 132 */
			break;
		case 170 ... 179:
			backlightlevel = GAMMA_236CD;  /* duty 140 */
			break;
		case 180 ... 189:
			backlightlevel = GAMMA_248CD;  /* duty 147 */
			break;
		case 190 ... 199:
			backlightlevel = GAMMA_260CD;  /* duty 155 */
			break;
		case 200 ... 209:
			backlightlevel = GAMMA_270CD;  /* duty 161 */
			break;
		case 210 ... 219:
			backlightlevel = GAMMA_280CD;  /* duty 168 */
			break;
		case 220 ... 229:
			backlightlevel = GAMMA_290CD;  /* duty 175 */
			break;
		case 230 ... 239:
			backlightlevel = GAMMA_300CD;  /* duty 180 */
			break;
		case 240 ... 249:
			backlightlevel = GAMMA_310CD;  /* duty 185 */
			break;
		case 250 ... 255:
			backlightlevel = GAMMA_330CD;  /* duty 195 */
			break;
		default:
			backlightlevel = GAMMA_15CD;
			break;
		}

	cd = lux_tbl[backlightlevel];
	return cd;
}

static struct mipi_panel_data mipi_pd = {
	.panel_name = "JDI_L5F31188T07\n",
	.ready_to_on = {samsung_display_on_cmds
				, ARRAY_SIZE(samsung_display_on_cmds)},
	.ready_to_on_t05 = {samsung_display_on_cmds_t05
				, ARRAY_SIZE(samsung_display_on_cmds_t05)},
	.ready_to_off	= {samsung_panel_ready_to_off_cmds
				, ARRAY_SIZE(samsung_panel_ready_to_off_cmds)},
	.on		= {samsung_panel_on_cmds
				, ARRAY_SIZE(samsung_panel_on_cmds)},
	.off		= {samsung_panel_off_cmds
				, ARRAY_SIZE(samsung_panel_off_cmds)},
	.late_on	= {samsung_panel_late_on_cmds
				, ARRAY_SIZE(samsung_panel_late_on_cmds)},
	.early_off	= {samsung_panel_early_off_cmds
				, ARRAY_SIZE(samsung_panel_early_off_cmds)},
	.set_brightness_level = get_candela_index,
};

#ifdef CONFIG_CAMERA_PICTURE_QUALIRY_TEST
static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 450MHz, 4 lane, RGB888 */
	{0x09, 0x08, 0x05, 0x00, 0x20},    /* regulator */
	/* timing */
	{0xb7, 0x2b, 0x1d, 0x00, 0x57, 0x62, 0x22, 0x2f,
	0x32, 0x03, 0x04, 0xa0},
    /* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
    /* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0x7f, 0x30, 0xda, 0x00, 0x50, 0x48, 0x63,
	0x41, 0x0f, 0x01,
	0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01 },
};

static int __init mipi_video_himax_tft_hd_pt_init(void)
{
	int ret;
#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_video_himax_tft_hd"))
		return 0;
#endif

	pinfo.xres = 720;
	pinfo.yres = 1280;
	pinfo.mode2_xres = 0;
	pinfo.mode2_yres = 0;
	pinfo.mode2_bpp = 0;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.lcdc.h_pulse_width = 36;
	pinfo.lcdc.h_back_porch = 108;
	pinfo.lcdc.h_front_porch = 108;
	pinfo.lcdc.v_pulse_width = 2;
	pinfo.lcdc.v_back_porch = 4;
	pinfo.lcdc.v_front_porch = 11;
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = 255;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;
	pinfo.clk_rate = 450000000;

/*
	mipi clk =[(720+108+108+36)*(1280+11+4+2)*24*60]/4(lanes)
		=453846240Hz

	pixel clk=(mipi clk *4)/24
		=75641040Hz
*/
	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = TRUE;
	pinfo.mipi.hbp_power_stop = TRUE;
	pinfo.mipi.hsa_power_stop = TRUE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = TRUE;
	pinfo.mipi.traffic_mode = DSI_NON_BURST_SYNCH_PULSE;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;

	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.dlane_swap = 0x00;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.data_lane2 = TRUE;
	pinfo.mipi.data_lane3 = TRUE;

	pinfo.mipi.force_clk_lane_hs = 1;
	pinfo.mipi.no_max_pkt_size = 1;

	pinfo.mipi.tx_eot_append = FALSE;
	pinfo.mipi.t_clk_post = 0x20;
	pinfo.mipi.t_clk_pre = 0x2D;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;

	pinfo.mipi.esc_byte_ratio = 4;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	ret = mipi_samsung_device_register(&pinfo, MIPI_DSI_PRIM,
				MIPI_DSI_PANEL_720P_PT,
				&mipi_pd);
	if (ret)
		pr_info("%s: failed to register device!\n", __func__);

		pr_info("%s:\n", __func__);
	return ret;
}

#else
static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 450MHz, 4 lane, RGB888 */
	{0x0F, 0x0a, 0x04, 0x00, 0x20}, /* regulator */
	/* timing   */
	{0x7F, 0x30, 0x13, 0x00, 0x41, 0x47, 0x17, 0x34,
	 0x20, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
	/* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0x7f, 0x1, 0x1a, 0x00, 0x50, 0x48, 0x63,
	 0x31, 0x0F, 0x03,/* 4 lane */
	 0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01},
};

static int __init mipi_video_himax_tft_hd_pt_init(void)
{
	int ret;
#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_video_himax_tft_hd"))
		return 0;
#endif

	pinfo.xres = 720;
	pinfo.yres = 1280;
	pinfo.height = 101;
	pinfo.width = 57;
	pinfo.mode2_xres = 0;
	pinfo.mode2_yres = 0;
	pinfo.mode2_bpp = 0;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.lcdc.h_pulse_width = 36;
	pinfo.lcdc.h_back_porch = 108;
	pinfo.lcdc.h_front_porch = 108;
	pinfo.lcdc.v_pulse_width = 2;
	pinfo.lcdc.v_back_porch = 4;
	pinfo.lcdc.v_front_porch = 11;
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = 255;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;
	pinfo.clk_rate = 450000000;

/*
	mipi clk =[(720+108+108+36)*(1280+11+4+2)*24*60]/4(lanes)
		=453846240Hz

	pixel clk=(mipi clk *4)/24
		=75641040Hz
*/
	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = FALSE;
	pinfo.mipi.hbp_power_stop = FALSE;
	pinfo.mipi.hsa_power_stop = FALSE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = TRUE;
	pinfo.mipi.traffic_mode = DSI_BURST_MODE;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;

	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.dlane_swap = 0x00;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.data_lane2 = TRUE;
	pinfo.mipi.data_lane3 = TRUE;

	pinfo.mipi.force_clk_lane_hs = 1;
	pinfo.mipi.no_max_pkt_size = 0;

	pinfo.mipi.tx_eot_append = FALSE;
	pinfo.mipi.t_clk_post = 0x19;
	pinfo.mipi.t_clk_pre = 0x2D;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;

	pinfo.mipi.esc_byte_ratio = 4;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	ret = mipi_samsung_device_register(&pinfo, MIPI_DSI_PRIM,
				MIPI_DSI_PANEL_720P_PT,
				&mipi_pd);
	if (ret)
		pr_info("%s: failed to register device!\n", __func__);

		pr_info("%s:\n", __func__);
	return ret;
}
#endif
module_init(mipi_video_himax_tft_hd_pt_init);
