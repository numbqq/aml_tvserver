/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */

#ifndef _CTVEVENT_H_
#define _CTVEVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

class CTvEvent {
public:
    static const int TV_EVENT_COMMOM = 0;
    static const int TV_EVENT_SCANNER = 1;
    static const int TV_EVENT_EPG = 2;//EPG
    static const int TV_EVENT_SOURCE_SWITCH = 3;
    static const int TV_EVENT_SIGLE_DETECT = 4;
    static const int TV_EVENT_ADC_CALIBRATION = 5;
    static const int TV_EVENT_VGA = 6;//VGA
    static const int TV_EVENT_3D_STATE = 7;//3D
    static const int TV_EVENT_AV_PLAYBACK = 8;//PLAYBACK EVENT MSG
    static const int TV_EVENT_SERIAL_COMMUNICATION = 9;
    static const int TV_EVENT_SOURCE_CONNECT = 10;
    static const int TV_EVENT_HDMIRX_CEC = 11;
    static const int TV_EVENT_BLOCK = 12;
    static const int TV_EVENT_CC = 13; //close caption
    static const int TV_EVENT_VCHIP = 14; //VCHIP
    static const int TV_EVENT_HDMI_IN_CAP = 15;
    static const int TV_EVENT_UPGRADE_FBC = 16;
    static const int TV_EVENT_2d4G_HEADSET = 17;
    static const int TV_EVENT_AV = 18;
    static const int TV_EVENT_SUBTITLE = 19;
    static const int TV_EVENT_SCANNING_FRAME_STABLE = 20;
    static const int TV_EVENT_FRONTEND = 21;
    static const int TV_EVENT_RECORDER = 22;
    static const int TV_EVENT_RRT = 23;//RRT
    static const int TV_EVENT_EAS = 24;//EAS
    static const int TV_EVENT_AUDIO_CB = 25;

    CTvEvent(int type) {
        mEventType = type;
    }
    ~CTvEvent() {

    }
    int getEventType() {
        return mEventType;
    }
private:
    int mEventType;
};

namespace  TvEvent {
    //events
    class SignalDetectEvent: public CTvEvent {
    public:
        SignalDetectEvent() : CTvEvent(CTvEvent::TV_EVENT_SIGLE_DETECT)
        {
            mSourceInput = 0;
            mFmt = 0;
            mTrans_fmt = 0;
            mStatus = 0;
            mDviFlag = 0;
        }
        ~SignalDetectEvent() {}
        int mSourceInput;
        int mFmt;
        int mTrans_fmt;
        int mStatus;
        int mDviFlag;
    };

    class SourceConnectEvent: public CTvEvent {
    public:
        SourceConnectEvent() : CTvEvent(CTvEvent::TV_EVENT_SOURCE_CONNECT)
        {
            mSourceInput = 0;
            connectionState = 0;
        }
        ~SourceConnectEvent() {}
        int mSourceInput;
        int connectionState;
    };

    class SourceSwitchEvent: public CTvEvent {
    public:
        SourceSwitchEvent() : CTvEvent(CTvEvent::TV_EVENT_SOURCE_SWITCH)
        {
            DestSourceInput = 0;
            DestSourcePortNum = 0;
        }
        ~SourceSwitchEvent() {}
        int DestSourceInput;
        int DestSourcePortNum;
    };
};

#ifdef __cplusplus
}
#endif

#endif
