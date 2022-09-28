/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvClientWrapper"

#include <vector>
#include "TvClientWrapper.h"
#include "TvClient.h"
#include "CTvClientLog.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TvClientEventCallbackFunc)(event_type_t eventType, void *eventData);

class TvClientWrapper: public TvClient::TvClientIObserver {
public:
    TvClientWrapper() {
        mpTvClient = TvClient::GetInstance();
        mpTvClient->setTvClientObserver(this);
    }

    ~TvClientWrapper() {
        LOGD("%s.\n", __FUNCTION__);
        if (mpTvClient != NULL) {
            delete mpTvClient;
            mpTvClient = NULL;
        }
    }

    int RegisterCallback(TvClientEventCallbackFunc eventCallbackFunc) {
        if (eventCallbackFunc == NULL) {
            LOGD("eventCallbackFunc is NULL.\n");
            return -1;
        } else {
            mEventCallbackFunc = eventCallbackFunc;
            return 0;
        }
    }

    int StartTv(tv_source_input_t source) {
        return mpTvClient->StartTv(source);
    }

    int StopTv(tv_source_input_t source) {
        return mpTvClient->StopTv(source);
    }

    int SetEdidVersion(tv_source_input_t source, int edidVer) {
        return mpTvClient->SetEdidVersion(source, edidVer);
    }

    int GetEdidVersion(tv_source_input_t source) {
        return mpTvClient->GetEdidVersion(source);
    }

    int SetEdidData(tv_source_input_t source, char *dataBuf) {
        return mpTvClient->SetEdidData(source, dataBuf);
    }

    int GetEdidData(tv_source_input_t source,char *dataBuf)
    {
        LOGD("%s\n", __FUNCTION__);
        char buf[512] = {0};
        int ret = mpTvClient->GetEdidData(source, buf);
        if (ret < 0) {
            LOGD("%s failed.\n", __FUNCTION__);
        } else {
            memcpy(dataBuf, buf, sizeof(buf));
        }

        return ret;
    }

    int SetVdinWorkMode(vdin_work_mode_t vdinWorkMode)
    {
        return mpTvClient->SetVdinWorkMode(vdinWorkMode);
    }

    int GetCurrentSourceFrameHeight()
    {
        return mpTvClient->GetCurrentSourceFrameHeight();
    }

    int GetCurrentSourceFrameWidth()
    {
        return mpTvClient->GetCurrentSourceFrameWidth();
    }

    int GetCurrentSourceFrameFps()
    {
        return mpTvClient->GetCurrentSourceFrameFps();
    }

    int GetCurrentSourceColorDepth()
    {
        return mpTvClient->GetCurrentSourceColorDepth();
    }

    tvin_aspect_ratio_t GetCurrentSourceAspectRatio()
    {
        return mpTvClient->GetCurrentSourceAspectRatio();
    }

    tvin_color_fmt_t GetCurrentSourceColorFormat()
    {
        return mpTvClient->GetCurrentSourceColorFormat();
    }

    tvin_color_range_t GetCurrentSourceColorRange()
    {
        return mpTvClient->GetCurrentSourceColorRange();
    }

    tvin_line_scan_mode_t GetCurrentSourceLineScanMode()
    {
        return mpTvClient->GetCurrentSourceLineScanMode();
    }

    int GetSourceConnectStatus(tv_source_input_t source)
    {
        return mpTvClient->GetSourceConnectStatus(source);
    }

    void onTvClientEvent(CTvEvent &event) {
        int eventType = event.getEventType();
        LOGD("%s: eventType: %d.\n", __FUNCTION__, eventType);
        switch (eventType) {
        case CTvEvent::TV_EVENT_SIGLE_DETECT: {
            LOGD("%s: signal event.\n", __FUNCTION__);
            TvEvent::SignalDetectEvent *signalDetectEvent = (TvEvent::SignalDetectEvent *)(&event);
            SignalDetectCallback_t SignalDetectCallback;
            SignalDetectCallback.SourceInput = (tv_source_input_t)signalDetectEvent->mSourceInput;
            SignalDetectCallback.SignalFmt = (tvin_sig_fmt_t)signalDetectEvent->mFmt;
            SignalDetectCallback.TransFmt = (tvin_trans_fmt_t)signalDetectEvent->mTrans_fmt;
            SignalDetectCallback.SignalStatus = (tvin_sig_status_t)signalDetectEvent->mStatus;
            SignalDetectCallback.isDviSignal = signalDetectEvent->mDviFlag;
            mEventCallbackFunc(TV_EVENT_TYPE_SIGLE_DETECT, &SignalDetectCallback);
            break;
            }
        case CTvEvent::TV_EVENT_SOURCE_CONNECT: {
            LOGD("%s: connect event.\n", __FUNCTION__);
            TvEvent::SourceConnectEvent *sourceConnectEvent = (TvEvent::SourceConnectEvent *)(&event);
            SourceConnectCallback_t SourceConnectCallback;
            SourceConnectCallback.SourceInput = (tv_source_input_t)sourceConnectEvent->mSourceInput;
            SourceConnectCallback.ConnectionState = sourceConnectEvent->connectionState;

            mEventCallbackFunc(TV_EVENT_TYPE_SOURCE_CONNECT, &SourceConnectCallback);
            break;
            }
        default:
            LOGD("invalid event!\n", __FUNCTION__);
            break;
        }
        return;
    }

    TvClient *mpTvClient;
    TvClientEventCallbackFunc mEventCallbackFunc;
};

static void HandleTvClientEvent(event_type_t eventType, void *eventData) {
    if (eventType == TV_EVENT_TYPE_SIGLE_DETECT) {
        SignalDetectCallback_t *SignalDetectCallback = (SignalDetectCallback_t *)(eventData);
        LOGD("%s: source: %d, signalFmt: %d, transFmt: %d, status: %d, isDVI: %d.\n", __FUNCTION__,
                                                   SignalDetectCallback->SourceInput,
                                                   SignalDetectCallback->SignalFmt,
                                                   SignalDetectCallback->TransFmt,
                                                   SignalDetectCallback->SignalStatus,
                                                   SignalDetectCallback->isDviSignal);
    } else if (eventType == TV_EVENT_TYPE_SOURCE_CONNECT) {
        SourceConnectCallback_t *SourceConnectCallback = (SourceConnectCallback_t *)eventData;
        LOGD("%s: source: %d, connectStatus: %d\n", __FUNCTION__,
                  SourceConnectCallback->SourceInput, SourceConnectCallback->ConnectionState);
    } else {
        LOGD("%s: invalid event.\n", __FUNCTION__);
    }
    mEventCallback(eventType, eventData);
}

/**************************************wrapper api*******************************************/
struct TvClientWrapper_t {
    TvClientWrapper tvClientWrapper;
};

struct TvClientWrapper_t *GetInstance(void)
{
    LOGD("%s: start GetInstance.\n", __FUNCTION__);
    struct TvClientWrapper_t *pTvClientWrapper = new struct TvClientWrapper_t;
    pTvClientWrapper->tvClientWrapper.RegisterCallback(HandleTvClientEvent);

    return pTvClientWrapper;
}

void ReleaseInstance(struct TvClientWrapper_t **ppInstance)
{
    delete *ppInstance;
    *ppInstance = 0;
}

int StartTv(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source)
{
    return pTvClientWrapper->tvClientWrapper.StartTv(source);
}

int StopTv(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source)
{
    return pTvClientWrapper->tvClientWrapper.StopTv(source);
}

int SetEdidVersion(TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source, int edidVer)
{
    return pTvClientWrapper->tvClientWrapper.SetEdidVersion(source, edidVer);
}

int GetEdidVersion(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source)
{
    return pTvClientWrapper->tvClientWrapper.GetEdidVersion(source);
}

int SetEdidData(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source, char *dataBuf)
{
    return pTvClientWrapper->tvClientWrapper.SetEdidData(source, dataBuf);
}

int GetEdidData(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source, char *dataBuf)
{
    return pTvClientWrapper->tvClientWrapper.GetEdidData(source, dataBuf);
}

int setTvEventCallback(EventCallback Callback)
{
    if (Callback == NULL) {
        LOGD("%s: Callback is NULL.\n", __FUNCTION__);
    } else {
        mEventCallback = Callback;
    }

    return 0;
}

int SetVdinWorkMode(struct TvClientWrapper_t *pTvClientWrapper, vdin_work_mode_t vdinWorkMode)
{
    return pTvClientWrapper->tvClientWrapper.SetVdinWorkMode(vdinWorkMode);
}

int GetCurrentSourceFrameHeight(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceFrameHeight();
}

int GetCurrentSourceFrameWidth(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceFrameWidth();
}

int GetCurrentSourceFrameFps(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceFrameFps();
}

int GetCurrentSourceColorDepth(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceColorDepth();
}

tvin_aspect_ratio_t GetCurrentSourceAspectRatio(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceAspectRatio();
}

tvin_color_fmt_t GetCurrentSourceColorFormat(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceColorFormat();
}

tvin_color_range_t GetCurrentSourceColorRange(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceColorRange();
}

tvin_line_scan_mode_t GetCurrentSourceLineScanMode(struct TvClientWrapper_t *pTvClientWrapper)
{
    return pTvClientWrapper->tvClientWrapper.GetCurrentSourceLineScanMode();
}

int GetSourceConnectStatus(struct TvClientWrapper_t *pTvClientWrapper, tv_source_input_t source)
{
    return pTvClientWrapper->tvClientWrapper.GetSourceConnectStatus(source);
}

#ifdef __cplusplus
};
#endif
