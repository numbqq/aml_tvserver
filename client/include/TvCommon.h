/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#ifndef __TV_COMMON_H__
#define __TV_COMMON_H__

#ifndef __TV_SOURCE_INPUT__
#define __TV_SOURCE_INPUT__
typedef enum tv_source_input_e {
    SOURCE_INVALID = -1,
    SOURCE_TV = 0,
    SOURCE_AV1,
    SOURCE_AV2,
    SOURCE_YPBPR1,
    SOURCE_YPBPR2,
    SOURCE_HDMI1,
    SOURCE_HDMI2,
    SOURCE_HDMI3,
    SOURCE_HDMI4,
    SOURCE_VGA,
    SOURCE_MPEG,
    SOURCE_DTV,
    SOURCE_SVIDEO,
    SOURCE_IPTV,
    SOURCE_DUMMY,
    SOURCE_SPDIF,
    SOURCE_MAX,
} tv_source_input_t;
#endif

#ifndef __TV_SOURCE_INPUT_TYPE__
#define __TV_SOURCE_INPUT_TYPE__
typedef enum tv_source_input_type_e {
    SOURCE_TYPE_TV,
    SOURCE_TYPE_AV,
    SOURCE_TYPE_COMPONENT,
    SOURCE_TYPE_HDMI,
    SOURCE_TYPE_VGA,
    SOURCE_TYPE_MPEG,
    SOURCE_TYPE_DTV,
    SOURCE_TYPE_SVIDEO,
    SOURCE_TYPE_IPTV,
    SOURCE_TYPE_SPDIF,
    SOURCE_TYPE_MAX,
} tv_source_input_type_t;
#endif

#ifndef __TVIN_PORT__
#define __TVIN_PORT__
/* tvin input port select */
typedef enum tvin_port_e {
    TVIN_PORT_NULL    = 0x00000000,
    TVIN_PORT_MPEG0   = 0x00000100,
    TVIN_PORT_BT656   = 0x00000200,
    TVIN_PORT_BT601,
    TVIN_PORT_CAMERA,
    TVIN_PORT_VGA0    = 0x00000400,
    TVIN_PORT_VGA1,
    TVIN_PORT_VGA2,
    TVIN_PORT_VGA3,
    TVIN_PORT_VGA4,
    TVIN_PORT_VGA5,
    TVIN_PORT_VGA6,
    TVIN_PORT_VGA7,
    TVIN_PORT_COMP0   = 0x00000800,
    TVIN_PORT_COMP1,
    TVIN_PORT_COMP2,
    TVIN_PORT_COMP3,
    TVIN_PORT_COMP4,
    TVIN_PORT_COMP5,
    TVIN_PORT_COMP6,
    TVIN_PORT_COMP7,
    TVIN_PORT_CVBS0   = 0x00001000,
    TVIN_PORT_CVBS1,
    TVIN_PORT_CVBS2,
    TVIN_PORT_CVBS3,    //as atv demod to tvafe
    TVIN_PORT_CVBS4,
    TVIN_PORT_CVBS5,
    TVIN_PORT_CVBS6,
    TVIN_PORT_CVBS7,
    TVIN_PORT_SVIDEO0 = 0x00002000,
    TVIN_PORT_SVIDEO1,
    TVIN_PORT_SVIDEO2,
    TVIN_PORT_SVIDEO3,
    TVIN_PORT_SVIDEO4,
    TVIN_PORT_SVIDEO5,
    TVIN_PORT_SVIDEO6,
    TVIN_PORT_SVIDEO7,
    TVIN_PORT_HDMI0   = 0x00004000,
    TVIN_PORT_HDMI1,
    TVIN_PORT_HDMI2,
    TVIN_PORT_HDMI3,
    TVIN_PORT_HDMI4,
    TVIN_PORT_HDMI5,
    TVIN_PORT_HDMI6,
    TVIN_PORT_HDMI7,
    TVIN_PORT_DVIN0   = 0x00008000,
    TVIN_PORT_VIU     = 0x0000C000,
    TVIN_PORT_MIPI    = 0x00010000,
    TVIN_PORT_ISP     = 0x00020000,
    TVIN_PORT_DTV     = 0x00040000,
    TVIN_PORT_MAX     = 0x80000000,
} tvin_port_t;
#endif

#ifndef __TVIN_TRANS_FMT__
#define __TVIN_TRANS_FMT__
typedef enum tvin_trans_fmt {
    TVIN_TFMT_2D = 0,
    TVIN_TFMT_3D_LRH_OLOR,  // 1 Primary: Side-by-Side(Half) Odd/Left picture, Odd/Right p
    TVIN_TFMT_3D_LRH_OLER,  // 2 Primary: Side-by-Side(Half) Odd/Left picture, Even/Right picture
    TVIN_TFMT_3D_LRH_ELOR,  // 3 Primary: Side-by-Side(Half) Even/Left picture, Odd/Right picture
    TVIN_TFMT_3D_LRH_ELER,  // 4 Primary: Side-by-Side(Half) Even/Left picture, Even/Right picture
    TVIN_TFMT_3D_TB,        // 5 Primary: Top-and-Bottom
    TVIN_TFMT_3D_FP,        // 6 Primary: Frame Packing
    TVIN_TFMT_3D_FA,        // 7 Secondary: Field Alternative
    TVIN_TFMT_3D_LA,        // 8 Secondary: Line Alternative
    TVIN_TFMT_3D_LRF,       // 9 Secondary: Side-by-Side(Full)
    TVIN_TFMT_3D_LD,        // 10 Secondary: L+depth
    TVIN_TFMT_3D_LDGD,      // 11 Secondary: L+depth+Graphics+Graphics-depth
    /* normal 3D format */
    TVIN_TFMT_3D_DET_TB,    // 12
    TVIN_TFMT_3D_DET_LR,    // 13
    TVIN_TFMT_3D_DET_INTERLACE,// 14
    TVIN_TFMT_3D_DET_CHESSBOARD,// 15
    TVIN_TFMT_3D_MAX,
} tvin_trans_fmt_t;
#endif

#ifndef __TVIN_SIG_FMT__
#define __TVIN_SIG_FMT__
/* tvin signal format table */
typedef enum tvin_sig_fmt_e {
    TVIN_SIG_FMT_NULL = 0,
    //VGA Formats
    TVIN_SIG_FMT_VGA_512X384P_60HZ_D147             = 0x001,
    TVIN_SIG_FMT_VGA_560X384P_60HZ_D147             = 0x002,
    TVIN_SIG_FMT_VGA_640X200P_59HZ_D924             = 0x003,
    TVIN_SIG_FMT_VGA_640X350P_85HZ_D080             = 0x004,
    TVIN_SIG_FMT_VGA_640X400P_59HZ_D940             = 0x005,
    TVIN_SIG_FMT_VGA_640X400P_85HZ_D080             = 0x006,
    TVIN_SIG_FMT_VGA_640X400P_59HZ_D638             = 0x007,
    TVIN_SIG_FMT_VGA_640X400P_56HZ_D416             = 0x008,
    TVIN_SIG_FMT_VGA_640X480P_66HZ_D619             = 0x009,
    TVIN_SIG_FMT_VGA_640X480P_66HZ_D667             = 0x00a,
    TVIN_SIG_FMT_VGA_640X480P_59HZ_D940             = 0x00b,
    TVIN_SIG_FMT_VGA_640X480P_60HZ_D000             = 0x00c,
    TVIN_SIG_FMT_VGA_640X480P_72HZ_D809             = 0x00d,
    TVIN_SIG_FMT_VGA_640X480P_75HZ_D000_A           = 0x00e,
    TVIN_SIG_FMT_VGA_640X480P_85HZ_D008             = 0x00f,
    TVIN_SIG_FMT_VGA_640X480P_59HZ_D638             = 0x010,
    TVIN_SIG_FMT_VGA_640X480P_75HZ_D000_B           = 0x011,
    TVIN_SIG_FMT_VGA_640X870P_75HZ_D000             = 0x012,
    TVIN_SIG_FMT_VGA_720X350P_70HZ_D086             = 0x013,
    TVIN_SIG_FMT_VGA_720X400P_85HZ_D039             = 0x014,
    TVIN_SIG_FMT_VGA_720X400P_70HZ_D086             = 0x015,
    TVIN_SIG_FMT_VGA_720X400P_87HZ_D849             = 0x016,
    TVIN_SIG_FMT_VGA_720X400P_59HZ_D940             = 0x017,
    TVIN_SIG_FMT_VGA_720X480P_59HZ_D940             = 0x018,
    TVIN_SIG_FMT_VGA_768X480P_59HZ_D896             = 0x019,
    TVIN_SIG_FMT_VGA_800X600P_56HZ_D250             = 0x01a,
    TVIN_SIG_FMT_VGA_800X600P_60HZ_D000             = 0x01b,
    TVIN_SIG_FMT_VGA_800X600P_60HZ_D000_A           = 0x01c,
    TVIN_SIG_FMT_VGA_800X600P_60HZ_D317             = 0x01d,
    TVIN_SIG_FMT_VGA_800X600P_72HZ_D188             = 0x01e,
    TVIN_SIG_FMT_VGA_800X600P_75HZ_D000             = 0x01f,
    TVIN_SIG_FMT_VGA_800X600P_85HZ_D061             = 0x020,
    TVIN_SIG_FMT_VGA_832X624P_75HZ_D087             = 0x021,
    TVIN_SIG_FMT_VGA_848X480P_84HZ_D751             = 0x022,
    TVIN_SIG_FMT_VGA_960X600P_59HZ_D635             = 0x023,
    TVIN_SIG_FMT_VGA_1024X768P_59HZ_D278            = 0x024,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D000            = 0x025,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D000_A          = 0x026,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D000_B          = 0x027,
    TVIN_SIG_FMT_VGA_1024X768P_74HZ_D927            = 0x028,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D004            = 0x029,
    TVIN_SIG_FMT_VGA_1024X768P_70HZ_D069            = 0x02a,
    TVIN_SIG_FMT_VGA_1024X768P_75HZ_D029            = 0x02b,
    TVIN_SIG_FMT_VGA_1024X768P_84HZ_D997            = 0x02c,
    TVIN_SIG_FMT_VGA_1024X768P_74HZ_D925            = 0x02d,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D020            = 0x02e,
    TVIN_SIG_FMT_VGA_1024X768P_70HZ_D008            = 0x02f,
    TVIN_SIG_FMT_VGA_1024X768P_75HZ_D782            = 0x030,
    TVIN_SIG_FMT_VGA_1024X768P_77HZ_D069            = 0x031,
    TVIN_SIG_FMT_VGA_1024X768P_71HZ_D799            = 0x032,
    TVIN_SIG_FMT_VGA_1024X1024P_60HZ_D000           = 0x033,
    TVIN_SIG_FMT_VGA_1152X864P_60HZ_D000            = 0x034,
    TVIN_SIG_FMT_VGA_1152X864P_70HZ_D012            = 0x035,
    TVIN_SIG_FMT_VGA_1152X864P_75HZ_D000            = 0x036,
    TVIN_SIG_FMT_VGA_1152X864P_84HZ_D999            = 0x037,
    TVIN_SIG_FMT_VGA_1152X870P_75HZ_D062            = 0x038,
    TVIN_SIG_FMT_VGA_1152X900P_65HZ_D950            = 0x039,
    TVIN_SIG_FMT_VGA_1152X900P_66HZ_D004            = 0x03a,
    TVIN_SIG_FMT_VGA_1152X900P_76HZ_D047            = 0x03b,
    TVIN_SIG_FMT_VGA_1152X900P_76HZ_D149            = 0x03c,
    TVIN_SIG_FMT_VGA_1280X720P_59HZ_D855            = 0x03d,
    TVIN_SIG_FMT_VGA_1280X720P_60HZ_D000_A          = 0x03e,
    TVIN_SIG_FMT_VGA_1280X720P_60HZ_D000_B          = 0x03f,
    TVIN_SIG_FMT_VGA_1280X720P_60HZ_D000_C          = 0x040,
    TVIN_SIG_FMT_VGA_1280X720P_60HZ_D000_D          = 0x041,
    TVIN_SIG_FMT_VGA_1280X768P_59HZ_D870            = 0x042,
    TVIN_SIG_FMT_VGA_1280X768P_59HZ_D995            = 0x043,
    TVIN_SIG_FMT_VGA_1280X768P_60HZ_D100            = 0x044,
    TVIN_SIG_FMT_VGA_1280X768P_85HZ_D000            = 0x045,
    TVIN_SIG_FMT_VGA_1280X768P_74HZ_D893            = 0x046,
    TVIN_SIG_FMT_VGA_1280X768P_84HZ_D837            = 0x047,
    TVIN_SIG_FMT_VGA_1280X800P_59HZ_D810            = 0x048,
    TVIN_SIG_FMT_VGA_1280X800P_59HZ_D810_A          = 0x049,
    TVIN_SIG_FMT_VGA_1280X800P_60HZ_D000            = 0x04a,
    TVIN_SIG_FMT_VGA_1280X800P_85HZ_D000            = 0x04b,
    TVIN_SIG_FMT_VGA_1280X960P_60HZ_D000            = 0x04c,
    TVIN_SIG_FMT_VGA_1280X960P_60HZ_D000_A          = 0x04d,
    TVIN_SIG_FMT_VGA_1280X960P_75HZ_D000            = 0x04e,
    TVIN_SIG_FMT_VGA_1280X960P_85HZ_D002            = 0x04f,
    TVIN_SIG_FMT_VGA_1280X1024P_60HZ_D020           = 0x050,
    TVIN_SIG_FMT_VGA_1280X1024P_60HZ_D020_A         = 0x051,
    TVIN_SIG_FMT_VGA_1280X1024P_75HZ_D025           = 0x052,
    TVIN_SIG_FMT_VGA_1280X1024P_85HZ_D024           = 0x053,
    TVIN_SIG_FMT_VGA_1280X1024P_59HZ_D979           = 0x054,
    TVIN_SIG_FMT_VGA_1280X1024P_72HZ_D005           = 0x055,
    TVIN_SIG_FMT_VGA_1280X1024P_60HZ_D002           = 0x056,
    TVIN_SIG_FMT_VGA_1280X1024P_67HZ_D003           = 0x057,
    TVIN_SIG_FMT_VGA_1280X1024P_74HZ_D112           = 0x058,
    TVIN_SIG_FMT_VGA_1280X1024P_76HZ_D179           = 0x059,
    TVIN_SIG_FMT_VGA_1280X1024P_66HZ_D718           = 0x05a,
    TVIN_SIG_FMT_VGA_1280X1024P_66HZ_D677           = 0x05b,
    TVIN_SIG_FMT_VGA_1280X1024P_76HZ_D107           = 0x05c,
    TVIN_SIG_FMT_VGA_1280X1024P_59HZ_D996           = 0x05d,
    TVIN_SIG_FMT_VGA_1280X1024P_60HZ_D000           = 0x05e,
    TVIN_SIG_FMT_VGA_1360X768P_59HZ_D799            = 0x05f,
    TVIN_SIG_FMT_VGA_1360X768P_60HZ_D015            = 0x060,
    TVIN_SIG_FMT_VGA_1360X768P_60HZ_D015_A          = 0x061,
    TVIN_SIG_FMT_VGA_1360X850P_60HZ_D000            = 0x062,
    TVIN_SIG_FMT_VGA_1360X1024P_60HZ_D000           = 0x063,
    TVIN_SIG_FMT_VGA_1366X768P_59HZ_D790            = 0x064,
    TVIN_SIG_FMT_VGA_1366X768P_60HZ_D000            = 0x065,
    TVIN_SIG_FMT_VGA_1400X1050P_59HZ_D978           = 0x066,
    TVIN_SIG_FMT_VGA_1440X900P_59HZ_D887            = 0x067,
    TVIN_SIG_FMT_VGA_1440X1080P_60HZ_D000           = 0x068,
    TVIN_SIG_FMT_VGA_1600X900P_60HZ_D000            = 0x069,
    TVIN_SIG_FMT_VGA_1600X1024P_60HZ_D000           = 0x06a,
    TVIN_SIG_FMT_VGA_1600X1200P_59HZ_D869           = 0x06b,
    TVIN_SIG_FMT_VGA_1600X1200P_60HZ_D000           = 0x06c,
    TVIN_SIG_FMT_VGA_1600X1200P_65HZ_D000           = 0x06d,
    TVIN_SIG_FMT_VGA_1600X1200P_70HZ_D000           = 0x06e,
    TVIN_SIG_FMT_VGA_1680X1050P_59HZ_D954           = 0x06f,
    TVIN_SIG_FMT_VGA_1680X1080P_60HZ_D000           = 0x070,
    TVIN_SIG_FMT_VGA_1920X1080P_49HZ_D929           = 0x071,
    TVIN_SIG_FMT_VGA_1920X1080P_59HZ_D963_A         = 0x072,
    TVIN_SIG_FMT_VGA_1920X1080P_59HZ_D963           = 0x073,
    TVIN_SIG_FMT_VGA_1920X1080P_60HZ_D000           = 0x074,
    TVIN_SIG_FMT_VGA_1920X1200P_59HZ_D950           = 0x075,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D000_C          = 0x076,
    TVIN_SIG_FMT_VGA_1024X768P_60HZ_D000_D          = 0x077,
    TVIN_SIG_FMT_VGA_1920X1200P_59HZ_D988           = 0x078,
    TVIN_SIG_FMT_VGA_1400X900P_60HZ_D000            = 0x079,
    TVIN_SIG_FMT_VGA_1680X1050P_60HZ_D000           = 0x07a,
    TVIN_SIG_FMT_VGA_800X600P_60HZ_D062             = 0x07b,
    TVIN_SIG_FMT_VGA_800X600P_60HZ_317_B            = 0x07c,
    TVIN_SIG_FMT_VGA_RESERVE8                       = 0x07d,
    TVIN_SIG_FMT_VGA_RESERVE9                       = 0x07e,
    TVIN_SIG_FMT_VGA_RESERVE10                      = 0x07f,
    TVIN_SIG_FMT_VGA_RESERVE11                      = 0x080,
    TVIN_SIG_FMT_VGA_RESERVE12                      = 0x081,
    TVIN_SIG_FMT_VGA_MAX                            = 0x082,
    TVIN_SIG_FMT_VGA_THRESHOLD                      = 0x200,
    //Component Formats
    TVIN_SIG_FMT_COMP_480P_60HZ_D000                = 0x201,
    TVIN_SIG_FMT_COMP_480I_59HZ_D940                = 0x202,
    TVIN_SIG_FMT_COMP_576P_50HZ_D000                = 0x203,
    TVIN_SIG_FMT_COMP_576I_50HZ_D000                = 0x204,
    TVIN_SIG_FMT_COMP_720P_59HZ_D940                = 0x205,
    TVIN_SIG_FMT_COMP_720P_50HZ_D000                = 0x206,
    TVIN_SIG_FMT_COMP_1080P_23HZ_D976               = 0x207,
    TVIN_SIG_FMT_COMP_1080P_24HZ_D000               = 0x208,
    TVIN_SIG_FMT_COMP_1080P_25HZ_D000               = 0x209,
    TVIN_SIG_FMT_COMP_1080P_30HZ_D000               = 0x20a,
    TVIN_SIG_FMT_COMP_1080P_50HZ_D000               = 0x20b,
    TVIN_SIG_FMT_COMP_1080P_60HZ_D000               = 0x20c,
    TVIN_SIG_FMT_COMP_1080I_47HZ_D952               = 0x20d,
    TVIN_SIG_FMT_COMP_1080I_48HZ_D000               = 0x20e,
    TVIN_SIG_FMT_COMP_1080I_50HZ_D000_A             = 0x20f,
    TVIN_SIG_FMT_COMP_1080I_50HZ_D000_B             = 0x210,
    TVIN_SIG_FMT_COMP_1080I_50HZ_D000_C             = 0x211,
    TVIN_SIG_FMT_COMP_1080I_60HZ_D000               = 0x212,
    TVIN_SIG_FMT_COMP_MAX                           = 0x213,
    TVIN_SIG_FMT_COMP_THRESHOLD                     = 0x400,
    //HDMI Formats
    TVIN_SIG_FMT_HDMI_640X480P_60HZ                 = 0x401,
    TVIN_SIG_FMT_HDMI_720X480P_60HZ                 = 0x402,
    TVIN_SIG_FMT_HDMI_1280X720P_60HZ                = 0x403,
    TVIN_SIG_FMT_HDMI_1920X1080I_60HZ               = 0x404,
    TVIN_SIG_FMT_HDMI_1440X480I_60HZ                = 0x405,
    TVIN_SIG_FMT_HDMI_1440X240P_60HZ                = 0x406,
    TVIN_SIG_FMT_HDMI_2880X480I_60HZ                = 0x407,
    TVIN_SIG_FMT_HDMI_2880X240P_60HZ                = 0x408,
    TVIN_SIG_FMT_HDMI_1440X480P_60HZ                = 0x409,
    TVIN_SIG_FMT_HDMI_1920X1080P_60HZ               = 0x40a,
    TVIN_SIG_FMT_HDMI_720X576P_50HZ                 = 0x40b,
    TVIN_SIG_FMT_HDMI_1280X720P_50HZ                = 0x40c,
    TVIN_SIG_FMT_HDMI_1920X1080I_50HZ_A             = 0x40d,
    TVIN_SIG_FMT_HDMI_1440X576I_50HZ                = 0x40e,
    TVIN_SIG_FMT_HDMI_1440X288P_50HZ                = 0x40f,
    TVIN_SIG_FMT_HDMI_2880X576I_50HZ                = 0x410,
    TVIN_SIG_FMT_HDMI_2880X288P_50HZ                = 0x411,
    TVIN_SIG_FMT_HDMI_1440X576P_50HZ                = 0x412,
    TVIN_SIG_FMT_HDMI_1920X1080P_50HZ               = 0x413,
    TVIN_SIG_FMT_HDMI_1920X1080P_24HZ               = 0x414,
    TVIN_SIG_FMT_HDMI_1920X1080P_25HZ               = 0x415,
    TVIN_SIG_FMT_HDMI_1920X1080P_30HZ               = 0x416,
    TVIN_SIG_FMT_HDMI_2880X480P_60HZ                = 0x417,
    TVIN_SIG_FMT_HDMI_2880X576P_60HZ                = 0x418,
    TVIN_SIG_FMT_HDMI_1920X1080I_50HZ_B             = 0x419,
    TVIN_SIG_FMT_HDMI_1920X1080I_100HZ              = 0x41a,
    TVIN_SIG_FMT_HDMI_1280X720P_100HZ               = 0x41b,
    TVIN_SIG_FMT_HDMI_720X576P_100HZ                = 0x41c,
    TVIN_SIG_FMT_HDMI_1440X576I_100HZ               = 0x41d,
    TVIN_SIG_FMT_HDMI_1920X1080I_120HZ              = 0x41e,
    TVIN_SIG_FMT_HDMI_1280X720P_120HZ               = 0x41f,
    TVIN_SIG_FMT_HDMI_720X480P_120HZ                = 0x420,
    TVIN_SIG_FMT_HDMI_1440X480I_120HZ               = 0x421,
    TVIN_SIG_FMT_HDMI_720X576P_200HZ                = 0x422,
    TVIN_SIG_FMT_HDMI_1440X576I_200HZ               = 0x423,
    TVIN_SIG_FMT_HDMI_720X480P_240HZ                = 0x424,
    TVIN_SIG_FMT_HDMI_1440X480I_240HZ               = 0x425,
    TVIN_SIG_FMT_HDMI_1280X720P_24HZ                = 0x426,
    TVIN_SIG_FMT_HDMI_1280X720P_25HZ                = 0x427,
    TVIN_SIG_FMT_HDMI_1280X720P_30HZ                = 0x428,
    TVIN_SIG_FMT_HDMI_1920X1080P_120HZ              = 0x429,
    TVIN_SIG_FMT_HDMI_1920X1080P_100HZ              = 0x42a,
    TVIN_SIG_FMT_HDMI_1280X720P_60HZ_FRAME_PACKING  = 0x42b,
    TVIN_SIG_FMT_HDMI_1280X720P_50HZ_FRAME_PACKING  = 0x42c,
    TVIN_SIG_FMT_HDMI_1280X720P_24HZ_FRAME_PACKING  = 0x42d,
    TVIN_SIG_FMT_HDMI_1280X720P_30HZ_FRAME_PACKING  = 0x42e,
    TVIN_SIG_FMT_HDMI_1920X1080I_60HZ_FRAME_PACKING = 0x42f,
    TVIN_SIG_FMT_HDMI_1920X1080I_50HZ_FRAME_PACKING = 0x430,
    TVIN_SIG_FMT_HDMI_1920X1080P_24HZ_FRAME_PACKING = 0x431,
    TVIN_SIG_FMT_HDMI_1920X1080P_30HZ_FRAME_PACKING = 0x432,
    TVIN_SIG_FMT_HDMI_800X600_00HZ                  = 0x433,
    TVIN_SIG_FMT_HDMI_1024X768_00HZ                 = 0x434,
    TVIN_SIG_FMT_HDMI_720X400_00HZ                  = 0x435,
    TVIN_SIG_FMT_HDMI_1280X768_00HZ                 = 0x436,
    TVIN_SIG_FMT_HDMI_1280X800_00HZ                 = 0x437,
    TVIN_SIG_FMT_HDMI_1280X960_00HZ                 = 0x438,
    TVIN_SIG_FMT_HDMI_1280X1024_00HZ                = 0x439,
    TVIN_SIG_FMT_HDMI_1360X768_00HZ                 = 0x43a,
    TVIN_SIG_FMT_HDMI_1366X768_00HZ                 = 0x43b,
    TVIN_SIG_FMT_HDMI_1600X1200_00HZ                = 0x43c,
    TVIN_SIG_FMT_HDMI_1920X1200_00HZ                = 0x43d,
    TVIN_SIG_FMT_HDMI_1440X900_00HZ                 = 0x43e,
    TVIN_SIG_FMT_HDMI_1400X1050_00HZ                = 0x43f,
    TVIN_SIG_FMT_HDMI_1680X1050_00HZ                = 0x440,
    /* for alternative and 4k2k */
    TVIN_SIG_FMT_HDMI_1920X1080I_60HZ_ALTERNATIVE   = 0x441,
    TVIN_SIG_FMT_HDMI_1920X1080I_50HZ_ALTERNATIVE   = 0x442,
    TVIN_SIG_FMT_HDMI_1920X1080P_24HZ_ALTERNATIVE   = 0x443,
    TVIN_SIG_FMT_HDMI_1920X1080P_30HZ_ALTERNATIVE   = 0x444,
    TVIN_SIG_FMT_HDMI_3840_2160_00HZ                = 0x445,
    TVIN_SIG_FMT_HDMI_4096_2160_00HZ                = 0x446,
    TVIN_SIG_FMT_HDMI_HDR                           = 0x447,
    TVIN_SIG_FMT_HDMI_RESERVE8                      = 0x448,
    TVIN_SIG_FMT_HDMI_RESERVE9                      = 0x449,
    TVIN_SIG_FMT_HDMI_RESERVE10                     = 0x44a,
    TVIN_SIG_FMT_HDMI_RESERVE11                     = 0x44b,
    TVIN_SIG_FMT_HDMI_720X480P_60HZ_FRAME_PACKING   = 0x44c,
    TVIN_SIG_FMT_HDMI_720X576P_50HZ_FRAME_PACKING   = 0x44d,
    TVIN_SIG_FMT_HDMI_MAX                           = 0x44e,
    TVIN_SIG_FMT_HDMI_THRESHOLD                     = 0x600,
    //Video Formats
    TVIN_SIG_FMT_CVBS_NTSC_M                        = 0x601,
    TVIN_SIG_FMT_CVBS_NTSC_443                      = 0x602,
    TVIN_SIG_FMT_CVBS_PAL_I                         = 0x603,
    TVIN_SIG_FMT_CVBS_PAL_M                         = 0x604,
    TVIN_SIG_FMT_CVBS_PAL_60                        = 0x605,
    TVIN_SIG_FMT_CVBS_PAL_CN                        = 0x606,
    TVIN_SIG_FMT_CVBS_SECAM                         = 0x607,
    TVIN_SIG_FMT_CVBS_NTSC_50                       = 0x608,
    TVIN_SIG_FMT_CVBS_MAX                           = 0x609,
    TVIN_SIG_FMT_CVBS_THRESHOLD                     = 0x800,
    //656 Formats
    TVIN_SIG_FMT_BT656IN_576I_50HZ                  = 0x801,
    TVIN_SIG_FMT_BT656IN_480I_60HZ                  = 0x802,
    //601 Formats
    TVIN_SIG_FMT_BT601IN_576I_50HZ                  = 0x803,
    TVIN_SIG_FMT_BT601IN_480I_60HZ                  = 0x804,
    //Camera Formats
    TVIN_SIG_FMT_CAMERA_640X480P_30HZ               = 0x805,
    TVIN_SIG_FMT_CAMERA_800X600P_30HZ               = 0x806,
    TVIN_SIG_FMT_CAMERA_1024X768P_30HZ              = 0x807,
    TVIN_SIG_FMT_CAMERA_1920X1080P_30HZ             = 0x808,
    TVIN_SIG_FMT_CAMERA_1280X720P_30HZ              = 0x809,
    TVIN_SIG_FMT_BT601_MAX                          = 0x80a,
    TVIN_SIG_FMT_BT601_THRESHOLD                    = 0xa00,
    TVIN_SIG_FMT_MAX,
} tvin_sig_fmt_t;
#endif

#ifndef __TVIN_SIG_STATUS__
#define __TVIN_SIG_STATUS__
//tvin signal status
typedef enum tvin_sig_status_e {
    TVIN_SIG_STATUS_NULL = 0, // processing status from init to the finding of the 1st confirmed status
    TVIN_SIG_STATUS_NOSIG,    // no signal - physically no signal
    TVIN_SIG_STATUS_UNSTABLE, // unstable - physically bad signal
    TVIN_SIG_STATUS_NOTSUP,   // not supported - physically good signal & not supported
    TVIN_SIG_STATUS_STABLE,   // stable - physically good signal & supported
} tvin_sig_status_t;
#endif

#ifndef __TVIN_WORK_MODE__
#define __TVIN_WORK_MODE__
typedef enum vdin_work_mode_e {
    VDIN_WORK_MODE_VFM = 0,
    VDIN_WORK_MODE_V4L2,
    VDIN_WORK_MODE_MAX,
} vdin_work_mode_t;
#endif

#ifndef __TVIN_ASPECT_RATIO__
#define __TVIN_ASPECT_RATIO__
typedef enum tvin_aspect_ratio_e {
    TVIN_ASPECT_NULL = 0,
    TVIN_ASPECT_1x1,
    TVIN_ASPECT_4x3_FULL,
    TVIN_ASPECT_14x9_FULL,
    TVIN_ASPECT_14x9_LB_CENTER,
    TVIN_ASPECT_14x9_LB_TOP,
    TVIN_ASPECT_16x9_FULL,
    TVIN_ASPECT_16x9_LB_CENTER,
    TVIN_ASPECT_16x9_LB_TOP,
    TVIN_ASPECT_MAX,
} tvin_aspect_ratio_t;
#endif

#ifndef __TVIN_COLOR_FORMAT__
#define __TVIN_COLOR_FORMAT__
typedef enum tvin_color_fmt_e {
    TVIN_RGB444 = 0,
    TVIN_YUV422, // 1
    TVIN_YUV444, // 2
    TVIN_YUYV422,// 3
    TVIN_YVYU422,// 4
    TVIN_UYVY422,// 5
    TVIN_VYUY422,// 6
    TVIN_NV12,   // 7
    TVIN_NV21,   // 8
    TVIN_BGGR,   // 9  raw data
    TVIN_RGGB,   // 10 raw data
    TVIN_GBRG,   // 11 raw data
    TVIN_GRBG,   // 12 raw data
    TVIN_COLOR_FMT_MAX,
} tvin_color_fmt_t;
#endif

#ifndef __TVIN_COLOR_RANGE__
#define __TVIN_COLOR_RANGE__
typedef enum tvin_color_range_e {
    TVIN_COLOR_RANGE_AUTO = 0,
    TVIN_COLOR_RANGE_FULL,
    TVIN_COLOR_RANGE_LIMIT,
    TVIN_COLOR_RANGE_MAX,
} tvin_color_range_t;
#endif

#ifndef __TVIN_LINE_SCAN_MODE__
#define __TVIN_LINE_SCAN_MODE__
typedef enum tvin_line_scan_mode_e {
	TVIN_LINE_SCAN_MODE_NULL = 0,
	TVIN_LINE_SCAN_MODE_PROGRESSIVE,
	TVIN_LINE_SCAN_MODE_INTERLACED,
	TVIN_LINE_SCAN_MODE_MAX,
} tvin_line_scan_mode_t;
#endif

#endif
