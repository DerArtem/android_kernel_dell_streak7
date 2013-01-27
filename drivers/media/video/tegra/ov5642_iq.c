/*
 * ov5642_iq.c - ov5642 sensor driver
 *
 * Copyright (C) 2011 Google Inc.
 *
 * Contributors:
 *      Rebecca Schultz Zavin <rebecca@android.com>
 *
 * Leverage OV9640.c
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */
#include <media/yuv_sensor.h> 
#include "ov5642.h"

static struct ov5642_reg ov5642_abico_iq[] = {
    
    
#if 1 
    {0x5000, 0xcf}, 
#else
    {0x5000, 0x4f}, 
#endif
    {0x5001, 0x3f}, 
#if 1 
    
    {0x3a0f, 0x30}, 
    {0x3a10, 0x28}, 
    {0x3a1b, 0x30}, 
    {0x3a1e, 0x28}, 
    {0x3a11, 0x61}, 
    {0x3a1f, 0x10}, 

    
    {0x3a14, 0x00},
    {0x3a15, 0x7d},
    {0x3a16, 0x00},
    {0x3a08, 0x09},
    {0x3a09, 0x60},
    {0x3a0a, 0x07},
    {0x3a0b, 0xd0},
    {0x3a0d, 0x10},
    {0x3a0e, 0x0d},

    
    {0x3a00, 0x7c},
    
    
    {0x3a04, 0x00},
    {0x3a02, 0x00}, 
    {0x3a03, 0xfa},

    
    

    
    
    {0x528a, 0x02},
    {0x528b, 0x06},
    {0x528c, 0x20},
    {0x528d, 0x30},
    {0x528e, 0x40},
    {0x528f, 0x50},
    {0x5290, 0x60},
    {0x5292, 0x00},
    {0x5293, 0x02},
    {0x5294, 0x00},
    {0x5295, 0x04},
    {0x5296, 0x00},
    {0x5297, 0x08},
    {0x5298, 0x00},
    {0x5299, 0x10},
    {0x529a, 0x00},
    {0x529b, 0x20},
    {0x529c, 0x00},
    {0x529d, 0x28},
    {0x529e, 0x00},
    {0x529f, 0x30},
    {0x5282, 0x0 },

    
    
    {0x3a19, 0x7c}, 

    
    

    
    {0x530c, 0x00}, 
    {0x530d, 0x0c}, 
    {0x5312, 0x40}, 

    {0x530e, 0x20},
    {0x530f, 0x80},
    {0x5310, 0x20},
    {0x5311, 0x80},

    {0x3406, 0x00}, 


    
    {0x5688, 0x21}, 
    {0x5689, 0x12}, 
    {0x568a, 0xa6}, 
    {0x568b, 0x6a}, 
    {0x568c, 0xd6}, 
    {0x568d, 0x6d}, 
    {0x568e, 0x26}, 
    {0x568f, 0x62}, 

    
    {0x4007, 0x10}, 

    
    
    {0x5800, 0x52},
    {0x5801, 0x31},
    {0x5802, 0x26},
    {0x5803, 0x22},
    {0x5804, 0x21},
    {0x5805, 0x26},
    {0x5806, 0x33},
    {0x5807, 0x57},
    {0x5808, 0x29},
    {0x5809, 0x1c},
    {0x580a, 0x15},
    {0x580b, 0x12},
    {0x580c, 0x12},
    {0x580d, 0x15},
    {0x580e, 0x1b},
    {0x580f, 0x22},
    {0x5810, 0x17},
    {0x5811, 0x10},
    {0x5812, 0x9 },
    {0x5813, 0x6 },
    {0x5814, 0x6 },
    {0x5815, 0x9 },
    {0x5816, 0x10},
    {0x5817, 0x17},
    {0x5818, 0x13},
    {0x5819, 0xa },
    {0x581a, 0x3 },
    {0x581b, 0x0 },
    {0x581c, 0x0 },
    {0x581d, 0x3 },
    {0x581e, 0xa },
    {0x581f, 0x11},
    {0x5820, 0x11},
    {0x5821, 0x9 },
    {0x5822, 0x3 },
    {0x5823, 0x0 },
    {0x5824, 0x0 },
    {0x5825, 0x3 },
    {0x5826, 0xa },
    {0x5827, 0x11},
    {0x5828, 0x16},
    {0x5829, 0xe },
    {0x582a, 0x8 },
    {0x582b, 0x5 },
    {0x582c, 0x5 },
    {0x582d, 0x8 },
    {0x582e, 0xf },
    {0x582f, 0x16},
    {0x5830, 0x24},
    {0x5831, 0x18},
    {0x5832, 0x13},
    {0x5833, 0x10},
    {0x5834, 0x10},
    {0x5835, 0x13},
    {0x5836, 0x19},
    {0x5837, 0x1f},
    {0x5838, 0x4e},
    {0x5839, 0x2f},
    {0x583a, 0x21},
    {0x583b, 0x1d},
    {0x583c, 0x1e},
    {0x583d, 0x21},
    {0x583e, 0x2d},
    {0x583f, 0x47},
    
    {0x5840, 0x8 },
    {0x5841, 0x9 },
    {0x5842, 0xb },
    {0x5843, 0xc },
    {0x5844, 0xc },
    {0x5845, 0xd },
    {0x5846, 0xd },
    {0x5847, 0xc },
    {0x5848, 0xc },
    {0x5849, 0xc },
    {0x584a, 0xd },
    {0x584b, 0x9 },
    {0x584c, 0xc },
    {0x584d, 0xf },
    {0x584e, 0x10},
    {0x584f, 0x10},
    {0x5850, 0xf },
    {0x5851, 0x9 },
    {0x5852, 0xd },
    {0x5853, 0xf },
    {0x5854, 0x10},
    {0x5855, 0x10},
    {0x5856, 0x10},
    {0x5857, 0xa },
    {0x5858, 0xd },
    {0x5859, 0xe },
    {0x585a, 0xf },
    {0x585b, 0xf },
    {0x585c, 0xf },
    {0x585d, 0xb },
    {0x585e, 0xa },
    {0x585f, 0x8 },
    {0x5860, 0x8 },
    {0x5861, 0x9 },
    {0x5862, 0xa },
    {0x5863, 0xa },
    
    {0x5864, 0x15},
    {0x5865, 0x18},
    {0x5866, 0x15},
    {0x5867, 0x16},
    {0x5868, 0x14},
    {0x5869, 0x15},
    {0x586a, 0x18},
    {0x586b, 0x16},
    {0x586c, 0x16},
    {0x586d, 0x15},
    {0x586e, 0x17},
    {0x586f, 0x1a},
    {0x5870, 0x19},
    {0x5871, 0x14},
    {0x5872, 0x10},
    {0x5873, 0x10},
    {0x5874, 0x14},
    {0x5875, 0x19},
    {0x5876, 0x1b},
    {0x5877, 0x15},
    {0x5878, 0x10},
    {0x5879, 0x10},
    {0x587a, 0x14},
    {0x587b, 0x1b},
    {0x587c, 0x1b},
    {0x587d, 0x19},
    {0x587e, 0x18},
    {0x587f, 0x17},
    {0x5880, 0x19},
    {0x5881, 0x1a},
    {0x5882, 0x15},
    {0x5883, 0x1b},
    {0x5884, 0x1d},
    {0x5885, 0x1c},
    {0x5886, 0x19},
    {0x5887, 0x1d},

    
    {0x5480, 0x8 }, 
    {0x5481, 0x19}, 
    {0x5482, 0x2f}, 
    {0x5483, 0x4d}, 
    {0x5484, 0x5b}, 
    {0x5485, 0x64}, 
    {0x5486, 0x6f}, 
    {0x5487, 0x79}, 
    {0x5488, 0x82}, 
    {0x5489, 0x8a}, 
    {0x548a, 0x96}, 
    {0x548b, 0xa1}, 
    {0x548c, 0xb3}, 
    {0x548d, 0xc7}, 
    {0x548e, 0xdb}, 
    {0x548f, 0x31}, 
    {0x5490, 0x5 }, 
    {0x5491, 0x0 }, 
    {0x5492, 0x4 }, 
    {0x5493, 0x20}, 
    {0x5494, 0x3 }, 
    {0x5495, 0x60}, 
    {0x5496, 0x2 }, 
    {0x5497, 0xb8}, 
    {0x5498, 0x2 }, 
    {0x5499, 0x86}, 
    {0x549a, 0x2 }, 
    {0x549b, 0x5b}, 
    {0x549c, 0x2 }, 
    {0x549d, 0x3b}, 
    {0x549e, 0x2 }, 
    {0x549f, 0x1c}, 
    {0x54a0, 0x2 }, 
    {0x54a1, 0x4 }, 
    {0x54a2, 0x1 }, 
    {0x54a3, 0xed}, 
    {0x54a4, 0x1 }, 
    {0x54a5, 0xc5}, 
    {0x54a6, 0x1 }, 
    {0x54a7, 0xa5}, 
    {0x54a8, 0x1 }, 
    {0x54a9, 0x6c}, 
    {0x54aa, 0x1 }, 
    {0x54ab, 0x41}, 
    {0x54ac, 0x1 }, 
    {0x54ad, 0x20}, 
    {0x54ae, 0x0 }, 
    {0x54af, 0x16}, 
    {0x54b0, 0x1 }, 
    {0x54b1, 0x20}, 
    {0x54b2, 0x1 }, 
    {0x54b3, 0x40}, 
    {0x54b4, 0x0 }, 
    {0x54b5, 0xf0}, 
    {0x54b6, 0x1 }, 
    {0x54b7, 0xdf}, 

    
    
    {0x5380, 0x1 },
    {0x5381, 0x0 },
    {0x5382, 0x0 },
    {0x5383, 0x4e},
    {0x5384, 0x0 },
    {0x5385, 0xf },
    {0x5386, 0x0 },
    {0x5387, 0x0 },
    {0x5388, 0x1 },
    {0x5389, 0x6d},
    {0x538a, 0x0 },
    {0x538b, 0x5b},
    {0x538c, 0x0 },
    {0x538d, 0x0 },
    {0x538e, 0x0 },
    {0x538f, 0xc },
    {0x5390, 0x0 },
    {0x5391, 0xe8},
    {0x5392, 0x0 },
    {0x5393, 0xa2},
    {0x5394, 0x8 },

    
    {0x5001, 0xbf},
    {0x5583, 0x38}, 
    {0x5584, 0x38}, 
    {0x5580, 0x02},

    
    
    {0x5180, 0xff},
    {0x5181, 0x58},
    {0x5182, 0x11},
    {0x5183, 0x14},
    {0x5184, 0x25},
    {0x5185, 0x24},
    {0x5186, 0x14},
    {0x5187, 0x1b},
    {0x5188, 0x18},
    {0x5189, 0x5e},
    {0x518a, 0x64},
    {0x518b, 0x98},
    {0x518c, 0x8f},
    {0x518d, 0x37},
    {0x518e, 0x3d},
    {0x518f, 0x60},
    {0x5190, 0x54},
    {0x5191, 0xf8},
    {0x5192, 0x4 },
    {0x5193, 0x70},
    {0x5194, 0xf0},
    {0x5195, 0xf0},
    {0x5196, 0x3 },
    {0x5197, 0x1 },
    {0x5198, 0x6 },
    {0x5199, 0xd6},
    {0x519a, 0x4 },
    {0x519b, 0x0 },
    {0x519c, 0x5 },
    {0x519d, 0xa },
    {0x519e, 0xa0},
#endif
    {SENSOR_WAIT_MS, 150},
    {SENSOR_TABLE_END, 0x0},
};

static void* ov5642_get_iq_regs(void)
{
	return (void*)ov5642_abico_iq;
}








static struct ov5642_reg ov5642_auto_wb_reg[] =
{
    {SENSOR_BIT_OPERATION, 0x01},
    {0x3406, 0x00},
    {SENSOR_TABLE_END, 0x0000}
};

static struct ov5642_reg ov5642_incandscent_reg[] =  
{
    {SENSOR_BIT_OPERATION, 0x01},
    {0x3406, 0x01}, 
    {0x3400, 0x3 }, 
    {0x3401, 0xc0}, 
    {0x3402, 0x4 }, 
    {0x3403, 0x0 },
    {0x3404, 0x9 }, 
    {0x3405, 0x20}, 
    {SENSOR_TABLE_END, 0x0},
};
static struct ov5642_reg ov5642_fluorescent_reg[] =  
{
    {SENSOR_BIT_OPERATION, 0x01},
    {0x3406, 0x01}, 
    {0x3400, 0x5 }, 
    {0x3401, 0x00}, 
    {0x3402, 0x4 }, 
    {0x3403, 0x0 },
    {0x3404, 0x6 }, 
    {0x3405, 0xe0}, 
    {SENSOR_TABLE_END, 0x0},
};
static struct ov5642_reg ov5642_sun_light_reg[] =    
{
    {SENSOR_BIT_OPERATION, 0x01},
    {0x3406, 0x01}, 
    {0x3400, 0x6 }, 
    {0x3401, 0x30}, 
    {0x3402, 0x4 }, 
    {0x3403, 0x0 },
    {0x3404, 0x4 }, 
    {0x3405, 0xa0}, 
    {SENSOR_TABLE_END, 0x0},
};

static void* ov5642_get_white_balance_regs(u8 wb)
{
	switch (wb) {
		case YUV_Whitebalance_Incandescent:
			return (void*)ov5642_incandscent_reg;
		case YUV_Whitebalance_Fluorescent:
			return (void*)ov5642_fluorescent_reg;
		case YUV_Whitebalance_Daylight:
			return (void*)ov5642_sun_light_reg;
		default:
			return (void*)ov5642_auto_wb_reg;
	}
}







static struct ov5642_reg ov5642_auto_banding_reg[] =
{
    {SENSOR_BIT_OPERATION, 0x80},
    {0x3C01, 0x00},
    {SENSOR_BIT_OPERATION, 0x04},
    {0x3C00, 0x00},
    {SENSOR_TABLE_END, 0x0},
};
static struct ov5642_reg ov5642_50Hz_banding_reg[] =
{
    {SENSOR_BIT_OPERATION, 0x80},
    {0x3C01, 0x80},
    {SENSOR_BIT_OPERATION, 0x04},
    {0x3C00, 0x04},
    {SENSOR_TABLE_END, 0x0},
};
static struct ov5642_reg ov5642_60Hz_banding_reg[] =
{
    {SENSOR_BIT_OPERATION, 0x80},
    {0x3C01, 0x80 },
    {SENSOR_BIT_OPERATION, 0x04},
    {0x3C00, 0x00 },
    {SENSOR_TABLE_END, 0x0},
};
static void* ov5642_get_anti_banding_regs(u8 banding)
{
	switch (banding) {
		case YUV_AntiBanding_50Hz:
			return (void*)ov5642_50Hz_banding_reg;
		case YUV_AntiBanding_60Hz:
			return (void*)ov5642_60Hz_banding_reg;
		default:
			return (void*)ov5642_auto_banding_reg;
	}
}

struct ov5642_iq_func_array ov5642_iq_func =
{
	.get_iq_regs = ov5642_get_iq_regs,
	.get_white_balance_regs = ov5642_get_white_balance_regs,
	.get_anti_banding_regs = ov5642_get_anti_banding_regs,
};
