/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvClient"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

#include "TvClient.h"
#include "CTvClientLog.h"
#include "tvcmd.h"

const int RET_SUCCESS = 0;
const int RET_FAILED  = -1;

const int EVENT_SIGLE_DETECT = 4;
const int EVENT_SOURCE_CONNECT = 10;

pthread_mutex_t tvclient_mutex = PTHREAD_MUTEX_INITIALIZER;

TvClient *TvClient::mInstance;
TvClient *TvClient::GetInstance() {
    if (mInstance == NULL) {
        mInstance = new TvClient();
    }

    return mInstance;
}

TvClient::TvClient() {
    LOGD("%s.\n", __FUNCTION__);
    pthread_mutex_lock(&tvclient_mutex);
    sp<ProcessState> proc(ProcessState::self());
    proc->startThreadPool();
    Parcel send, reply;
    sp<IServiceManager> sm = defaultServiceManager();
    do {
        tvServicebinder = sm->getService(String16("tvservice"));
        if (tvServicebinder != 0) break;
        LOGD("TvClient: Waiting tvservice published.\n");
        usleep(500000);
    } while(true);
    LOGD("Connected to tvservice.\n");
    send.writeStrongBinder(sp<IBinder>(this));
    tvServicebinder->transact(CMD_SET_TV_CB, send, &reply);
    tvServicebinderId = reply.readInt32();
    pthread_mutex_unlock(&tvclient_mutex);
}

TvClient::~TvClient() {
    LOGD("%s.\n", __FUNCTION__);
    pthread_mutex_lock(&tvclient_mutex);
    if (tvServicebinder != NULL) {
        Parcel send, reply;
        send.writeInt32(tvServicebinderId);
        tvServicebinder->transact(CMD_CLR_TV_CB, send, &reply);
    }
    tvServicebinder = NULL;
    pthread_mutex_unlock(&tvclient_mutex);
}

int TvClient::SendMethodCall(char *CmdString)
{
    LOGD("%s.\n", __FUNCTION__);
    int ReturnVal = 0;
    Parcel send, reply;

    if (tvServicebinder != NULL) {
        send.writeCString(CmdString);
        if (tvServicebinder->transact(CMD_TV_ACTION, send, &reply) != 0) {
            LOGE("%s: tvServicebinder failed.\n", __FUNCTION__);
            ReturnVal = -1;
        } else {
            ReturnVal = reply.readInt32();
        }
    } else {
        LOGE("%s: tvServicebinder is NULL.\n", __FUNCTION__);
    }
    return ReturnVal;
}

int TvClient::SendTvClientEvent(CTvEvent &event)
{
    LOGD("%s\n", __FUNCTION__);

    int clientSize = mTvClientObserver.size();
    LOGD("%s: now has %d tvclient.\n", __FUNCTION__, clientSize);
    int i = 0;
    for (i = 0; i < clientSize; i++) {
        if (mTvClientObserver[i] != NULL) {
            mTvClientObserver[i]->onTvClientEvent(event);
        } else {
            LOGD("%s: mTvClientObserver[%d] is NULL.\n", __FUNCTION__, i, mTvClientObserver[i]);
        }
    }

    LOGD("send event for %d count TvClientObserver!\n", i);
    return 0;
}

int TvClient::HandSourceConnectEvent(const void* param)
{
    Parcel *parcel = (Parcel *) param;
    TvEvent::SourceConnectEvent event;
    event.mSourceInput = parcel->readInt32();
    event.connectionState = parcel->readInt32();
    mInstance->SendTvClientEvent(event);

    return 0;
}

int TvClient::HandSignalDetectEvent(const void* param)
{
    Parcel *parcel = (Parcel*) param;
    TvEvent::SignalDetectEvent event;
    event.mSourceInput = parcel->readInt32();
    event.mFmt = parcel->readInt32();
    event.mTrans_fmt = parcel->readInt32();
    event.mStatus = parcel->readInt32();
    event.mDviFlag = parcel->readInt32();
    mInstance->SendTvClientEvent(event);

    return 0;

}

int TvClient::setTvClientObserver(TvClientIObserver *observer)
{
    LOGD("%s\n", __FUNCTION__);
    if (observer != nullptr) {
        LOGD("%s: observer is %p.\n", __FUNCTION__, observer);
        int cookie = -1;
        int clientSize = mTvClientObserver.size();
        for (int i = 0; i < clientSize; i++) {
            if (mTvClientObserver[i] == NULL) {
                cookie = i;
                mTvClientObserver[i] = observer;
                break;
            } else {
                LOGD("%s: mTvClientObserver[%d] has been register.\n", __FUNCTION__, i);
            }
        }

        if (cookie < 0) {
            cookie = clientSize;
            mTvClientObserver[clientSize] = observer;
        }
    } else {
        LOGD("%s: observer is NULL.\n", __FUNCTION__);
    }

    return 0;
}

int TvClient::StartTv(tv_source_input_t source) {
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d.%d", TV_CONTROL_START_TV, source);
    return SendMethodCall(buf);
}

int TvClient::StopTv(tv_source_input_t source) {
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d.%d", TV_CONTROL_STOP_TV, source);
    return SendMethodCall(buf);
}

int TvClient::SetVdinWorkMode(vdin_work_mode_t vdinWorkMode)
{
    LOGD("%s\n", __FUNCTION__);
    char buf[512] = {0};
    sprintf(buf, "control.%d.%d", TV_CONTROL_VDIN_WORK_MODE_SET, vdinWorkMode);
    return SendMethodCall(buf);
}

int TvClient::SetEdidVersion(tv_source_input_t source, int edidVer)
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "edid.set.%d.%d.%d", HDMI_EDID_VER_SET, source, edidVer);
    return SendMethodCall(buf);
}

int TvClient::GetEdidVersion(tv_source_input_t source)
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "edid.get.%d.%d", HDMI_EDID_VER_GET, source);
    return SendMethodCall(buf);
}

int TvClient::SetEdidData(tv_source_input_t source, char *dataBuf)
{
    LOGD("%s\n", __FUNCTION__);
    char CmdString[32] = {0};
    sprintf(CmdString, "%d.%d.", HDMI_EDID_DATA_SET, source);

    int ret = -1;
    Parcel send, reply;
    if (tvServicebinder != NULL) {
        send.writeCString(CmdString);
        send.writeInt32(256);
        for (int i=0;i<256;i++) {
            send.writeInt32(dataBuf[i]);
        }
        if (tvServicebinder->transact(DATA_SET_ACTION, send, &reply) != 0) {
            LOGE("%s: tvServicebinder failed.\n", __FUNCTION__);
        } else {
            ret = reply.readInt32();
        }
    } else {
        LOGE("%s: tvServicebinder is NULL.\n", __FUNCTION__);
    }

    return ret;

}

int TvClient::GetEdidData(tv_source_input_t source, char *dataBuf)
{
    LOGD("%s\n", __FUNCTION__);
    char buf[512] = {0};
    sprintf(buf, "edid.get.%d.%d.%s", HDMI_EDID_DATA_GET, source, dataBuf);
    return SendMethodCall(buf);
}

int TvClient::GetCurrentSourceFrameHeight()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d", TV_CONTROL_GET_FRAME_HEIGHT);
    return SendMethodCall(buf);
}

int TvClient::GetCurrentSourceFrameWidth()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d", TV_CONTROL_GET_FRAME_WIDTH);
    return SendMethodCall(buf);
}

int TvClient::GetCurrentSourceFrameFps()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d", TV_CONTROL_GET_FRAME_RATE);
    return SendMethodCall(buf);
}

int TvClient::GetCurrentSourceColorDepth()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d", TV_CONTROL_GET_COLOR_DEPTH);
    return SendMethodCall(buf);
}

tvin_aspect_ratio_t TvClient::GetCurrentSourceAspectRatio()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "hdmi.%d", HDMI_GET_ASPECT_RATIO);
    return (tvin_aspect_ratio_t)SendMethodCall(buf);
}

tvin_color_fmt_t TvClient::GetCurrentSourceColorFormat()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "hdmi.%d", HDMI_GET_COLOR_FORMAT);
    return (tvin_color_fmt_t)SendMethodCall(buf);
}

tvin_color_range_t TvClient::GetCurrentSourceColorRange()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "hdmi.%d", HDMI_GET_COLOR_RANGE);
    return (tvin_color_range_t)SendMethodCall(buf);
}

tvin_line_scan_mode_t TvClient::GetCurrentSourceLineScanMode()
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d", TV_CONTROL_GET_LINE_SCAN_MODE);
    return (tvin_line_scan_mode_t)SendMethodCall(buf);
}

int TvClient::GetSourceConnectStatus(tv_source_input_t source)
{
    LOGD("%s\n", __FUNCTION__);
    char buf[32] = {0};
    sprintf(buf, "control.%d.%d", TV_CONTROL_GET_CONNECT_STATUS, source);
    return SendMethodCall(buf);
}

status_t TvClient::onTransact(uint32_t code,
                                const Parcel& data, Parcel* reply,
                                uint32_t flags) {
    pthread_mutex_lock(&tvclient_mutex);
    LOGD("TvClient get tanscode: %u\n", code);
    switch (code) {
        case EVT_SRC_CT_CB: {
            HandSourceConnectEvent(&data);
            break;
        }
        case EVT_SIG_DT_CB: {
            HandSignalDetectEvent(&data);
            break;
        }
        case CMD_START:
        default:
            pthread_mutex_unlock(&tvclient_mutex);
            return BBinder::onTransact(code, data, reply, flags);
    }
    pthread_mutex_unlock(&tvclient_mutex);
    return (0);
}
