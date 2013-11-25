/* Copyright (c) 2009-2011, Code Aurora Forum. All rights reserved.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#ifndef _MDP4_VIDEO_TUNING_H_
#define _MDP4_VIDEO_TUNING_H_
#define SHARPNESS_DMB 52
#define SHARPNESS_VIDEO  -127
#define SHARPNESS_BYPASS 0
#define SHARPNESS_NEGATIVE	0

#define DMB_LUT		DMB_TUNE
#if defined(CONFIG_FB_MSM_MIPI_MAGNA_OLED_VIDEO_WVGA_PT) \
	|| defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT)
#define GALLERY_LUT	DARKGAMMA_LUT
#define VIDEO_LUT	DARKGAMMA_LUT
#else
#define GALLERY_LUT	BYPASS_LUT
#define VIDEO_LUT	BYPASS_LUT
#endif
#define NEGATIVE_LUT	NEGATIVE_TUNE
#define UI_LUT		BYPASS_LUT

#if  defined(CONFIG_FB_MSM_MIPI_MAGNA_OLED_VIDEO_WVGA_PT) \
	|| defined(CONFIG_FB_MSM_MIPI_HIMAX_TFT_VIDEO_HD_PT)
static unsigned int DARKGAMMA_LUT[] = {
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x00, 0x00, 0x00,
0x01, 0x01, 0x01,
0x03, 0x03, 0x03,
0x05, 0x05, 0x05,
0x07, 0x07, 0x07,
0x09, 0x09, 0x09,
0x0B, 0x0B, 0x0B,
0x0D, 0x0D, 0x0D,
0x0E, 0x0E, 0x0E,
0x0F, 0x0F, 0x0F,
0x10, 0x10, 0x10,
0x11, 0x11, 0x11,
0x12, 0x12, 0x12,
0x13, 0x13, 0x13,
0x14, 0x14, 0x14,
0x15, 0x15, 0x15,
0x16, 0x16, 0x16,
0x17, 0x17, 0x17,
0x18, 0x18, 0x18,
0x19, 0x19, 0x19,
0x1A, 0x1A, 0x1A,
0x1B, 0x1B, 0x1B,
0x1C, 0x1C, 0x1C,
0x1D, 0x1D, 0x1D,
0x1E, 0x1E, 0x1E,
0x1F, 0x1F, 0x1F,
0x20, 0x20, 0x20,
0x21, 0x21, 0x21,
0x22, 0x22, 0x22,
0x23, 0x23, 0x23,
0x24, 0x24, 0x24,
0x25, 0x25, 0x25,
0x26, 0x26, 0x26,
0x27, 0x27, 0x27,
0x28, 0x28, 0x28,
0x29, 0x29, 0x29,
0x2A, 0x2A, 0x2A,
0x2B, 0x2B, 0x2B,
0x2C, 0x2C, 0x2C,
0x2D, 0x2D, 0x2D,
0x2E, 0x2E, 0x2E,
0x2F, 0x2F, 0x2F,
0x30, 0x30, 0x30,
0x31, 0x31, 0x31,
0x32, 0x32, 0x32,
0x33, 0x33, 0x33,
0x34, 0x34, 0x34,
0x35, 0x35, 0x35,
0x36, 0x36, 0x36,
0x37, 0x37, 0x37,
0x38, 0x38, 0x38,
0x39, 0x39, 0x39,
0x3A, 0x3A, 0x3A,
0x3B, 0x3B, 0x3B,
0x3C, 0x3C, 0x3C,
0x3D, 0x3D, 0x3D,
0x3E, 0x3E, 0x3E,
0x3F, 0x3F, 0x3F,
0x40, 0x40, 0x40,
0x41, 0x41, 0x41,
0x42, 0x42, 0x42,
0x43, 0x43, 0x43,
0x44, 0x44, 0x44,
0x45, 0x45, 0x45,
0x46, 0x46, 0x46,
0x47, 0x47, 0x47,
0x48, 0x48, 0x48,
0x49, 0x49, 0x49,
0x4A, 0x4A, 0x4A,
0x4B, 0x4B, 0x4B,
0x4C, 0x4C, 0x4C,
0x4D, 0x4D, 0x4D,
0x4E, 0x4E, 0x4E,
0x4F, 0x4F, 0x4F,
0x50, 0x50, 0x50,
0x51, 0x51, 0x51,
0x52, 0x52, 0x52,
0x53, 0x53, 0x53,
0x54, 0x54, 0x54,
0x55, 0x55, 0x55,
0x56, 0x56, 0x56,
0x57, 0x57, 0x57,
0x58, 0x58, 0x58,
0x59, 0x59, 0x59,
0x5A, 0x5A, 0x5A,
0x5B, 0x5B, 0x5B,
0x5C, 0x5C, 0x5C,
0x5D, 0x5D, 0x5D,
0x5E, 0x5E, 0x5E,
0x5F, 0x5F, 0x5F,
0x60, 0x60, 0x60,
0x61, 0x61, 0x61,
0x62, 0x62, 0x62,
0x63, 0x63, 0x63,
0x64, 0x64, 0x64,
0x65, 0x65, 0x65,
0x66, 0x66, 0x66,
0x67, 0x67, 0x67,
0x68, 0x68, 0x68,
0x69, 0x69, 0x69,
0x6A, 0x6A, 0x6A,
0x6B, 0x6B, 0x6B,
0x6C, 0x6C, 0x6C,
0x6D, 0x6D, 0x6D,
0x6E, 0x6E, 0x6E,
0x6F, 0x6F, 0x6F,
0x70, 0x70, 0x70,
0x71, 0x71, 0x71,
0x72, 0x72, 0x72,
0x73, 0x73, 0x73,
0x74, 0x74, 0x74,
0x75, 0x75, 0x75,
0x76, 0x76, 0x76,
0x77, 0x77, 0x77,
0x78, 0x78, 0x78,
0x79, 0x79, 0x79,
0x7A, 0x7A, 0x7A,
0x7B, 0x7B, 0x7B,
0x7C, 0x7C, 0x7C,
0x7D, 0x7D, 0x7D,
0x7E, 0x7E, 0x7E,
0x7F, 0x7F, 0x7F,
0x80, 0x80, 0x80,
0x81, 0x81, 0x81,
0x82, 0x82, 0x82,
0x83, 0x83, 0x83,
0x84, 0x84, 0x84,
0x85, 0x85, 0x85,
0x86, 0x86, 0x86,
0x87, 0x87, 0x87,
0x88, 0x88, 0x88,
0x89, 0x89, 0x89,
0x8A, 0x8A, 0x8A,
0x8B, 0x8B, 0x8B,
0x8C, 0x8C, 0x8C,
0x8D, 0x8D, 0x8D,
0x8E, 0x8E, 0x8E,
0x8F, 0x8F, 0x8F,
0x90, 0x90, 0x90,
0x91, 0x91, 0x91,
0x92, 0x92, 0x92,
0x93, 0x93, 0x93,
0x94, 0x94, 0x94,
0x95, 0x95, 0x95,
0x96, 0x96, 0x96,
0x97, 0x97, 0x97,
0x98, 0x98, 0x98,
0x99, 0x99, 0x99,
0x9A, 0x9A, 0x9A,
0x9B, 0x9B, 0x9B,
0x9C, 0x9C, 0x9C,
0x9D, 0x9D, 0x9D,
0x9E, 0x9E, 0x9E,
0x9F, 0x9F, 0x9F,
0xA0, 0xA0, 0xA0,
0xA1, 0xA1, 0xA1,
0xA2, 0xA2, 0xA2,
0xA3, 0xA3, 0xA3,
0xA4, 0xA4, 0xA4,
0xA5, 0xA5, 0xA5,
0xA6, 0xA6, 0xA6,
0xA7, 0xA7, 0xA7,
0xA8, 0xA8, 0xA8,
0xA9, 0xA9, 0xA9,
0xAA, 0xAA, 0xAA,
0xAB, 0xAB, 0xAB,
0xAC, 0xAC, 0xAC,
0xAD, 0xAD, 0xAD,
0xAE, 0xAE, 0xAE,
0xAF, 0xAF, 0xAF,
0xB0, 0xB0, 0xB0,
0xB1, 0xB1, 0xB1,
0xB2, 0xB2, 0xB2,
0xB3, 0xB3, 0xB3,
0xB4, 0xB4, 0xB4,
0xB5, 0xB5, 0xB5,
0xB6, 0xB6, 0xB6,
0xB7, 0xB7, 0xB7,
0xB8, 0xB8, 0xB8,
0xB9, 0xB9, 0xB9,
0xBA, 0xBA, 0xBA,
0xBB, 0xBB, 0xBB,
0xBC, 0xBC, 0xBC,
0xBD, 0xBD, 0xBD,
0xBE, 0xBE, 0xBE,
0xBF, 0xBF, 0xBF,
0xC0, 0xC0, 0xC0,
0xC1, 0xC1, 0xC1,
0xC2, 0xC2, 0xC2,
0xC3, 0xC3, 0xC3,
0xC4, 0xC4, 0xC4,
0xC5, 0xC5, 0xC5,
0xC6, 0xC6, 0xC6,
0xC7, 0xC7, 0xC7,
0xC8, 0xC8, 0xC8,
0xC9, 0xC9, 0xC9,
0xCA, 0xCA, 0xCA,
0xCB, 0xCB, 0xCB,
0xCC, 0xCC, 0xCC,
0xCD, 0xCD, 0xCD,
0xCE, 0xCE, 0xCE,
0xCF, 0xCF, 0xCF,
0xD0, 0xD0, 0xD0,
0xD1, 0xD1, 0xD1,
0xD2, 0xD2, 0xD2,
0xD3, 0xD3, 0xD3,
0xD4, 0xD4, 0xD4,
0xD5, 0xD5, 0xD5,
0xD6, 0xD6, 0xD6,
0xD7, 0xD7, 0xD7,
0xD8, 0xD8, 0xD8,
0xD9, 0xD9, 0xD9,
0xDA, 0xDA, 0xDA,
0xDB, 0xDB, 0xDB,
0xDC, 0xDC, 0xDC,
0xDD, 0xDD, 0xDD,
0xDE, 0xDE, 0xDE,
0xDF, 0xDF, 0xDF,
0xE0, 0xE0, 0xE0,
0xE1, 0xE1, 0xE1,
0xE2, 0xE2, 0xE2,
0xE3, 0xE3, 0xE3,
0xE4, 0xE4, 0xE4,
0xE5, 0xE5, 0xE5,
0xE6, 0xE6, 0xE6,
0xE7, 0xE7, 0xE7,
0xE8, 0xE8, 0xE8,
0xE9, 0xE9, 0xE9,
0xEA, 0xEA, 0xEA,
0xEB, 0xEB, 0xEB,
0xEC, 0xEC, 0xEC,
0xED, 0xED, 0xED,
0xEE, 0xEE, 0xEE,
0xEF, 0xEF, 0xEF,
0xF0, 0xF0, 0xF0,
0xF1, 0xF1, 0xF1,
0xF2, 0xF2, 0xF2,
0xF3, 0xF3, 0xF3,
0xF4, 0xF4, 0xF4,
0xF5, 0xF5, 0xF5,
0xF6, 0xF6, 0xF6,
0xF7, 0xF7, 0xF7,
0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9,
0xFA, 0xFA, 0xFA,
0xFB, 0xFB, 0xFB,
0xFC, 0xFC, 0xFC,
0xFD, 0xFD, 0xFD,
0xFE, 0xFE, 0xFE,
0xFF, 0xFF, 0xFF
};
#endif


#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT)
static unsigned int UI_LUT[] = {
0x00, 0x00, 0x00,
0x01, 0x01, 0x01,
0x02, 0x02, 0x02,
0x02, 0x02, 0x02,
0x03, 0x03, 0x03,
0x04, 0x04, 0x04,
0x05, 0x05, 0x05,
0x05, 0x05, 0x05,
0x06, 0x06, 0x06,
0x07, 0x07, 0x07,
0x08, 0x08, 0x08,
0x08, 0x08, 0x08,
0x09, 0x09, 0x09,
0x0A, 0x0A, 0x0A,
0x0B, 0x0B, 0x0B,
0x0B, 0x0B, 0x0B,
0x0C, 0x0C, 0x0C,
0x0D, 0x0D, 0x0D,
0x0E, 0x0E, 0x0E,
0x0E, 0x0E, 0x0E,
0x0F, 0x0F, 0x0F,
0x10, 0x10, 0x10,
0x11, 0x11, 0x11,
0x11, 0x11, 0x11,
0x12, 0x12, 0x12,
0x13, 0x13, 0x13,
0x14, 0x14, 0x14,
0x14, 0x14, 0x14,
0x15, 0x15, 0x15,
0x16, 0x16, 0x16,
0x17, 0x17, 0x17,
0x17, 0x17, 0x17,
0x18, 0x18, 0x18,
0x19, 0x19, 0x19,
0x1A, 0x1A, 0x1A,
0x1B, 0x1B, 0x1B,
0x1C, 0x1C, 0x1C,
0x1D, 0x1D, 0x1D,
0x1F, 0x1F, 0x1F,
0x20, 0x20, 0x20,
0x21, 0x21, 0x21,
0x22, 0x22, 0x22,
0x23, 0x23, 0x23,
0x24, 0x24, 0x24,
0x26, 0x26, 0x26,
0x27, 0x27, 0x27,
0x28, 0x28, 0x28,
0x29, 0x29, 0x29,
0x2A, 0x2A, 0x2A,
0x2B, 0x2B, 0x2B,
0x2C, 0x2C, 0x2C,
0x2D, 0x2D, 0x2D,
0x2F, 0x2F, 0x2F,
0x30, 0x30, 0x30,
0x31, 0x31, 0x31,
0x32, 0x32, 0x32,
0x33, 0x33, 0x33,
0x34, 0x34, 0x34,
0x35, 0x35, 0x35,
0x36, 0x36, 0x36,
0x38, 0x38, 0x38,
0x39, 0x39, 0x39,
0x3A, 0x3A, 0x3A,
0x3B, 0x3B, 0x3B,
0x3C, 0x3C, 0x3C,
0x3D, 0x3D, 0x3D,
0x3E, 0x3E, 0x3E,
0x3F, 0x3F, 0x3F,
0x41, 0x41, 0x41,
0x42, 0x42, 0x42,
0x43, 0x43, 0x43,
0x44, 0x44, 0x44,
0x45, 0x45, 0x45,
0x46, 0x46, 0x46,
0x47, 0x47, 0x47,
0x48, 0x48, 0x48,
0x4A, 0x4A, 0x4A,
0x4B, 0x4B, 0x4B,
0x4C, 0x4C, 0x4C,
0x4D, 0x4D, 0x4D,
0x4E, 0x4E, 0x4E,
0x4F, 0x4F, 0x4F,
0x50, 0x50, 0x50,
0x51, 0x51, 0x51,
0x53, 0x53, 0x53,
0x54, 0x54, 0x54,
0x55, 0x55, 0x55,
0x56, 0x56, 0x56,
0x57, 0x57, 0x57,
0x58, 0x58, 0x58,
0x59, 0x59, 0x59,
0x5A, 0x5A, 0x5A,
0x5C, 0x5C, 0x5C,
0x5D, 0x5D, 0x5D,
0x5E, 0x5E, 0x5E,
0x5F, 0x5F, 0x5F,
0x60, 0x60, 0x60,
0x61, 0x61, 0x61,
0x62, 0x62, 0x62,
0x63, 0x63, 0x63,
0x65, 0x65, 0x65,
0x66, 0x66, 0x66,
0x67, 0x67, 0x67,
0x68, 0x68, 0x68,
0x69, 0x69, 0x69,
0x6A, 0x6A, 0x6A,
0x6B, 0x6B, 0x6B,
0x6C, 0x6C, 0x6C,
0x6E, 0x6E, 0x6E,
0x6F, 0x6F, 0x6F,
0x70, 0x70, 0x70,
0x71, 0x71, 0x71,
0x72, 0x72, 0x72,
0x73, 0x73, 0x73,
0x74, 0x74, 0x74,
0x75, 0x75, 0x75,
0x77, 0x77, 0x77,
0x78, 0x78, 0x78,
0x79, 0x79, 0x79,
0x7A, 0x7A, 0x7A,
0x7B, 0x7B, 0x7B,
0x7C, 0x7C, 0x7C,
0x7D, 0x7D, 0x7D,
0x7E, 0x7E, 0x7E,
0x80, 0x80, 0x80,
0x81, 0x81, 0x81,
0x82, 0x82, 0x82,
0x83, 0x83, 0x83,
0x84, 0x84, 0x84,
0x85, 0x85, 0x85,
0x86, 0x86, 0x86,
0x87, 0x87, 0x87,
0x89, 0x89, 0x89,
0x8A, 0x8A, 0x8A,
0x8B, 0x8B, 0x8B,
0x8C, 0x8C, 0x8C,
0x8D, 0x8D, 0x8D,
0x8E, 0x8E, 0x8E,
0x8F, 0x8F, 0x8F,
0x90, 0x90, 0x90,
0x92, 0x92, 0x92,
0x93, 0x93, 0x93,
0x94, 0x94, 0x94,
0x95, 0x95, 0x95,
0x96, 0x96, 0x96,
0x97, 0x97, 0x97,
0x98, 0x98, 0x98,
0x99, 0x99, 0x99,
0x9B, 0x9B, 0x9B,
0x9C, 0x9C, 0x9C,
0x9D, 0x9D, 0x9D,
0x9E, 0x9E, 0x9E,
0x9F, 0x9F, 0x9F,
0xA0, 0xA0, 0xA0,
0xA1, 0xA1, 0xA1,
0xA2, 0xA2, 0xA2,
0xA4, 0xA4, 0xA4,
0xA5, 0xA5, 0xA5,
0xA6, 0xA6, 0xA6,
0xA7, 0xA7, 0xA7,
0xA8, 0xA8, 0xA8,
0xA9, 0xA9, 0xA9,
0xAA, 0xAA, 0xAA,
0xAB, 0xAB, 0xAB,
0xAD, 0xAD, 0xAD,
0xAE, 0xAE, 0xAE,
0xAF, 0xAF, 0xAF,
0xB0, 0xB0, 0xB0,
0xB1, 0xB1, 0xB1,
0xB2, 0xB2, 0xB2,
0xB3, 0xB3, 0xB3,
0xB4, 0xB4, 0xB4,
0xB6, 0xB6, 0xB6,
0xB7, 0xB7, 0xB7,
0xB8, 0xB8, 0xB8,
0xB9, 0xB9, 0xB9,
0xBA, 0xBA, 0xBA,
0xBB, 0xBB, 0xBB,
0xBC, 0xBC, 0xBC,
0xBD, 0xBD, 0xBD,
0xBF, 0xBF, 0xBF,
0xC0, 0xC0, 0xC0,
0xC1, 0xC1, 0xC1,
0xC2, 0xC2, 0xC2,
0xC3, 0xC3, 0xC3,
0xC4, 0xC4, 0xC4,
0xC5, 0xC5, 0xC5,
0xC6, 0xC6, 0xC6,
0xC8, 0xC8, 0xC8,
0xC9, 0xC9, 0xC9,
0xCA, 0xCA, 0xCA,
0xCB, 0xCB, 0xCB,
0xCC, 0xCC, 0xCC,
0xCD, 0xCD, 0xCD,
0xCE, 0xCE, 0xCE,
0xCF, 0xCF, 0xCF,
0xD0, 0xD0, 0xD0,
0xD1, 0xD1, 0xD1,
0xD2, 0xD2, 0xD2,
0xD3, 0xD3, 0xD3,
0xD4, 0xD4, 0xD4,
0xD5, 0xD5, 0xD5,
0xD6, 0xD6, 0xD6,
0xD7, 0xD7, 0xD7,
0xD8, 0xD8, 0xD8,
0xD9, 0xD9, 0xD9,
0xDA, 0xDA, 0xDA,
0xDB, 0xDB, 0xDB,
0xDC, 0xDC, 0xDC,
0xDD, 0xDD, 0xDD,
0xDE, 0xDE, 0xDE,
0xDF, 0xDF, 0xDF,
0xE0, 0xE0, 0xE0,
0xE0, 0xE0, 0xE0,
0xE1, 0xE1, 0xE1,
0xE2, 0xE2, 0xE2,
0xE3, 0xE3, 0xE3,
0xE4, 0xE4, 0xE4,
0xE5, 0xE5, 0xE5,
0xE6, 0xE6, 0xE6,
0xE7, 0xE7, 0xE7,
0xE7, 0xE7, 0xE7,
0xE8, 0xE8, 0xE8,
0xE9, 0xE9, 0xE9,
0xEA, 0xEA, 0xEA,
0xEB, 0xEB, 0xEB,
0xEC, 0xEC, 0xEC,
0xEC, 0xEC, 0xEC,
0xED, 0xED, 0xED,
0xEE, 0xEE, 0xEE,
0xEF, 0xEF, 0xEF,
0xEF, 0xEF, 0xEF,
0xF0, 0xF0, 0xF0,
0xF1, 0xF1, 0xF1,
0xF2, 0xF2, 0xF2,
0xF2, 0xF2, 0xF2,
0xF3, 0xF3, 0xF3,
0xF4, 0xF4, 0xF4,
0xF5, 0xF5, 0xF5,
0xF5, 0xF5, 0xF5,
0xF6, 0xF6, 0xF6,
0xF7, 0xF7, 0xF7,
0xF7, 0xF7, 0xF7,
0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9,
0xF9, 0xF9, 0xF9,
0xFA, 0xFA, 0xFA,
0xFB, 0xFB, 0xFB,
0xFB, 0xFB, 0xFB,
0xFC, 0xFC, 0xFC,
0xFD, 0xFD, 0xFD,
0xFD, 0xFD, 0xFD,
0xFE, 0xFE, 0xFE,
0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF,
};
#endif

static unsigned int BYPASS_LUT[] = {
0x00, 0x00, 0x00,
0x01, 0x01, 0x01,
0x02, 0x02, 0x02,
0x03, 0x03, 0x03,
0x04, 0x04, 0x04,
0x05, 0x05, 0x05,
0x06, 0x06, 0x06,
0x07, 0x07, 0x07,
0x08, 0x08, 0x08,
0x09, 0x09, 0x09,
0x0A, 0x0A, 0x0A,
0x0B, 0x0B, 0x0B,
0x0C, 0x0C, 0x0C,
0x0D, 0x0D, 0x0D,
0x0E, 0x0E, 0x0E,
0x0F, 0x0F, 0x0F,
0x10, 0x10, 0x10,
0x11, 0x11, 0x11,
0x12, 0x12, 0x12,
0x13, 0x13, 0x13,
0x14, 0x14, 0x14,
0x15, 0x15, 0x15,
0x16, 0x16, 0x16,
0x17, 0x17, 0x17,
0x18, 0x18, 0x18,
0x19, 0x19, 0x19,
0x1A, 0x1A, 0x1A,
0x1B, 0x1B, 0x1B,
0x1C, 0x1C, 0x1C,
0x1D, 0x1D, 0x1D,
0x1E, 0x1E, 0x1E,
0x1F, 0x1F, 0x1F,
0x20, 0x20, 0x20,
0x21, 0x21, 0x21,
0x22, 0x22, 0x22,
0x23, 0x23, 0x23,
0x24, 0x24, 0x24,
0x25, 0x25, 0x25,
0x26, 0x26, 0x26,
0x27, 0x27, 0x27,
0x28, 0x28, 0x28,
0x29, 0x29, 0x29,
0x2A, 0x2A, 0x2A,
0x2B, 0x2B, 0x2B,
0x2C, 0x2C, 0x2C,
0x2D, 0x2D, 0x2D,
0x2E, 0x2E, 0x2E,
0x2F, 0x2F, 0x2F,
0x30, 0x30, 0x30,
0x31, 0x31, 0x31,
0x32, 0x32, 0x32,
0x33, 0x33, 0x33,
0x34, 0x34, 0x34,
0x35, 0x35, 0x35,
0x36, 0x36, 0x36,
0x37, 0x37, 0x37,
0x38, 0x38, 0x38,
0x39, 0x39, 0x39,
0x3A, 0x3A, 0x3A,
0x3B, 0x3B, 0x3B,
0x3C, 0x3C, 0x3C,
0x3D, 0x3D, 0x3D,
0x3E, 0x3E, 0x3E,
0x3F, 0x3F, 0x3F,
0x40, 0x40, 0x40,
0x41, 0x41, 0x41,
0x42, 0x42, 0x42,
0x43, 0x43, 0x43,
0x44, 0x44, 0x44,
0x45, 0x45, 0x45,
0x46, 0x46, 0x46,
0x47, 0x47, 0x47,
0x48, 0x48, 0x48,
0x49, 0x49, 0x49,
0x4A, 0x4A, 0x4A,
0x4B, 0x4B, 0x4B,
0x4C, 0x4C, 0x4C,
0x4D, 0x4D, 0x4D,
0x4E, 0x4E, 0x4E,
0x4F, 0x4F, 0x4F,
0x50, 0x50, 0x50,
0x51, 0x51, 0x51,
0x52, 0x52, 0x52,
0x53, 0x53, 0x53,
0x54, 0x54, 0x54,
0x55, 0x55, 0x55,
0x56, 0x56, 0x56,
0x57, 0x57, 0x57,
0x58, 0x58, 0x58,
0x59, 0x59, 0x59,
0x5A, 0x5A, 0x5A,
0x5B, 0x5B, 0x5B,
0x5C, 0x5C, 0x5C,
0x5D, 0x5D, 0x5D,
0x5E, 0x5E, 0x5E,
0x5F, 0x5F, 0x5F,
0x60, 0x60, 0x60,
0x61, 0x61, 0x61,
0x62, 0x62, 0x62,
0x63, 0x63, 0x63,
0x64, 0x64, 0x64,
0x65, 0x65, 0x65,
0x66, 0x66, 0x66,
0x67, 0x67, 0x67,
0x68, 0x68, 0x68,
0x69, 0x69, 0x69,
0x6A, 0x6A, 0x6A,
0x6B, 0x6B, 0x6B,
0x6C, 0x6C, 0x6C,
0x6D, 0x6D, 0x6D,
0x6E, 0x6E, 0x6E,
0x6F, 0x6F, 0x6F,
0x70, 0x70, 0x70,
0x71, 0x71, 0x71,
0x72, 0x72, 0x72,
0x73, 0x73, 0x73,
0x74, 0x74, 0x74,
0x75, 0x75, 0x75,
0x76, 0x76, 0x76,
0x77, 0x77, 0x77,
0x78, 0x78, 0x78,
0x79, 0x79, 0x79,
0x7A, 0x7A, 0x7A,
0x7B, 0x7B, 0x7B,
0x7C, 0x7C, 0x7C,
0x7D, 0x7D, 0x7D,
0x7E, 0x7E, 0x7E,
0x7F, 0x7F, 0x7F,
0x80, 0x80, 0x80,
0x81, 0x81, 0x81,
0x82, 0x82, 0x82,
0x83, 0x83, 0x83,
0x84, 0x84, 0x84,
0x85, 0x85, 0x85,
0x86, 0x86, 0x86,
0x87, 0x87, 0x87,
0x88, 0x88, 0x88,
0x89, 0x89, 0x89,
0x8A, 0x8A, 0x8A,
0x8B, 0x8B, 0x8B,
0x8C, 0x8C, 0x8C,
0x8D, 0x8D, 0x8D,
0x8E, 0x8E, 0x8E,
0x8F, 0x8F, 0x8F,
0x90, 0x90, 0x90,
0x91, 0x91, 0x91,
0x92, 0x92, 0x92,
0x93, 0x93, 0x93,
0x94, 0x94, 0x94,
0x95, 0x95, 0x95,
0x96, 0x96, 0x96,
0x97, 0x97, 0x97,
0x98, 0x98, 0x98,
0x99, 0x99, 0x99,
0x9A, 0x9A, 0x9A,
0x9B, 0x9B, 0x9B,
0x9C, 0x9C, 0x9C,
0x9D, 0x9D, 0x9D,
0x9E, 0x9E, 0x9E,
0x9F, 0x9F, 0x9F,
0xA0, 0xA0, 0xA0,
0xA1, 0xA1, 0xA1,
0xA2, 0xA2, 0xA2,
0xA3, 0xA3, 0xA3,
0xA4, 0xA4, 0xA4,
0xA5, 0xA5, 0xA5,
0xA6, 0xA6, 0xA6,
0xA7, 0xA7, 0xA7,
0xA8, 0xA8, 0xA8,
0xA9, 0xA9, 0xA9,
0xAA, 0xAA, 0xAA,
0xAB, 0xAB, 0xAB,
0xAC, 0xAC, 0xAC,
0xAD, 0xAD, 0xAD,
0xAE, 0xAE, 0xAE,
0xAF, 0xAF, 0xAF,
0xB0, 0xB0, 0xB0,
0xB1, 0xB1, 0xB1,
0xB2, 0xB2, 0xB2,
0xB3, 0xB3, 0xB3,
0xB4, 0xB4, 0xB4,
0xB5, 0xB5, 0xB5,
0xB6, 0xB6, 0xB6,
0xB7, 0xB7, 0xB7,
0xB8, 0xB8, 0xB8,
0xB9, 0xB9, 0xB9,
0xBA, 0xBA, 0xBA,
0xBB, 0xBB, 0xBB,
0xBC, 0xBC, 0xBC,
0xBD, 0xBD, 0xBD,
0xBE, 0xBE, 0xBE,
0xBF, 0xBF, 0xBF,
0xC0, 0xC0, 0xC0,
0xC1, 0xC1, 0xC1,
0xC2, 0xC2, 0xC2,
0xC3, 0xC3, 0xC3,
0xC4, 0xC4, 0xC4,
0xC5, 0xC5, 0xC5,
0xC6, 0xC6, 0xC6,
0xC7, 0xC7, 0xC7,
0xC8, 0xC8, 0xC8,
0xC9, 0xC9, 0xC9,
0xCA, 0xCA, 0xCA,
0xCB, 0xCB, 0xCB,
0xCC, 0xCC, 0xCC,
0xCD, 0xCD, 0xCD,
0xCE, 0xCE, 0xCE,
0xCF, 0xCF, 0xCF,
0xD0, 0xD0, 0xD0,
0xD1, 0xD1, 0xD1,
0xD2, 0xD2, 0xD2,
0xD3, 0xD3, 0xD3,
0xD4, 0xD4, 0xD4,
0xD5, 0xD5, 0xD5,
0xD6, 0xD6, 0xD6,
0xD7, 0xD7, 0xD7,
0xD8, 0xD8, 0xD8,
0xD9, 0xD9, 0xD9,
0xDA, 0xDA, 0xDA,
0xDB, 0xDB, 0xDB,
0xDC, 0xDC, 0xDC,
0xDD, 0xDD, 0xDD,
0xDE, 0xDE, 0xDE,
0xDF, 0xDF, 0xDF,
0xE0, 0xE0, 0xE0,
0xE1, 0xE1, 0xE1,
0xE2, 0xE2, 0xE2,
0xE3, 0xE3, 0xE3,
0xE4, 0xE4, 0xE4,
0xE5, 0xE5, 0xE5,
0xE6, 0xE6, 0xE6,
0xE7, 0xE7, 0xE7,
0xE8, 0xE8, 0xE8,
0xE9, 0xE9, 0xE9,
0xEA, 0xEA, 0xEA,
0xEB, 0xEB, 0xEB,
0xEC, 0xEC, 0xEC,
0xED, 0xED, 0xED,
0xEE, 0xEE, 0xEE,
0xEF, 0xEF, 0xEF,
0xF0, 0xF0, 0xF0,
0xF1, 0xF1, 0xF1,
0xF2, 0xF2, 0xF2,
0xF3, 0xF3, 0xF3,
0xF4, 0xF4, 0xF4,
0xF5, 0xF5, 0xF5,
0xF6, 0xF6, 0xF6,
0xF7, 0xF7, 0xF7,
0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9,
0xFA, 0xFA, 0xFA,
0xFB, 0xFB, 0xFB,
0xFC, 0xFC, 0xFC,
0xFD, 0xFD, 0xFD,
0xFE, 0xFE, 0xFE,
0xFF, 0xFF, 0xFF,
};

static unsigned int BROWSER_TONE1_LUT[] = {
0x00,	0x00,	0x00,
0x01,	0x01,	0x01,
0x01,	0x01,	0x01,
0x02,	0x02,	0x02,
0x03,	0x03,	0x03,
0x03,	0x04,	0x04,
0x04,	0x04,	0x04,
0x05,	0x05,	0x05,
0x05,	0x06,	0x06,
0x06,	0x06,	0x07,
0x07,	0x07,	0x07,
0x08,	0x08,	0x08,
0x08,	0x09,	0x09,
0x09,	0x09,	0x0A,
0x0A,	0x0A,	0x0A,
0x0A,	0x0B,	0x0B,
0x0B,	0x0B,	0x0C,
0x0C,	0x0C,	0x0D,
0x0C,	0x0D,	0x0D,
0x0D,	0x0E,	0x0E,
0x0E,	0x0E,	0x0F,
0x0E,	0x0F,	0x0F,
0x0F,	0x10,	0x10,
0x10,	0x11,	0x11,
0x10,	0x11,	0x12,
0x11,	0x12,	0x12,
0x12,	0x13,	0x13,
0x13,	0x13,	0x14,
0x13,	0x14,	0x15,
0x14,	0x15,	0x15,
0x15,	0x16,	0x16,
0x15,	0x16,	0x17,
0x16,	0x17,	0x18,
0x17,	0x18,	0x18,
0x17,	0x18,	0x19,
0x18,	0x19,	0x1A,
0x19,	0x1A,	0x1B,
0x19,	0x1B,	0x1B,
0x1A,	0x1B,	0x1C,
0x1B,	0x1C,	0x1D,
0x1B,	0x1D,	0x1D,
0x1C,	0x1D,	0x1E,
0x1D,	0x1E,	0x1F,
0x1E,	0x1F,	0x20,
0x1E,	0x20,	0x20,
0x1F,	0x20,	0x21,
0x20,	0x21,	0x22,
0x20,	0x22,	0x23,
0x21,	0x22,	0x23,
0x22,	0x23,	0x24,
0x22,	0x24,	0x25,
0x23,	0x25,	0x26,
0x24,	0x25,	0x26,
0x24,	0x26,	0x27,
0x25,	0x27,	0x28,
0x26,	0x27,	0x29,
0x26,	0x28,	0x29,
0x27,	0x29,	0x2A,
0x28,	0x2A,	0x2B,
0x28,	0x2A,	0x2B,
0x29,	0x2B,	0x2C,
0x2A,	0x2C,	0x2D,
0x2B,	0x2C,	0x2E,
0x2B,	0x2D,	0x2E,
0x2C,	0x2E,	0x2F,
0x2D,	0x2F,	0x30,
0x2D,	0x2F,	0x31,
0x2E,	0x30,	0x31,
0x2F,	0x31,	0x32,
0x2F,	0x32,	0x33,
0x30,	0x32,	0x34,
0x31,	0x33,	0x34,
0x31,	0x34,	0x35,
0x32,	0x34,	0x36,
0x33,	0x35,	0x37,
0x33,	0x36,	0x37,
0x34,	0x37,	0x38,
0x35,	0x37,	0x39,
0x36,	0x38,	0x3A,
0x36,	0x39,	0x3A,
0x37,	0x39,	0x3B,
0x38,	0x3A,	0x3C,
0x38,	0x3B,	0x3C,
0x39,	0x3C,	0x3D,
0x3A,	0x3C,	0x3E,
0x3A,	0x3D,	0x3F,
0x3B,	0x3E,	0x3F,
0x3C,	0x3E,	0x40,
0x3C,	0x3F,	0x41,
0x3D,	0x40,	0x42,
0x3E,	0x41,	0x42,
0x3E,	0x41,	0x43,
0x3F,	0x42,	0x44,
0x40,	0x43,	0x45,
0x41,	0x43,	0x45,
0x41,	0x44,	0x46,
0x42,	0x45,	0x47,
0x43,	0x46,	0x48,
0x43,	0x46,	0x48,
0x44,	0x47,	0x49,
0x45,	0x48,	0x4A,
0x45,	0x48,	0x4A,
0x46,	0x49,	0x4B,
0x47,	0x4A,	0x4C,
0x47,	0x4B,	0x4D,
0x48,	0x4B,	0x4D,
0x49,	0x4C,	0x4E,
0x49,	0x4D,	0x4F,
0x4A,	0x4E,	0x50,
0x4B,	0x4E,	0x50,
0x4B,	0x4F,	0x51,
0x4C,	0x50,	0x52,
0x4D,	0x50,	0x53,
0x4E,	0x51,	0x53,
0x4E,	0x52,	0x54,
0x4F,	0x53,	0x55,
0x50,	0x53,	0x56,
0x50,	0x54,	0x56,
0x51,	0x55,	0x57,
0x52,	0x55,	0x58,
0x52,	0x56,	0x58,
0x53,	0x57,	0x59,
0x54,	0x58,	0x5A,
0x54,	0x58,	0x5B,
0x55,	0x59,	0x5B,
0x56,	0x5A,	0x5C,
0x56,	0x5A,	0x5D,
0x57,	0x5B,	0x5E,
0x58,	0x5C,	0x5E,
0x59,	0x5D,	0x5F,
0x59,	0x5D,	0x60,
0x5A,	0x5E,	0x61,
0x5B,	0x5F,	0x61,
0x5B,	0x5F,	0x62,
0x5C,	0x60,	0x63,
0x5D,	0x61,	0x64,
0x5D,	0x62,	0x64,
0x5E,	0x62,	0x65,
0x5F,	0x63,	0x66,
0x5F,	0x64,	0x66,
0x60,	0x64,	0x67,
0x61,	0x65,	0x68,
0x61,	0x66,	0x69,
0x62,	0x67,	0x69,
0x63,	0x67,	0x6A,
0x64,	0x68,	0x6B,
0x64,	0x69,	0x6C,
0x65,	0x69,	0x6C,
0x66,	0x6A,	0x6D,
0x66,	0x6B,	0x6E,
0x67,	0x6C,	0x6F,
0x68,	0x6C,	0x6F,
0x68,	0x6D,	0x70,
0x69,	0x6E,	0x71,
0x6A,	0x6F,	0x72,
0x6A,	0x6F,	0x72,
0x6B,	0x70,	0x73,
0x6C,	0x71,	0x74,
0x6C,	0x71,	0x74,
0x6D,	0x72,	0x75,
0x6E,	0x73,	0x76,
0x6E,	0x74,	0x77,
0x6F,	0x74,	0x77,
0x70,	0x75,	0x78,
0x71,	0x76,	0x79,
0x71,	0x76,	0x7A,
0x72,	0x77,	0x7A,
0x73,	0x78,	0x7B,
0x73,	0x79,	0x7C,
0x74,	0x79,	0x7D,
0x75,	0x7A,	0x7D,
0x75,	0x7B,	0x7E,
0x76,	0x7B,	0x7F,
0x77,	0x7C,	0x80,
0x77,	0x7D,	0x80,
0x78,	0x7E,	0x81,
0x79,	0x7E,	0x82,
0x79,	0x7F,	0x82,
0x7A,	0x80,	0x83,
0x7B,	0x80,	0x84,
0x7C,	0x81,	0x85,
0x7C,	0x82,	0x85,
0x7D,	0x83,	0x86,
0x7E,	0x83,	0x87,
0x7E,	0x84,	0x88,
0x7F,	0x85,	0x88,
0x80,	0x85,	0x89,
0x80,	0x86,	0x8A,
0x81,	0x87,	0x8B,
0x82,	0x88,	0x8B,
0x82,	0x88,	0x8C,
0x83,	0x89,	0x8D,
0x84,	0x8A,	0x8E,
0x84,	0x8B,	0x8E,
0x85,	0x8B,	0x8F,
0x86,	0x8C,	0x90,
0x87,	0x8D,	0x91,
0x87,	0x8D,	0x91,
0x88,	0x8E,	0x92,
0x89,	0x8F,	0x93,
0x89,	0x90,	0x93,
0x8A,	0x90,	0x94,
0x8B,	0x91,	0x95,
0x8B,	0x92,	0x96,
0x8C,	0x92,	0x96,
0x8D,	0x93,	0x97,
0x8D,	0x94,	0x98,
0x8E,	0x95,	0x99,
0x8F,	0x95,	0x99,
0x8F,	0x96,	0x9A,
0x90,	0x97,	0x9B,
0x91,	0x97,	0x9C,
0x91,	0x98,	0x9C,
0x92,	0x99,	0x9D,
0x93,	0x9A,	0x9E,
0x94,	0x9A,	0x9F,
0x94,	0x9B,	0x9F,
0x95,	0x9C,	0xA0,
0x96,	0x9C,	0xA1,
0x96,	0x9D,	0xA1,
0x97,	0x9E,	0xA2,
0x98,	0x9F,	0xA3,
0x98,	0x9F,	0xA4,
0x99,	0xA0,	0xA4,
0x9A,	0xA1,	0xA5,
0x9A,	0xA1,	0xA6,
0x9B,	0xA2,	0xA7,
0x9C,	0xA3,	0xA7,
0x9C,	0xA4,	0xA8,
0x9D,	0xA4,	0xA9,
0x9E,	0xA5,	0xAA,
0x9F,	0xA6,	0xAA,
0x9F,	0xA6,	0xAB,
0xA0,	0xA7,	0xAC,
0xA1,	0xA8,	0xAD,
0xA1,	0xA9,	0xAD,
0xA2,	0xA9,	0xAE,
0xA3,	0xAA,	0xAF,
0xA3,	0xAB,	0xAF,
0xA4,	0xAC,	0xB0,
0xA5,	0xAC,	0xB1,
0xA5,	0xAD,	0xB2,
0xA6,	0xAE,	0xB2,
0xA7,	0xAE,	0xB3,
0xA7,	0xAF,	0xB4,
0xA8,	0xB0,	0xB5,
0xA9,	0xB1,	0xB5,
0xAA,	0xB1,	0xB6,
0xAA,	0xB2,	0xB7,
0xAB,	0xB3,	0xB8,
0xAC,	0xB3,	0xB8,
0xAC,	0xB4,	0xB9,
0xAD,	0xB5,	0xBA,
0xAE,	0xB6,	0xBB,
0xAE,	0xB6,	0xBB,
0xAF,	0xB7,	0xBC,
};

static unsigned int BROWSER_TONE2_LUT[] = {
0x00,	0x00,	0x00,
0x01,	0x01,	0x01,
0x01,	0x01,	0x01,
0x02,	0x02,	0x02,
0x03,	0x03,	0x03,
0x03,	0x03,	0x03,
0x04,	0x04,	0x04,
0x04,	0x05,	0x05,
0x05,	0x05,	0x06,
0x06,	0x06,	0x06,
0x06,	0x07,	0x07,
0x07,	0x07,	0x08,
0x08,	0x08,	0x08,
0x08,	0x09,	0x09,
0x09,	0x09,	0x0A,
0x09,	0x0A,	0x0A,
0x0A,	0x0B,	0x0B,
0x0B,	0x0B,	0x0C,
0x0B,	0x0C,	0x0D,
0x0C,	0x0D,	0x0D,
0x0D,	0x0D,	0x0E,
0x0D,	0x0E,	0x0F,
0x0E,	0x0E,	0x0F,
0x0E,	0x0F,	0x10,
0x0F,	0x10,	0x11,
0x10,	0x10,	0x11,
0x10,	0x11,	0x12,
0x11,	0x12,	0x13,
0x12,	0x12,	0x14,
0x12,	0x13,	0x14,
0x13,	0x14,	0x15,
0x13,	0x14,	0x16,
0x14,	0x15,	0x16,
0x15,	0x16,	0x17,
0x15,	0x16,	0x18,
0x16,	0x17,	0x18,
0x17,	0x18,	0x19,
0x17,	0x18,	0x1A,
0x18,	0x19,	0x1B,
0x18,	0x1A,	0x1B,
0x19,	0x1A,	0x1C,
0x1A,	0x1B,	0x1D,
0x1A,	0x1C,	0x1D,
0x1B,	0x1C,	0x1E,
0x1C,	0x1D,	0x1F,
0x1C,	0x1E,	0x1F,
0x1D,	0x1E,	0x20,
0x1D,	0x1F,	0x21,
0x1E,	0x20,	0x22,
0x1F,	0x20,	0x22,
0x1F,	0x21,	0x23,
0x20,	0x22,	0x24,
0x21,	0x22,	0x24,
0x21,	0x23,	0x25,
0x22,	0x24,	0x26,
0x23,	0x24,	0x26,
0x23,	0x25,	0x27,
0x24,	0x26,	0x28,
0x24,	0x26,	0x28,
0x25,	0x27,	0x29,
0x26,	0x28,	0x2A,
0x26,	0x28,	0x2B,
0x27,	0x29,	0x2B,
0x28,	0x2A,	0x2C,
0x28,	0x2A,	0x2D,
0x29,	0x2B,	0x2D,
0x29,	0x2B,	0x2E,
0x2A,	0x2C,	0x2F,
0x2B,	0x2D,	0x2F,
0x2B,	0x2D,	0x30,
0x2C,	0x2E,	0x31,
0x2D,	0x2F,	0x32,
0x2D,	0x2F,	0x32,
0x2E,	0x30,	0x33,
0x2E,	0x31,	0x34,
0x2F,	0x31,	0x34,
0x30,	0x32,	0x35,
0x30,	0x33,	0x36,
0x31,	0x33,	0x36,
0x32,	0x34,	0x37,
0x32,	0x35,	0x38,
0x33,	0x35,	0x39,
0x33,	0x36,	0x39,
0x34,	0x37,	0x3A,
0x35,	0x37,	0x3B,
0x35,	0x38,	0x3B,
0x36,	0x39,	0x3C,
0x37,	0x39,	0x3D,
0x37,	0x3A,	0x3D,
0x38,	0x3B,	0x3E,
0x38,	0x3B,	0x3F,
0x39,	0x3C,	0x40,
0x3A,	0x3D,	0x40,
0x3A,	0x3D,	0x41,
0x3B,	0x3E,	0x42,
0x3C,	0x3F,	0x42,
0x3C,	0x3F,	0x43,
0x3D,	0x40,	0x44,
0x3D,	0x41,	0x44,
0x3E,	0x41,	0x45,
0x3F,	0x42,	0x46,
0x3F,	0x43,	0x47,
0x40,	0x43,	0x47,
0x41,	0x44,	0x48,
0x41,	0x45,	0x49,
0x42,	0x45,	0x49,
0x43,	0x46,	0x4A,
0x43,	0x46,	0x4B,
0x44,	0x47,	0x4B,
0x44,	0x48,	0x4C,
0x45,	0x48,	0x4D,
0x46,	0x49,	0x4D,
0x46,	0x4A,	0x4E,
0x47,	0x4A,	0x4F,
0x48,	0x4B,	0x50,
0x48,	0x4C,	0x50,
0x49,	0x4C,	0x51,
0x49,	0x4D,	0x52,
0x4A,	0x4E,	0x52,
0x4B,	0x4E,	0x53,
0x4B,	0x4F,	0x54,
0x4C,	0x50,	0x54,
0x4D,	0x50,	0x55,
0x4D,	0x51,	0x56,
0x4E,	0x52,	0x57,
0x4E,	0x52,	0x57,
0x4F,	0x53,	0x58,
0x50,	0x54,	0x59,
0x50,	0x54,	0x59,
0x51,	0x55,	0x5A,
0x52,	0x56,	0x5B,
0x52,	0x56,	0x5B,
0x53,	0x57,	0x5C,
0x53,	0x58,	0x5D,
0x54,	0x58,	0x5E,
0x55,	0x59,	0x5E,
0x55,	0x5A,	0x5F,
0x56,	0x5A,	0x60,
0x57,	0x5B,	0x60,
0x57,	0x5C,	0x61,
0x58,	0x5C,	0x62,
0x58,	0x5D,	0x62,
0x59,	0x5E,	0x63,
0x5A,	0x5E,	0x64,
0x5A,	0x5F,	0x65,
0x5B,	0x60,	0x65,
0x5C,	0x60,	0x66,
0x5C,	0x61,	0x67,
0x5D,	0x62,	0x67,
0x5D,	0x62,	0x68,
0x5E,	0x63,	0x69,
0x5F,	0x63,	0x69,
0x5F,	0x64,	0x6A,
0x60,	0x65,	0x6B,
0x61,	0x65,	0x6B,
0x61,	0x66,	0x6C,
0x62,	0x67,	0x6D,
0x63,	0x67,	0x6E,
0x63,	0x68,	0x6E,
0x64,	0x69,	0x6F,
0x64,	0x69,	0x70,
0x65,	0x6A,	0x70,
0x66,	0x6B,	0x71,
0x66,	0x6B,	0x72,
0x67,	0x6C,	0x72,
0x68,	0x6D,	0x73,
0x68,	0x6D,	0x74,
0x69,	0x6E,	0x75,
0x69,	0x6F,	0x75,
0x6A,	0x6F,	0x76,
0x6B,	0x70,	0x77,
0x6B,	0x71,	0x77,
0x6C,	0x71,	0x78,
0x6D,	0x72,	0x79,
0x6D,	0x73,	0x79,
0x6E,	0x73,	0x7A,
0x6E,	0x74,	0x7B,
0x6F,	0x75,	0x7C,
0x70,	0x75,	0x7C,
0x70,	0x76,	0x7D,
0x71,	0x77,	0x7E,
0x72,	0x77,	0x7E,
0x72,	0x78,	0x7F,
0x73,	0x79,	0x80,
0x73,	0x79,	0x80,
0x74,	0x7A,	0x81,
0x75,	0x7B,	0x82,
0x75,	0x7B,	0x83,
0x76,	0x7C,	0x83,
0x77,	0x7D,	0x84,
0x77,	0x7D,	0x85,
0x78,	0x7E,	0x85,
0x78,	0x7E,	0x86,
0x79,	0x7F,	0x87,
0x7A,	0x80,	0x87,
0x7A,	0x80,	0x88,
0x7B,	0x81,	0x89,
0x7C,	0x82,	0x8A,
0x7C,	0x82,	0x8A,
0x7D,	0x83,	0x8B,
0x7D,	0x84,	0x8C,
0x7E,	0x84,	0x8C,
0x7F,	0x85,	0x8D,
0x7F,	0x86,	0x8E,
0x80,	0x86,	0x8E,
0x81,	0x87,	0x8F,
0x81,	0x88,	0x90,
0x82,	0x88,	0x90,
0x83,	0x89,	0x91,
0x83,	0x8A,	0x92,
0x84,	0x8A,	0x93,
0x84,	0x8B,	0x93,
0x85,	0x8C,	0x94,
0x86,	0x8C,	0x95,
0x86,	0x8D,	0x95,
0x87,	0x8E,	0x96,
0x88,	0x8E,	0x97,
0x88,	0x8F,	0x97,
0x89,	0x90,	0x98,
0x89,	0x90,	0x99,
0x8A,	0x91,	0x9A,
0x8B,	0x92,	0x9A,
0x8B,	0x92,	0x9B,
0x8C,	0x93,	0x9C,
0x8D,	0x94,	0x9C,
0x8D,	0x94,	0x9D,
0x8E,	0x95,	0x9E,
0x8E,	0x96,	0x9E,
0x8F,	0x96,	0x9F,
0x90,	0x97,	0xA0,
0x90,	0x98,	0xA1,
0x91,	0x98,	0xA1,
0x92,	0x99,	0xA2,
0x92,	0x9A,	0xA3,
0x93,	0x9A,	0xA3,
0x93,	0x9B,	0xA4,
0x94,	0x9B,	0xA5,
0x95,	0x9C,	0xA5,
0x95,	0x9D,	0xA6,
0x96,	0x9D,	0xA7,
0x97,	0x9E,	0xA8,
0x97,	0x9F,	0xA8,
0x98,	0x9F,	0xA9,
0x98,	0xA0,	0xAA,
0x99,	0xA1,	0xAA,
0x9A,	0xA1,	0xAB,
0x9A,	0xA2,	0xAC,
0x9B,	0xA3,	0xAC,
0x9C,	0xA3,	0xAD,
0x9C,	0xA4,	0xAE,
0x9D,	0xA5,	0xAF,
0x9D,	0xA5,	0xAF,
0x9E,	0xA6,	0xB0,
0x9F,	0xA7,	0xB1,
0x9F,	0xA7,	0xB1,
0xA0,	0xA8,	0xB2,
};

static unsigned int BROWSER_TONE3_LUT[] = {
0x00,	0x00,	0x00,
0x01,	0x01,	0x01,
0x01,	0x01,	0x01,
0x02,	0x02,	0x02,
0x02,	0x02,	0x03,
0x03,	0x03,	0x03,
0x03,	0x04,	0x04,
0x04,	0x04,	0x04,
0x05,	0x05,	0x05,
0x05,	0x05,	0x06,
0x06,	0x06,	0x06,
0x06,	0x07,	0x07,
0x07,	0x07,	0x08,
0x07,	0x08,	0x08,
0x08,	0x08,	0x09,
0x09,	0x09,	0x0A,
0x09,	0x0A,	0x0A,
0x0A,	0x0A,	0x0B,
0x0A,	0x0B,	0x0C,
0x0B,	0x0B,	0x0C,
0x0B,	0x0C,	0x0D,
0x0C,	0x0D,	0x0D,
0x0D,	0x0D,	0x0E,
0x0D,	0x0E,	0x0F,
0x0E,	0x0E,	0x0F,
0x0E,	0x0F,	0x10,
0x0F,	0x10,	0x11,
0x0F,	0x10,	0x11,
0x10,	0x11,	0x12,
0x10,	0x11,	0x13,
0x11,	0x12,	0x13,
0x12,	0x13,	0x14,
0x12,	0x13,	0x14,
0x13,	0x14,	0x15,
0x13,	0x14,	0x16,
0x14,	0x15,	0x16,
0x14,	0x16,	0x17,
0x15,	0x16,	0x18,
0x16,	0x17,	0x18,
0x16,	0x17,	0x19,
0x17,	0x18,	0x1A,
0x17,	0x19,	0x1A,
0x18,	0x19,	0x1B,
0x18,	0x1A,	0x1B,
0x19,	0x1A,	0x1C,
0x1A,	0x1B,	0x1D,
0x1A,	0x1C,	0x1D,
0x1B,	0x1C,	0x1E,
0x1B,	0x1D,	0x1F,
0x1C,	0x1D,	0x1F,
0x1C,	0x1E,	0x20,
0x1D,	0x1F,	0x21,
0x1E,	0x1F,	0x21,
0x1E,	0x20,	0x22,
0x1F,	0x20,	0x23,
0x1F,	0x21,	0x23,
0x20,	0x22,	0x24,
0x20,	0x22,	0x24,
0x21,	0x23,	0x25,
0x22,	0x23,	0x26,
0x22,	0x24,	0x26,
0x23,	0x25,	0x27,
0x23,	0x25,	0x28,
0x24,	0x26,	0x28,
0x24,	0x26,	0x29,
0x25,	0x27,	0x2A,
0x26,	0x28,	0x2A,
0x26,	0x28,	0x2B,
0x27,	0x29,	0x2B,
0x27,	0x29,	0x2C,
0x28,	0x2A,	0x2D,
0x28,	0x2B,	0x2D,
0x29,	0x2B,	0x2E,
0x2A,	0x2C,	0x2F,
0x2A,	0x2C,	0x2F,
0x2B,	0x2D,	0x30,
0x2B,	0x2E,	0x31,
0x2C,	0x2E,	0x31,
0x2C,	0x2F,	0x32,
0x2D,	0x2F,	0x32,
0x2D,	0x30,	0x33,
0x2E,	0x31,	0x34,
0x2F,	0x31,	0x34,
0x2F,	0x32,	0x35,
0x30,	0x32,	0x36,
0x30,	0x33,	0x36,
0x31,	0x34,	0x37,
0x31,	0x34,	0x38,
0x32,	0x35,	0x38,
0x33,	0x35,	0x39,
0x33,	0x36,	0x3A,
0x34,	0x37,	0x3A,
0x34,	0x37,	0x3B,
0x35,	0x38,	0x3B,
0x35,	0x38,	0x3C,
0x36,	0x39,	0x3D,
0x37,	0x3A,	0x3D,
0x37,	0x3A,	0x3E,
0x38,	0x3B,	0x3F,
0x38,	0x3B,	0x3F,
0x39,	0x3C,	0x40,
0x39,	0x3D,	0x41,
0x3A,	0x3D,	0x41,
0x3B,	0x3E,	0x42,
0x3B,	0x3E,	0x42,
0x3C,	0x3F,	0x43,
0x3C,	0x40,	0x44,
0x3D,	0x40,	0x44,
0x3D,	0x41,	0x45,
0x3E,	0x41,	0x46,
0x3F,	0x42,	0x46,
0x3F,	0x43,	0x47,
0x40,	0x43,	0x48,
0x40,	0x44,	0x48,
0x41,	0x44,	0x49,
0x41,	0x45,	0x4A,
0x42,	0x46,	0x4A,
0x43,	0x46,	0x4B,
0x43,	0x47,	0x4B,
0x44,	0x47,	0x4C,
0x44,	0x48,	0x4D,
0x45,	0x49,	0x4D,
0x45,	0x49,	0x4E,
0x46,	0x4A,	0x4F,
0x47,	0x4A,	0x4F,
0x47,	0x4B,	0x50,
0x48,	0x4C,	0x51,
0x48,	0x4C,	0x51,
0x49,	0x4D,	0x52,
0x49,	0x4D,	0x52,
0x4A,	0x4E,	0x53,
0x4A,	0x4F,	0x54,
0x4B,	0x4F,	0x54,
0x4C,	0x50,	0x55,
0x4C,	0x50,	0x56,
0x4D,	0x51,	0x56,
0x4D,	0x52,	0x57,
0x4E,	0x52,	0x58,
0x4E,	0x53,	0x58,
0x4F,	0x53,	0x59,
0x50,	0x54,	0x59,
0x50,	0x55,	0x5A,
0x51,	0x55,	0x5B,
0x51,	0x56,	0x5B,
0x52,	0x56,	0x5C,
0x52,	0x57,	0x5D,
0x53,	0x58,	0x5D,
0x54,	0x58,	0x5E,
0x54,	0x59,	0x5F,
0x55,	0x59,	0x5F,
0x55,	0x5A,	0x60,
0x56,	0x5B,	0x61,
0x56,	0x5B,	0x61,
0x57,	0x5C,	0x62,
0x58,	0x5C,	0x62,
0x58,	0x5D,	0x63,
0x59,	0x5E,	0x64,
0x59,	0x5E,	0x64,
0x5A,	0x5F,	0x65,
0x5A,	0x5F,	0x66,
0x5B,	0x60,	0x66,
0x5C,	0x61,	0x67,
0x5C,	0x61,	0x68,
0x5D,	0x62,	0x68,
0x5D,	0x62,	0x69,
0x5E,	0x63,	0x69,
0x5E,	0x64,	0x6A,
0x5F,	0x64,	0x6B,
0x60,	0x65,	0x6B,
0x60,	0x65,	0x6C,
0x61,	0x66,	0x6D,
0x61,	0x67,	0x6D,
0x62,	0x67,	0x6E,
0x62,	0x68,	0x6F,
0x63,	0x68,	0x6F,
0x64,	0x69,	0x70,
0x64,	0x6A,	0x71,
0x65,	0x6A,	0x71,
0x65,	0x6B,	0x72,
0x66,	0x6B,	0x72,
0x66,	0x6C,	0x73,
0x67,	0x6D,	0x74,
0x67,	0x6D,	0x74,
0x68,	0x6E,	0x75,
0x69,	0x6E,	0x76,
0x69,	0x6F,	0x76,
0x6A,	0x70,	0x77,
0x6A,	0x70,	0x78,
0x6B,	0x71,	0x78,
0x6B,	0x71,	0x79,
0x6C,	0x72,	0x79,
0x6D,	0x73,	0x7A,
0x6D,	0x73,	0x7B,
0x6E,	0x74,	0x7B,
0x6E,	0x74,	0x7C,
0x6F,	0x75,	0x7D,
0x6F,	0x76,	0x7D,
0x70,	0x76,	0x7E,
0x71,	0x77,	0x7F,
0x71,	0x77,	0x7F,
0x72,	0x78,	0x80,
0x72,	0x79,	0x80,
0x73,	0x79,	0x81,
0x73,	0x7A,	0x82,
0x74,	0x7A,	0x82,
0x75,	0x7B,	0x83,
0x75,	0x7C,	0x84,
0x76,	0x7C,	0x84,
0x76,	0x7D,	0x85,
0x77,	0x7D,	0x86,
0x77,	0x7E,	0x86,
0x78,	0x7F,	0x87,
0x79,	0x7F,	0x88,
0x79,	0x80,	0x88,
0x7A,	0x80,	0x89,
0x7A,	0x81,	0x89,
0x7B,	0x82,	0x8A,
0x7B,	0x82,	0x8B,
0x7C,	0x83,	0x8B,
0x7D,	0x83,	0x8C,
0x7D,	0x84,	0x8D,
0x7E,	0x85,	0x8D,
0x7E,	0x85,	0x8E,
0x7F,	0x86,	0x8F,
0x7F,	0x86,	0x8F,
0x80,	0x87,	0x90,
0x81,	0x88,	0x90,
0x81,	0x88,	0x91,
0x82,	0x89,	0x92,
0x82,	0x89,	0x92,
0x83,	0x8A,	0x93,
0x83,	0x8B,	0x94,
0x84,	0x8B,	0x94,
0x84,	0x8C,	0x95,
0x85,	0x8C,	0x96,
0x86,	0x8D,	0x96,
0x86,	0x8E,	0x97,
0x87,	0x8E,	0x97,
0x87,	0x8F,	0x98,
0x88,	0x8F,	0x99,
0x88,	0x90,	0x99,
0x89,	0x91,	0x9A,
0x8A,	0x91,	0x9B,
0x8A,	0x92,	0x9B,
0x8B,	0x92,	0x9C,
0x8B,	0x93,	0x9D,
0x8C,	0x94,	0x9D,
0x8C,	0x94,	0x9E,
0x8D,	0x95,	0x9F,
0x8E,	0x95,	0x9F,
0x8E,	0x96,	0xA0,
0x8F,	0x97,	0xA0,
0x8F,	0x97,	0xA1,
0x90,	0x98,	0xA2,
0x90,	0x98,	0xA2,
0x91,	0x99,	0xA3,
};

static unsigned int DMB_TUNE[] = {
	0x00, 0x00, 0x00,
	0x01, 0x01, 0x01,
	0x02, 0x02, 0x02,
	0x02, 0x02, 0x02,
	0x03, 0x03, 0x03,
	0x04, 0x04, 0x04,
	0x05, 0x05, 0x05,
	0x05, 0x05, 0x05,
	0x06, 0x06, 0x06,
	0x07, 0x07, 0x07,
	0x08, 0x08, 0x08,
	0x08, 0x08, 0x08,
	0x09, 0x09, 0x09,
	0x0A, 0x0A, 0x0A,
	0x0B, 0x0B, 0x0B,
	0x0B, 0x0B, 0x0B,
	0x0C, 0x0C, 0x0C,
	0x0D, 0x0D, 0x0D,
	0x0E, 0x0E, 0x0E,
	0x0E, 0x0E, 0x0E,
	0x0F, 0x0F, 0x0F,
	0x10, 0x10, 0x10,
	0x11, 0x11, 0x11,
	0x11, 0x11, 0x11,
	0x12, 0x12, 0x12,
	0x13, 0x13, 0x13,
	0x14, 0x14, 0x14,
	0x14, 0x14, 0x14,
	0x15, 0x15, 0x15,
	0x16, 0x16, 0x16,
	0x17, 0x17, 0x17,
	0x17, 0x17, 0x17,
	0x18, 0x18, 0x18,
	0x19, 0x19, 0x19,
	0x1A, 0x1A, 0x1A,
	0x1B, 0x1B, 0x1B,
	0x1C, 0x1C, 0x1C,
	0x1D, 0x1D, 0x1D,
	0x1F, 0x1F, 0x1F,
	0x20, 0x20, 0x20,
	0x21, 0x21, 0x21,
	0x22, 0x22, 0x22,
	0x23, 0x23, 0x23,
	0x24, 0x24, 0x24,
	0x26, 0x26, 0x26,
	0x27, 0x27, 0x27,
	0x28, 0x28, 0x28,
	0x29, 0x29, 0x29,
	0x2A, 0x2A, 0x2A,
	0x2B, 0x2B, 0x2B,
	0x2C, 0x2C, 0x2C,
	0x2D, 0x2D, 0x2D,
	0x2F, 0x2F, 0x2F,
	0x30, 0x30, 0x30,
	0x31, 0x31, 0x31,
	0x32, 0x32, 0x32,
	0x33, 0x33, 0x33,
	0x34, 0x34, 0x34,
	0x35, 0x35, 0x35,
	0x36, 0x36, 0x36,
	0x38, 0x38, 0x38,
	0x39, 0x39, 0x39,
	0x3A, 0x3A, 0x3A,
	0x3B, 0x3B, 0x3B,
	0x3C, 0x3C, 0x3C,
	0x3D, 0x3D, 0x3D,
	0x3E, 0x3E, 0x3E,
	0x3F, 0x3F, 0x3F,
	0x41, 0x41, 0x41,
	0x42, 0x42, 0x42,
	0x43, 0x43, 0x43,
	0x44, 0x44, 0x44,
	0x45, 0x45, 0x45,
	0x46, 0x46, 0x46,
	0x47, 0x47, 0x47,
	0x48, 0x48, 0x48,
	0x4A, 0x4A, 0x4A,
	0x4B, 0x4B, 0x4B,
	0x4C, 0x4C, 0x4C,
	0x4D, 0x4D, 0x4D,
	0x4E, 0x4E, 0x4E,
	0x4F, 0x4F, 0x4F,
	0x50, 0x50, 0x50,
	0x51, 0x51, 0x51,
	0x53, 0x53, 0x53,
	0x54, 0x54, 0x54,
	0x55, 0x55, 0x55,
	0x56, 0x56, 0x56,
	0x57, 0x57, 0x57,
	0x58, 0x58, 0x58,
	0x59, 0x59, 0x59,
	0x5A, 0x5A, 0x5A,
	0x5C, 0x5C, 0x5C,
	0x5D, 0x5D, 0x5D,
	0x5E, 0x5E, 0x5E,
	0x5F, 0x5F, 0x5F,
	0x60, 0x60, 0x60,
	0x61, 0x61, 0x61,
	0x62, 0x62, 0x62,
	0x63, 0x63, 0x63,
	0x65, 0x65, 0x65,
	0x66, 0x66, 0x66,
	0x67, 0x67, 0x67,
	0x68, 0x68, 0x68,
	0x69, 0x69, 0x69,
	0x6A, 0x6A, 0x6A,
	0x6B, 0x6B, 0x6B,
	0x6C, 0x6C, 0x6C,
	0x6E, 0x6E, 0x6E,
	0x6F, 0x6F, 0x6F,
	0x70, 0x70, 0x70,
	0x71, 0x71, 0x71,
	0x72, 0x72, 0x72,
	0x73, 0x73, 0x73,
	0x74, 0x74, 0x74,
	0x75, 0x75, 0x75,
	0x77, 0x77, 0x77,
	0x78, 0x78, 0x78,
	0x79, 0x79, 0x79,
	0x7A, 0x7A, 0x7A,
	0x7B, 0x7B, 0x7B,
	0x7C, 0x7C, 0x7C,
	0x7D, 0x7D, 0x7D,
	0x7E, 0x7E, 0x7E,
	0x80, 0x80, 0x80,
	0x81, 0x81, 0x81,
	0x82, 0x82, 0x82,
	0x83, 0x83, 0x83,
	0x84, 0x84, 0x84,
	0x85, 0x85, 0x85,
	0x86, 0x86, 0x86,
	0x87, 0x87, 0x87,
	0x89, 0x89, 0x89,
	0x8A, 0x8A, 0x8A,
	0x8B, 0x8B, 0x8B,
	0x8C, 0x8C, 0x8C,
	0x8D, 0x8D, 0x8D,
	0x8E, 0x8E, 0x8E,
	0x8F, 0x8F, 0x8F,
	0x90, 0x90, 0x90,
	0x92, 0x92, 0x92,
	0x93, 0x93, 0x93,
	0x94, 0x94, 0x94,
	0x95, 0x95, 0x95,
	0x96, 0x96, 0x96,
	0x97, 0x97, 0x97,
	0x98, 0x98, 0x98,
	0x99, 0x99, 0x99,
	0x9B, 0x9B, 0x9B,
	0x9C, 0x9C, 0x9C,
	0x9D, 0x9D, 0x9D,
	0x9E, 0x9E, 0x9E,
	0x9F, 0x9F, 0x9F,
	0xA0, 0xA0, 0xA0,
	0xA1, 0xA1, 0xA1,
	0xA2, 0xA2, 0xA2,
	0xA4, 0xA4, 0xA4,
	0xA5, 0xA5, 0xA5,
	0xA6, 0xA6, 0xA6,
	0xA7, 0xA7, 0xA7,
	0xA8, 0xA8, 0xA8,
	0xA9, 0xA9, 0xA9,
	0xAA, 0xAA, 0xAA,
	0xAB, 0xAB, 0xAB,
	0xAD, 0xAD, 0xAD,
	0xAE, 0xAE, 0xAE,
	0xAF, 0xAF, 0xAF,
	0xB0, 0xB0, 0xB0,
	0xB1, 0xB1, 0xB1,
	0xB2, 0xB2, 0xB2,
	0xB3, 0xB3, 0xB3,
	0xB4, 0xB4, 0xB4,
	0xB6, 0xB6, 0xB6,
	0xB7, 0xB7, 0xB7,
	0xB8, 0xB8, 0xB8,
	0xB9, 0xB9, 0xB9,
	0xBA, 0xBA, 0xBA,
	0xBB, 0xBB, 0xBB,
	0xBC, 0xBC, 0xBC,
	0xBD, 0xBD, 0xBD,
	0xBF, 0xBF, 0xBF,
	0xC0, 0xC0, 0xC0,
	0xC1, 0xC1, 0xC1,
	0xC2, 0xC2, 0xC2,
	0xC3, 0xC3, 0xC3,
	0xC4, 0xC4, 0xC4,
	0xC5, 0xC5, 0xC5,
	0xC6, 0xC6, 0xC6,
	0xC8, 0xC8, 0xC8,
	0xC9, 0xC9, 0xC9,
	0xCA, 0xCA, 0xCA,
	0xCB, 0xCB, 0xCB,
	0xCC, 0xCC, 0xCC,
	0xCD, 0xCD, 0xCD,
	0xCE, 0xCE, 0xCE,
	0xCF, 0xCF, 0xCF,
	0xD0, 0xD0, 0xD0,
	0xD1, 0xD1, 0xD1,
	0xD2, 0xD2, 0xD2,
	0xD3, 0xD3, 0xD3,
	0xD4, 0xD4, 0xD4,
	0xD5, 0xD5, 0xD5,
	0xD6, 0xD6, 0xD6,
	0xD7, 0xD7, 0xD7,
	0xD8, 0xD8, 0xD8,
	0xD9, 0xD9, 0xD9,
	0xDA, 0xDA, 0xDA,
	0xDB, 0xDB, 0xDB,
	0xDC, 0xDC, 0xDC,
	0xDD, 0xDD, 0xDD,
	0xDE, 0xDE, 0xDE,
	0xDF, 0xDF, 0xDF,
	0xE0, 0xE0, 0xE0,
	0xE0, 0xE0, 0xE0,
	0xE1, 0xE1, 0xE1,
	0xE2, 0xE2, 0xE2,
	0xE3, 0xE3, 0xE3,
	0xE4, 0xE4, 0xE4,
	0xE5, 0xE5, 0xE5,
	0xE6, 0xE6, 0xE6,
	0xE7, 0xE7, 0xE7,
	0xE7, 0xE7, 0xE7,
	0xE8, 0xE8, 0xE8,
	0xE9, 0xE9, 0xE9,
	0xEA, 0xEA, 0xEA,
	0xEB, 0xEB, 0xEB,
	0xEC, 0xEC, 0xEC,
	0xEC, 0xEC, 0xEC,
	0xED, 0xED, 0xED,
	0xEE, 0xEE, 0xEE,
	0xEF, 0xEF, 0xEF,
	0xEF, 0xEF, 0xEF,
	0xF0, 0xF0, 0xF0,
	0xF1, 0xF1, 0xF1,
	0xF2, 0xF2, 0xF2,
	0xF2, 0xF2, 0xF2,
	0xF3, 0xF3, 0xF3,
	0xF4, 0xF4, 0xF4,
	0xF5, 0xF5, 0xF5,
	0xF5, 0xF5, 0xF5,
	0xF6, 0xF6, 0xF6,
	0xF7, 0xF7, 0xF7,
	0xF7, 0xF7, 0xF7,
	0xF8, 0xF8, 0xF8,
	0xF9, 0xF9, 0xF9,
	0xF9, 0xF9, 0xF9,
	0xFA, 0xFA, 0xFA,
	0xFB, 0xFB, 0xFB,
	0xFB, 0xFB, 0xFB,
	0xFC, 0xFC, 0xFC,
	0xFD, 0xFD, 0xFD,
	0xFD, 0xFD, 0xFD,
	0xFE, 0xFE, 0xFE,
	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,
};
static unsigned int NEGATIVE_TUNE[] = {
	0xFF, 0xFF, 0xFF,
	0xFE, 0xFE, 0xFE,
	0xFD, 0xFD, 0xFD,
	0xFC, 0xFC, 0xFC,
	0xFB, 0xFB, 0xFB,
	0xFA, 0xFA, 0xFA,
	0xF9, 0xF9, 0xF9,
	0xF8, 0xF8, 0xF8,
	0xF7, 0xF7, 0xF7,
	0xF6, 0xF6, 0xF6,
	0xF5, 0xF5, 0xF5,
	0xF4, 0xF4, 0xF4,
	0xF3, 0xF3, 0xF3,
	0xF2, 0xF2, 0xF2,
	0xF1, 0xF1, 0xF1,
	0xF0, 0xF0, 0xF0,
	0xEF, 0xEF, 0xEF,
	0xEE, 0xEE, 0xEE,
	0xED, 0xED, 0xED,
	0xEC, 0xEC, 0xEC,
	0xEB, 0xEB, 0xEB,
	0xEA, 0xEA, 0xEA,
	0xE9, 0xE9, 0xE9,
	0xE8, 0xE8, 0xE8,
	0xE7, 0xE7, 0xE7,
	0xE6, 0xE6, 0xE6,
	0xE5, 0xE5, 0xE5,
	0xE4, 0xE4, 0xE4,
	0xE3, 0xE3, 0xE3,
	0xE2, 0xE2, 0xE2,
	0xE1, 0xE1, 0xE1,
	0xE0, 0xE0, 0xE0,
	0xDF, 0xDF, 0xDF,
	0xDE, 0xDE, 0xDE,
	0xDD, 0xDD, 0xDD,
	0xDC, 0xDC, 0xDC,
	0xDB, 0xDB, 0xDB,
	0xDA, 0xDA, 0xDA,
	0xD9, 0xD9, 0xD9,
	0xD8, 0xD8, 0xD8,
	0xD7, 0xD7, 0xD7,
	0xD6, 0xD6, 0xD6,
	0xD5, 0xD5, 0xD5,
	0xD4, 0xD4, 0xD4,
	0xD3, 0xD3, 0xD3,
	0xD2, 0xD2, 0xD2,
	0xD1, 0xD1, 0xD1,
	0xD0, 0xD0, 0xD0,
	0xCF, 0xCF, 0xCF,
	0xCE, 0xCE, 0xCE,
	0xCD, 0xCD, 0xCD,
	0xCC, 0xCC, 0xCC,
	0xCB, 0xCB, 0xCB,
	0xCA, 0xCA, 0xCA,
	0xC9, 0xC9, 0xC9,
	0xC8, 0xC8, 0xC8,
	0xC7, 0xC7, 0xC7,
	0xC6, 0xC6, 0xC6,
	0xC5, 0xC5, 0xC5,
	0xC4, 0xC4, 0xC4,
	0xC3, 0xC3, 0xC3,
	0xC2, 0xC2, 0xC2,
	0xC1, 0xC1, 0xC1,
	0xC0, 0xC0, 0xC0,
	0xBF, 0xBF, 0xBF,
	0xBE, 0xBE, 0xBE,
	0xBD, 0xBD, 0xBD,
	0xBC, 0xBC, 0xBC,
	0xBB, 0xBB, 0xBB,
	0xBA, 0xBA, 0xBA,
	0xB9, 0xB9, 0xB9,
	0xB8, 0xB8, 0xB8,
	0xB7, 0xB7, 0xB7,
	0xB6, 0xB6, 0xB6,
	0xB5, 0xB5, 0xB5,
	0xB4, 0xB4, 0xB4,
	0xB3, 0xB3, 0xB3,
	0xB2, 0xB2, 0xB2,
	0xB1, 0xB1, 0xB1,
	0xB0, 0xB0, 0xB0,
	0xAF, 0xAF, 0xAF,
	0xAE, 0xAE, 0xAE,
	0xAD, 0xAD, 0xAD,
	0xAC, 0xAC, 0xAC,
	0xAB, 0xAB, 0xAB,
	0xAA, 0xAA, 0xAA,
	0xA9, 0xA9, 0xA9,
	0xA8, 0xA8, 0xA8,
	0xA7, 0xA7, 0xA7,
	0xA6, 0xA6, 0xA6,
	0xA5, 0xA5, 0xA5,
	0xA4, 0xA4, 0xA4,
	0xA3, 0xA3, 0xA3,
	0xA2, 0xA2, 0xA2,
	0xA1, 0xA1, 0xA1,
	0xA0, 0xA0, 0xA0,
	0x9F, 0x9F, 0x9F,
	0x9E, 0x9E, 0x9E,
	0x9D, 0x9D, 0x9D,
	0x9C, 0x9C, 0x9C,
	0x9B, 0x9B, 0x9B,
	0x9A, 0x9A, 0x9A,
	0x99, 0x99, 0x99,
	0x98, 0x98, 0x98,
	0x97, 0x97, 0x97,
	0x96, 0x96, 0x96,
	0x95, 0x95, 0x95,
	0x94, 0x94, 0x94,
	0x93, 0x93, 0x93,
	0x92, 0x92, 0x92,
	0x91, 0x91, 0x91,
	0x90, 0x90, 0x90,
	0x8F, 0x8F, 0x8F,
	0x8E, 0x8E, 0x8E,
	0x8D, 0x8D, 0x8D,
	0x8C, 0x8C, 0x8C,
	0x8B, 0x8B, 0x8B,
	0x8A, 0x8A, 0x8A,
	0x89, 0x89, 0x89,
	0x88, 0x88, 0x88,
	0x87, 0x87, 0x87,
	0x86, 0x86, 0x86,
	0x85, 0x85, 0x85,
	0x84, 0x84, 0x84,
	0x83, 0x83, 0x83,
	0x82, 0x82, 0x82,
	0x81, 0x81, 0x81,
	0x80, 0x80, 0x80,
	0x7F, 0x7F, 0x7F,
	0x7E, 0x7E, 0x7E,
	0x7D, 0x7D, 0x7D,
	0x7C, 0x7C, 0x7C,
	0x7B, 0x7B, 0x7B,
	0x7A, 0x7A, 0x7A,
	0x79, 0x79, 0x79,
	0x78, 0x78, 0x78,
	0x77, 0x77, 0x77,
	0x76, 0x76, 0x76,
	0x75, 0x75, 0x75,
	0x74, 0x74, 0x74,
	0x73, 0x73, 0x73,
	0x72, 0x72, 0x72,
	0x71, 0x71, 0x71,
	0x70, 0x70, 0x70,
	0x6F, 0x6F, 0x6F,
	0x6E, 0x6E, 0x6E,
	0x6D, 0x6D, 0x6D,
	0x6C, 0x6C, 0x6C,
	0x6B, 0x6B, 0x6B,
	0x6A, 0x6A, 0x6A,
	0x69, 0x69, 0x69,
	0x68, 0x68, 0x68,
	0x67, 0x67, 0x67,
	0x66, 0x66, 0x66,
	0x65, 0x65, 0x65,
	0x64, 0x64, 0x64,
	0x63, 0x63, 0x63,
	0x62, 0x62, 0x62,
	0x61, 0x61, 0x61,
	0x60, 0x60, 0x60,
	0x5F, 0x5F, 0x5F,
	0x5E, 0x5E, 0x5E,
	0x5D, 0x5D, 0x5D,
	0x5C, 0x5C, 0x5C,
	0x5B, 0x5B, 0x5B,
	0x5A, 0x5A, 0x5A,
	0x59, 0x59, 0x59,
	0x58, 0x58, 0x58,
	0x57, 0x57, 0x57,
	0x56, 0x56, 0x56,
	0x55, 0x55, 0x55,
	0x54, 0x54, 0x54,
	0x53, 0x53, 0x53,
	0x52, 0x52, 0x52,
	0x51, 0x51, 0x51,
	0x50, 0x50, 0x50,
	0x4F, 0x4F, 0x4F,
	0x4E, 0x4E, 0x4E,
	0x4D, 0x4D, 0x4D,
	0x4C, 0x4C, 0x4C,
	0x4B, 0x4B, 0x4B,
	0x4A, 0x4A, 0x4A,
	0x49, 0x49, 0x49,
	0x48, 0x48, 0x48,
	0x47, 0x47, 0x47,
	0x46, 0x46, 0x46,
	0x45, 0x45, 0x45,
	0x44, 0x44, 0x44,
	0x43, 0x43, 0x43,
	0x42, 0x42, 0x42,
	0x41, 0x41, 0x41,
	0x40, 0x40, 0x40,
	0x3F, 0x3F, 0x3F,
	0x3E, 0x3E, 0x3E,
	0x3D, 0x3D, 0x3D,
	0x3C, 0x3C, 0x3C,
	0x3B, 0x3B, 0x3B,
	0x3A, 0x3A, 0x3A,
	0x39, 0x39, 0x39,
	0x38, 0x38, 0x38,
	0x37, 0x37, 0x37,
	0x36, 0x36, 0x36,
	0x35, 0x35, 0x35,
	0x34, 0x34, 0x34,
	0x33, 0x33, 0x33,
	0x32, 0x32, 0x32,
	0x31, 0x31, 0x31,
	0x30, 0x30, 0x30,
	0x2F, 0x2F, 0x2F,
	0x2E, 0x2E, 0x2E,
	0x2D, 0x2D, 0x2D,
	0x2C, 0x2C, 0x2C,
	0x2B, 0x2B, 0x2B,
	0x2A, 0x2A, 0x2A,
	0x29, 0x29, 0x29,
	0x28, 0x28, 0x28,
	0x27, 0x27, 0x27,
	0x26, 0x26, 0x26,
	0x25, 0x25, 0x25,
	0x24, 0x24, 0x24,
	0x23, 0x23, 0x23,
	0x22, 0x22, 0x22,
	0x21, 0x21, 0x21,
	0x20, 0x20, 0x20,
	0x1F, 0x1F, 0x1F,
	0x1E, 0x1E, 0x1E,
	0x1D, 0x1D, 0x1D,
	0x1C, 0x1C, 0x1C,
	0x1B, 0x1B, 0x1B,
	0x1A, 0x1A, 0x1A,
	0x19, 0x19, 0x19,
	0x18, 0x18, 0x18,
	0x17, 0x17, 0x17,
	0x16, 0x16, 0x16,
	0x15, 0x15, 0x15,
	0x14, 0x14, 0x14,
	0x13, 0x13, 0x13,
	0x12, 0x12, 0x12,
	0x11, 0x11, 0x11,
	0x10, 0x10, 0x10,
	0x0F, 0x0F, 0x0F,
	0x0E, 0x0E, 0x0E,
	0x0D, 0x0D, 0x0D,
	0x0C, 0x0C, 0x0C,
	0x0B, 0x0B, 0x0B,
	0x0A, 0x0A, 0x0A,
	0x09, 0x09, 0x09,
	0x08, 0x08, 0x08,
	0x07, 0x07, 0x07,
	0x06, 0x06, 0x06,
	0x05, 0x05, 0x05,
	0x04, 0x04, 0x04,
	0x03, 0x03, 0x03,
	0x02, 0x02, 0x02,
	0x01, 0x01, 0x01,
	0x00, 0x00, 0x00,
};
void init_mdnie_class(void);

#endif /*_MDP4_VIDEO_TUNING_H_*/
