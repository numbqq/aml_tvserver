/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */

#ifndef _TVCLIENT_WRAPPER_H__
#define _TVCLIENT_WRAPPER_H_

#include "TvCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum event_type_e {
    TV_EVENT_TYPE_COMMOM = 0,
    TV_EVENT_TYPE_SCANNER,
    TV_EVENT_TYPE_EPG,
    TV_EVENT_TYPE_SOURCE_SWITCH,
    TV_EVENT_TYPE_SIGLE_DETECT,
    TV_EVENT_TYPE_ADC_CALIBRATION,
    TV_EVENT_TYPE_VGA,
    TV_EVENT_TYPE_3D_STATE,
    TV_EVENT_TYPE_AV_PLAYBACK,
    TV_EVENT_TYPE_SERIAL_COMMUNICATION,
    TV_EVENT_TYPE_SOURCE_CONNECT,
    TV_EVENT_TYPE_HDMIRX_CEC,
    TV_EVENT_TYPE_BLOCK,
    TV_EVENT_TYPE_CC,
    TV_EVENT_TYPE_VCHIP,
    TV_EVENT_TYPE_HDMI_IN_CAP,
    TV_EVENT_TYPE_UPGRADE_FBC,
    TV_EVENT_TYPE_2d4G_HEADSET,
    TV_EVENT_TYPE_AV,
    TV_EVENT_TYPE_SUBTITLE,
    TV_EVENT_TYPE_SCANNING_FRAME_STABLE,
    TV_EVENT_TYPE_FRONTEND,
    TV_EVENT_TYPE_RECORDER,
    TV_EVENT_TYPE_RRT,
    TV_EVENT_TYPE_EAS,
    TV_EVENT_TYPE_AUDIO_CB,
    TV_EVENT_TYPE_MAX,
} event_type_t;

typedef struct {
    tv_source_input_t SourceInput;
    tvin_sig_fmt_t SignalFmt;
    tvin_trans_fmt_t TransFmt;
    tvin_sig_status_t SignalStatus;
    int isDviSignal; //1 is dvi signal
} SignalDetectCallback_t;

typedef struct {
    tv_source_input_t SourceInput;
    int ConnectionState; //1 is connect
} SourceConnectCallback_t;

typedef void (*EventCallback)(event_type_t eventType, void *eventData);
EventCallback mEventCallback;
struct TvClientWrapper_t;

extern struct TvClientWrapper_t *GetInstance(void);
extern void ReleaseInstance(struct TvClientWrapper_t **ppInstance);
extern int StartTv(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source);
extern int StopTv(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source);
extern int SetEdidVersion(struct TvClientWrapper_t *pTvClientWrappere, tv_source_input_t source, int edidVer);
extern int GetEdidVersion(struct TvClientWrapper_t *pTvClientWrappere, tv_source_input_t source);
extern int SetEdidData(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source, char *dataBuf);
extern int GetEdidData(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source,char *dataBuf);
extern int setTvEventCallback(EventCallback Callback);
extern int SetVdinWorkMode(struct TvClientWrapper_t *pTvClientWrapper, vdin_work_mode_t vdinWorkMode);
extern int GetCurrentSourceFrameHeight(struct TvClientWrapper_t *pTvClientWrapper);
extern int GetCurrentSourceFrameWidth(struct TvClientWrapper_t *pTvClientWrapper);
extern int GetCurrentSourceFrameFps(struct TvClientWrapper_t *pTvClientWrapper);
extern int GetCurrentSourceColorDepth(struct TvClientWrapper_t *pTvClientWrapper);
extern tvin_aspect_ratio_t GetCurrentSourceAspectRatio(struct TvClientWrapper_t *pTvClientWrapper);
extern tvin_color_fmt_t GetCurrentSourceColorFormat(struct TvClientWrapper_t *pTvClientWrapper);
extern tvin_color_range_t GetCurrentSourceColorRange(struct TvClientWrapper_t *pTvClientWrapper);
extern tvin_line_scan_mode_t GetCurrentSourceLineScanMode(struct TvClientWrapper_t *pTvClientWrapper);
extern int GetSourceConnectStatus(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source);
#ifdef __cplusplus
};
#endif

#endif
