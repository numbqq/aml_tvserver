/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */

#ifndef _CTVIN_H
#define _CTVIN_H

#include "TvCommon.h"

// ***************************************************************************
// *** TVIN general definition/enum/struct ***********************************
// ***************************************************************************
typedef enum tvin_port_id_e {
    TVIN_PORT_ID_1 = 1,
    TVIN_PORT_ID_2,
    TVIN_PORT_ID_3,
    TVIN_PORT_ID_4,
    TVIN_PORT_ID_MAX,
} tvin_port_id_t;

typedef struct tvin_info_s {
    tvin_trans_fmt_t    trans_fmt;
    tvin_sig_fmt_t    fmt;
    tvin_sig_status_t status;
    tvin_color_fmt_t  cfmt;
    unsigned int      fps;
    unsigned int      is_dvi;
    /*
     * bit 30: is_dv
     * bit 29: present_flag
     * bit 28-26: video_format
     *  "component", "PAL", "NTSC", "SECAM", "MAC", "unspecified"
     * bit 25: range "limited", "full_range"
     * bit 24: color_description_present_flag
     * bit 23-16: color_primaries
     *  "unknown", "bt709", "undef", "bt601", "bt470m", "bt470bg",
     *  "smpte170m", "smpte240m", "film", "bt2020"
     * bit 15-8: transfer_characteristic
     *  "unknown", "bt709", "undef", "bt601", "bt470m", "bt470bg",
     *  "smpte170m", "smpte240m", "linear", "log100", "log316",
     *  "iec61966-2-4", "bt1361e", "iec61966-2-1", "bt2020-10",
     *  "bt2020-12", "smpte-st-2084", "smpte-st-428"
     * bit 7-0: matrix_coefficient
     *  "GBR", "bt709", "undef", "bt601", "fcc", "bt470bg",
     *  "smpte170m", "smpte240m", "YCgCo", "bt2020nc", "bt2020c"
     */
    unsigned int hdr_info;
    tvin_aspect_ratio_t aspect_ratio;
} tvin_info_t;

typedef struct tvin_parm_s {
    int                         index;    // index of frontend for vdin
    enum tvin_port_e            port;     // must set port in IOCTL
    struct tvin_info_s          info;
    unsigned int                hist_pow;
    unsigned int                luma_sum;
    unsigned int                pixel_sum;
    unsigned short              histgram[64];
    unsigned int                flag;
    unsigned short              dest_width;//for vdin horizontal scale down
    unsigned short              dest_height;//for vdin vertical scale down
    bool                        h_reverse;//for vdin horizontal reverse
    bool                        v_reverse;//for vdin vertical reverse
    unsigned int                reserved;
} tvin_parm_t;

typedef struct tvin_buf_info_s {
    unsigned int vf_size;
    unsigned int buf_count;
    unsigned int buf_width;
    unsigned int buf_height;
    unsigned int buf_size;
    unsigned int wr_list_size;
} tvin_buf_info_t;

typedef struct tvin_video_buf_s {
    unsigned int index;
    unsigned int reserved;
} tvin_video_buf_t;

typedef enum tvin_cn_type_e {
    GRAPHICS,
    PHOTO,
    CINEMA,
    GAME,
} tvin_cn_type_t;

typedef struct tvin_latency_s {
    bool allm_mode;
    bool it_content;
    tvin_cn_type_e cn_type;
} tvin_latency_t;

typedef struct tvin_frontend_info_s {
	enum tvin_line_scan_mode_e scan_mode;
	enum tvin_color_fmt_e cfmt;
	unsigned int fps;
	unsigned int width;
	unsigned int height;
	unsigned int colordepth;
} tvin_frontend_info_t;

// ***************************************************************************
// *** AFE module definition/enum/struct *************************************
// ***************************************************************************
typedef struct tvafe_adc_cal_s {
    // ADC A
    unsigned short a_analog_clamp;    // 0x00~0x7f
    unsigned short a_analog_gain;     // 0x00~0xff, means 0dB~6dB
    unsigned short a_digital_offset1; // offset for fine-tuning
    // s11.0:   signed value, 11 integer bits,  0 fraction bits
    unsigned short a_digital_gain;    // 0~3.999
    // u2.10: unsigned value,  2 integer bits, 10 fraction bits
    unsigned short a_digital_offset2; // offset for format
    // s11.0:   signed value, 11 integer bits,  0 fraction bits
    // ADC B
    unsigned short b_analog_clamp;    // ditto to ADC A
    unsigned short b_analog_gain;
    unsigned short b_digital_offset1;
    unsigned short b_digital_gain;
    unsigned short b_digital_offset2;
    // ADC C
    unsigned short c_analog_clamp;    // ditto to ADC A
    unsigned short c_analog_gain;
    unsigned short c_digital_offset1;
    unsigned short c_digital_gain;
    unsigned short c_digital_offset2;
    // ADC D
    unsigned short d_analog_clamp;    // ditto to ADC A
    unsigned short d_analog_gain;
    unsigned short d_digital_offset1;
    unsigned short d_digital_gain;
    unsigned short d_digital_offset2;
    unsigned int   reserved;          // bit[ 0]: TVAFE_ADC_CAL_VALID
} tvafe_adc_cal_t;

typedef struct tvafe_comp_wss_s {
    unsigned int wss1[5];
    unsigned int wss2[5];
} tvafe_comp_wss_t;

typedef struct tvafe_vga_edid_s {
    unsigned char value[256]; //256 byte EDID
} tvafe_vga_edid_t;

typedef struct tvafe_vga_parm_s {
    signed short clk_step;  // clock < 0, tune down clock freq
    // clock > 0, tune up clock freq
    unsigned short phase;     // phase is 0~31, it is absolute value
    signed short hpos_step; // hpos_step < 0, shift display to left
    // hpos_step > 0, shift display to right
    signed short vpos_step; // vpos_step < 0, shift display to top
    // vpos_step > 0, shift display to bottom
    unsigned int   vga_in_clean;  // flage for vga clean screen
} tvafe_vga_parm_t;

typedef struct tvafe_adc_comp_cal_s {
    struct tvafe_adc_cal_s comp_cal_val[3];
} tvafe_adc_comp_cal_t;

typedef struct tvafe_adc_cal_clamp_s {
    short a_analog_clamp_diff;
    short b_analog_clamp_diff;
    short c_analog_clamp_diff;
} tvafe_adc_cal_clamp_t;

typedef enum tvafe_cmd_status_e {
    TVAFE_CMD_STATUS_IDLE = 0,   // idle, be ready for TVIN_IOC_S_AFE_VGA_AUTO command
    TVAFE_CMD_STATUS_PROCESSING, // TVIN_IOC_S_AFE_VGA_AUTO command is in process
    TVAFE_CMD_STATUS_SUCCESSFUL, // TVIN_IOC_S_AFE_VGA_AUTO command is done with success
    TVAFE_CMD_STATUS_FAILED,     // TVIN_IOC_S_AFE_VGA_AUTO command is done with failure
    TVAFE_CMD_STATUS_TERMINATED, // TVIN_IOC_S_AFE_VGA_AUTO command is terminated by others related
} tvafe_cmd_status_t;

typedef enum tvafe_cvbs_video_e {
    TVAFE_CVBS_VIDEO_HV_UNLOCKED = 0,
    TVAFE_CVBS_VIDEO_H_LOCKED,
    TVAFE_CVBS_VIDEO_V_LOCKED,
    TVAFE_CVBS_VIDEO_HV_LOCKED,
} tvafe_cvbs_video_t;

typedef enum tvin_sig_change_flag_e {
     TVIN_SIG_CHG_NONE = 0,
     TVIN_SIG_CHG_SDR2HDR = 0x01,
     TVIN_SIG_CHG_HDR2SDR = 0x02,
     TVIN_SIG_CHG_DV2NO = 0x04,
     TVIN_SIG_CHG_NO2DV = 0x08,
     TVIN_SIG_CHG_COLOR_FMT = 0x10,
     TVIN_SIG_CHG_RANGE = 0x20, /*color range:full or limit*/
     TVIN_SIG_CHG_BIT = 0x40, /*color bit deepth: 8,10,12 ...*/
     TVIN_SIG_CHG_VS_FRQ = 0x80,
     TVIN_SIG_CHG_DV_ALLM = 0x100,/*allm info*/
     TVIN_SIG_CHG_AFD   = 0x200,/*aspect ratio*/
     TVIN_SIG_CHG_STS = 0x80000000, /*sm state change*/
} tvin_sig_change_flag_t;

typedef struct vdin_event_info_s {
	/*enum tvin_sg_chg_flg*/
	unsigned int event_sts;
} vdin_event_info_t;

// ***************************************************************************
// *** function type definition **********************************************
// ***************************************************************************
typedef enum tv_path_type_e {
    TV_PATH_TYPE_DEFAULT,
    TV_PATH_TYPE_TVIN,
    TV_PATH_TYPE_MAX,
} tv_path_type_t;

typedef enum tv_path_status_e {
    TV_PATH_STATUS_NO_DEV = -2,
    TV_PATH_STATUS_ERROR = -1,
    TV_PATH_STATUS_INACTIVE = 0,
    TV_PATH_STATUS_ACTIVE = 1,
    TV_PATH_STATUS_MAX,
} tv_path_status_t;

enum {
    TV_PATH_VDIN_AMLVIDEO2_PPMGR_DEINTERLACE_AMVIDEO,
    TV_PATH_DECODER_AMLVIDEO2_PPMGR_DEINTERLACE_AMVIDEO,
};

// ***************************************************************************
// *** IOCTL command definition **********************************************
// ***************************************************************************

#define TVIN_IOC_MAGIC 'T'
//GENERAL
#define TVIN_IOC_OPEN               _IOW(TVIN_IOC_MAGIC, 0x01, struct tvin_parm_s)
#define TVIN_IOC_START_DEC          _IOW(TVIN_IOC_MAGIC, 0x02, struct tvin_parm_s)
#define TVIN_IOC_STOP_DEC           _IO( TVIN_IOC_MAGIC, 0x03)
#define TVIN_IOC_CLOSE              _IO( TVIN_IOC_MAGIC, 0x04)
#define TVIN_IOC_G_PARM             _IOR(TVIN_IOC_MAGIC, 0x05, struct tvin_parm_s)
#define TVIN_IOC_S_PARM             _IOW(TVIN_IOC_MAGIC, 0x06, struct tvin_parm_s)
#define TVIN_IOC_G_SIG_INFO         _IOR(TVIN_IOC_MAGIC, 0x07, struct tvin_info_s)
#define TVIN_IOC_G_BUF_INFO         _IOR(TVIN_IOC_MAGIC, 0x08, struct tvin_buf_info_s)
#define TVIN_IOC_START_GET_BUF      _IO( TVIN_IOC_MAGIC, 0x09)
#define TVIN_IOC_GET_BUF            _IOR(TVIN_IOC_MAGIC, 0x10, struct tvin_video_buf_s)
#define TVIN_IOC_PAUSE_DEC          _IO(TVIN_IOC_MAGIC, 0x41)
#define TVIN_IOC_RESUME_DEC         _IO(TVIN_IOC_MAGIC, 0x42)
#define TVIN_IOC_VF_REG             _IO(TVIN_IOC_MAGIC, 0x43)
#define TVIN_IOC_VF_UNREG           _IO(TVIN_IOC_MAGIC, 0x44)
#define TVIN_IOC_FREEZE_VF          _IO(TVIN_IOC_MAGIC, 0x45)
#define TVIN_IOC_UNFREEZE_VF        _IO(TVIN_IOC_MAGIC, 0x46)
#define TVIN_IOC_SNOWON             _IO(TVIN_IOC_MAGIC, 0x47)
#define TVIN_IOC_SNOWOFF            _IO(TVIN_IOC_MAGIC, 0x48)
#define TVIN_IOC_GET_COLOR_RANGE	_IOR(TVIN_IOC_MAGIC, 0X49, enum tvin_color_range_e)
#define TVIN_IOC_SET_COLOR_RANGE	_IOW(TVIN_IOC_MAGIC, 0X4a, enum tvin_color_range_e)
#define TVIN_IOC_GAME_MODE          _IOW(TVIN_IOC_MAGIC, 0x4b, unsigned int)
#define TVIN_IOC_SET_AUTO_RATIO_EN  _IOW(TVIN_IOC_MAGIC, 0x4c, unsigned int)
#define TVIN_IOC_GET_LATENCY_MODE   _IOR(TVIN_IOC_MAGIC, 0X4d, struct tvin_latency_s)
#define TVIN_IOC_G_FRONTEND_INFO    _IOR(TVIN_IOC_MAGIC, 0x4e, struct tvin_frontend_info_s)
#define TVIN_IOC_G_EVENT_INFO       _IOW(TVIN_IOC_MAGIC, 0x0a, struct vdin_event_info_s)

//TVAFE
#define TVIN_IOC_S_AFE_ADC_CAL      _IOW(TVIN_IOC_MAGIC, 0x11, struct tvafe_adc_cal_s)
#define TVIN_IOC_G_AFE_ADC_CAL      _IOR(TVIN_IOC_MAGIC, 0x12, struct tvafe_adc_cal_s)
#define TVIN_IOC_G_AFE_COMP_WSS     _IOR(TVIN_IOC_MAGIC, 0x13, struct tvafe_comp_wss_s)
#define TVIN_IOC_S_AFE_VGA_EDID     _IOW(TVIN_IOC_MAGIC, 0x14, struct tvafe_vga_edid_s)
#define TVIN_IOC_G_AFE_VGA_EDID     _IOR(TVIN_IOC_MAGIC, 0x15, struct tvafe_vga_edid_s)
#define TVIN_IOC_S_AFE_VGA_PARM     _IOW(TVIN_IOC_MAGIC, 0x16, struct tvafe_vga_parm_s)
#define TVIN_IOC_G_AFE_VGA_PARM     _IOR(TVIN_IOC_MAGIC, 0x17, struct tvafe_vga_parm_s)
#define TVIN_IOC_S_AFE_VGA_AUTO     _IO( TVIN_IOC_MAGIC, 0x18)
#define TVIN_IOC_G_AFE_CMD_STATUS   _IOR(TVIN_IOC_MAGIC, 0x19, enum tvafe_cmd_status_e)
#define TVIN_IOC_G_AFE_CVBS_LOCK    _IOR(TVIN_IOC_MAGIC, 0x1a, enum tvafe_cvbs_video_e)
#define TVIN_IOC_S_AFE_CVBS_STD     _IOW(TVIN_IOC_MAGIC, 0x1b, enum tvin_sig_fmt_e)
#define TVIN_IOC_CALLMASTER_SET     _IOW(TVIN_IOC_MAGIC, 0x1c, enum tvin_port_e)
#define TVIN_IOC_CALLMASTER_GET     _IO( TVIN_IOC_MAGIC, 0x1d)
#define TVIN_IOC_S_AFE_ADC_COMP_CAL _IOW(TVIN_IOC_MAGIC, 0x1e, struct tvafe_adc_comp_cal_s)
#define TVIN_IOC_G_AFE_ADC_COMP_CAL _IOR(TVIN_IOC_MAGIC, 0x1f, struct tvafe_adc_comp_cal_s)
#define TVIN_IOC_S_AFE_ADC_DIFF     _IOW(TVIN_IOC_MAGIC, 0x21, struct tvafe_adc_cal_clamp_s)
#define TVIN_IOC_S_AFE_SONWON       _IO(TVIN_IOC_MAGIC, 0x22)
#define TVIN_IOC_S_AFE_SONWOFF      _IO(TVIN_IOC_MAGIC, 0x23)

#define VDIN_DEV_PATH               "/dev/vdin0"
#define AFE_DEV_PATH                "/dev/tvafe0"
#define AMLVIDEO2_DEV_PATH          "/dev/video11"
#define SYS_VFM_MAP_PATH            "/sys/class/vfm/map"

class CTvin {
public:
    CTvin();
    ~CTvin();
    static CTvin *getInstance();
    int Tvin_OpenPort(tvin_port_t source_port);
    int Tvin_ClosePort(tvin_port_t source_port);
    int Tvin_StartDecoder(tvin_info_t info);
    int Tvin_StopDecoder(void);
    int Tvin_SwitchSnow(bool enable);
    int Tvin_GetSignalEventInfo(vdin_event_info_s *SignalEventInfo);
    int Tvin_GetSignalInfo(tvin_info_s *SignalInfo);
    int Tvin_GetVdinDeviceFd(void);
    int Tvin_CheckVideoPathComplete(tv_path_type_t path_type);
    int Tvin_AddVideoPath(int selPath);
    int Tvin_RemoveVideoPath(tv_path_type_t pathtype);
    void Tvin_LoadSourceInputToPortMap(void);
    tv_source_input_type_t Tvin_SourcePortToSourceInputType(tvin_port_t source_port);
    tv_source_input_type_t Tvin_SourceInputToSourceInputType(tv_source_input_t source_input);
    tvin_port_t Tvin_GetSourcePortBySourceType(tv_source_input_type_t source_type);
    tvin_port_t Tvin_GetSourcePortBySourceInput(tv_source_input_t source_input);
    unsigned int Tvin_TransPortStringToValue(const char *port_str);
    tv_source_input_t Tvin_PortToSourceInput(tvin_port_t port);
    tvin_port_id_t Tvin_GetHdmiPortIdBySourceInput(tv_source_input_t source_input);
    int Tvin_GetFrontendInfo(tvin_frontend_info_t *frontendInfo);
    int Tvin_SetColorRangeMode(tvin_color_range_t range_mode);
    int Tvin_GetColorRangeMode(void);
private:
    //VDIN
    int VDIN_OpenModule();
    int VDIN_CloseModule();
    int VDIN_DeviceIOCtl(int request, ...);
    int VDIN_OpenPort(tvin_port_t port);
    int VDIN_ClosePort();
    int VDIN_StartDec(tvin_parm_s *vdinParam);
    int VDIN_StopDec(void);
    int VDIN_GetSignalEventInfo(vdin_event_info_s *SignalEventInfo);
    int VDIN_GetSignalInfo(tvin_info_s *SignalInfo);
    int VDIN_AddPath(const char *videopath );
    int VDIN_RemovePath(tv_path_type_t pathtype);
    int VDIN_SetVdinParam(tvin_parm_s *vdinParam);
    int VDIN_GetVdinParam(tvin_parm_s *vdinParam);
    int VDIN_SetColorRangeMode(tvin_color_range_t range_mode);
    int VDIN_GetColorRangeMode(void);
    //AFE
    int AFE_OpenModule(void);
    int AFE_CloseModule(void );
    int AFE_DeviceIOCtl(int request, ...);

private:
    static CTvin *mInstance;
    int mVdin0DevFd;
    int mAfeDevFd;
    bool mDecoderStarted;
    int mSourceInputToPortMap[SOURCE_MAX];
    tvin_parm_t mTvinParam;
};
#endif
