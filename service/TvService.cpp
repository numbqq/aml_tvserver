/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvService"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "TvService.h"
#include "TvCommon.h"
#include "CTvLog.h"
#include "tvcmd.h"

#ifdef __cplusplus
extern "C" {
#endif

pthread_mutex_t tvservice_mutex = PTHREAD_MUTEX_INITIALIZER;

TvService *mInstance = NULL;
TvService *TvService::GetInstance() {
    if (mInstance == NULL) {
        mInstance = new TvService();
    }

    return mInstance;
}

TvService::TvService() {
    mpTv = new CTv();
    mpTv->setTvObserver(this);
}

TvService::~TvService() {

}

void TvService::onTvEvent(CTvEvent &event) {
    int eventType = event.getEventType();
    LOGD("%s: eventType: %d\n", __FUNCTION__, eventType);
    switch (eventType) {
    case CTvEvent::TV_EVENT_SIGLE_DETECT:
        SendSignalForSignalDetectEvent(event);
        break;
    case CTvEvent::TV_EVENT_SOURCE_CONNECT:
        SendSignalForSourceConnectEvent(event);
        break;
    default :
        LOGE("TvService: invalie event type!\n");
        break;
    }
    return;
}

int TvService::SendSignalForSourceConnectEvent(CTvEvent &event)
{
    LOGD("%s\n", __FUNCTION__);
    Parcel send, reply;

    TvEvent::SourceConnectEvent *sourceConnectEvent = (TvEvent::SourceConnectEvent *)(&event);
    int clientSize = mTvServiceCallBack.size();
    LOGD("%s: now has %d tvclient.\n", __FUNCTION__, clientSize);
    int i = 0;
    for (i = 0; i < clientSize; i++) {
        if (mTvServiceCallBack[i] != NULL) {
            send.writeInt32(sourceConnectEvent->mSourceInput);
            send.writeInt32(sourceConnectEvent->connectionState);
            LOGD("send source evt(%d,%d) to client.\n",
                 sourceConnectEvent->mSourceInput, sourceConnectEvent->connectionState);
            mTvServiceCallBack[i]->transact(EVT_SRC_CT_CB, send, &reply);
        } else {
            LOGD("Event callback is null.\n");
        }
    }
    return 0;
}

int TvService::SendSignalForSignalDetectEvent(CTvEvent &event)
{
    LOGD("%s\n", __FUNCTION__);
    Parcel send, reply;

    TvEvent::SignalDetectEvent *signalDetectEvent = (TvEvent::SignalDetectEvent *)(&event);
    int clientSize = mTvServiceCallBack.size();
    LOGD("%s: now has %d tvclient.\n", __FUNCTION__, clientSize);
    int i = 0;
    for (i = 0; i < clientSize; i++) {
        if (mTvServiceCallBack[i] != NULL) {
            send.writeInt32(signalDetectEvent->mSourceInput);
            send.writeInt32(signalDetectEvent->mFmt);
            send.writeInt32(signalDetectEvent->mTrans_fmt);
            send.writeInt32(signalDetectEvent->mStatus);
            send.writeInt32(signalDetectEvent->mDviFlag);
            mTvServiceCallBack[i]->transact(EVT_SIG_DT_CB, send, &reply);
        } else {
            LOGD("Event callback is null.\n");
        }
    }
    return 0;
}

int TvService::ParserTvCommand(const char *commandData)
{
    pthread_mutex_lock(&tvservice_mutex);

    int ret = 0;
    tvin_frontend_info_t frontendInfo;
    memset(&frontendInfo, 0x0, sizeof(tvin_frontend_info_t));
    tvin_info_t tvinSignalInfo;
    memset(&tvinSignalInfo, 0x0, sizeof(tvin_info_t));
    char cmdbuff[1024];
    memset(cmdbuff, 0x0, sizeof(cmdbuff));
    memcpy(cmdbuff, commandData, strlen(commandData));
    const char *delimitation = ".";
    char *temp = strtok(cmdbuff, delimitation);
    LOGD("%s: cmdType = %s\n", __FUNCTION__, temp);
    if (strcmp(temp, "control") == 0) {
        LOGD("%s: control cmd!\n", __FUNCTION__);
        temp = strtok(NULL, delimitation);
        int moudleID = atoi(temp);
        if (moudleID == TV_CONTROL_START_TV) {
            temp = strtok(NULL, delimitation);
            tv_source_input_t startSource = (tv_source_input_t)atoi(temp);
            ret = mpTv->StartTv(startSource);
        } else if (moudleID == TV_CONTROL_STOP_TV) {
            temp = strtok(NULL, delimitation);
            tv_source_input_t stopSource = (tv_source_input_t)atoi(temp);
            ret = mpTv->StopTv(stopSource);
        } else if (moudleID == TV_CONTROL_VDIN_WORK_MODE_SET) {
            temp = strtok(NULL, delimitation);
            vdin_work_mode_t setVdinWorkMode = (vdin_work_mode_t)atoi(temp);
            ret = mpTv->SetVdinWorkMode(setVdinWorkMode);
        } else if (moudleID == TV_CONTROL_GET_FRAME_HEIGHT) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = frontendInfo.height;
        } else if (moudleID == TV_CONTROL_GET_FRAME_WIDTH) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = frontendInfo.width;
        } else if (moudleID == TV_CONTROL_GET_FRAME_RATE) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = frontendInfo.fps;
        } else if (moudleID == TV_CONTROL_GET_COLOR_DEPTH) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = frontendInfo.colordepth;
        } else if (moudleID == TV_CONTROL_GET_LINE_SCAN_MODE) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = frontendInfo.scan_mode;
        } else if (moudleID == TV_CONTROL_GET_CONNECT_STATUS) {
            temp = strtok(NULL, delimitation);
            tv_source_input_t source = (tv_source_input_t)atoi(temp);
            ret = mpTv->GetSourceConnectStatus(source);
        } else {
            LOGD("%s: invalid sourec cmd!\n", __FUNCTION__);
        }
    } else if (strcmp(temp, "edid") == 0) {
        LOGD("%s: EDID cmd!\n", __FUNCTION__);
        temp = strtok(NULL, delimitation);
        if (strcmp(temp, "set") == 0) {
            temp = strtok(NULL, delimitation);
            int moudleID = atoi(temp);
            temp = strtok(NULL, delimitation);
            tv_source_input_t setSource = (tv_source_input_t)atoi(temp);
            if (moudleID == HDMI_EDID_VER_SET) {
                temp = strtok(NULL, delimitation);
                tv_hdmi_edid_version_t setVersion = (tv_hdmi_edid_version_t)atoi(temp);
                ret = mpTv->SetEdidVersion(setSource, setVersion);
            } else {
                LOGD("%s: invalid EDID set cmd!\n", __FUNCTION__);
                ret = 0;
            }
        } else if (strcmp(temp, "get") == 0) {
            temp = strtok(NULL, delimitation);
            int moudleID = atoi(temp);
            temp = strtok(NULL, delimitation);
            tv_source_input_t getSource = (tv_source_input_t)atoi(temp);
            if (moudleID == HDMI_EDID_VER_GET) {
                ret = mpTv->GetEdidVersion(getSource);
            } else {
                LOGD("%s: invalid EDID get cmd!\n", __FUNCTION__);
                ret = 0;
            }
        } else {
            LOGD("%s: invalid cmd!\n", __FUNCTION__);
            ret = 0;
        }
    } else if (strcmp(temp, "hdmi") == 0) {
        LOGD("%s: hdmi cmd!\n", __FUNCTION__);
        temp = strtok(NULL, delimitation);
        int moudleID = atoi(temp);
        if (moudleID == HDMI_GET_COLOR_FORMAT) {
            tvinSignalInfo = mpTv->GetCurrentSourceInfo();
            ret = tvinSignalInfo.cfmt;
        } else if (moudleID == HDMI_GET_COLOR_RANGE) {
            mpTv->GetFrontendInfo(&frontendInfo);
            ret = mpTv->GetColorRangeMode();
        } else if (moudleID == HDMI_GET_COLOR_DEPTH) {
            tvinSignalInfo = mpTv->GetCurrentSourceInfo();
            ret = frontendInfo.colordepth;
        } else if (moudleID == HDMI_GET_ASPECT_RATIO) {
            tvinSignalInfo = mpTv->GetCurrentSourceInfo();
            ret = tvinSignalInfo.aspect_ratio;
        } else {
            LOGD("%s: invalid hdmi cmd!\n", __FUNCTION__);
            ret = 0;
        }
    } else {
        LOGD("%s: invalie cmdType!\n", __FUNCTION__);
    }

    pthread_mutex_unlock(&tvservice_mutex);
    return ret;
}

int TvService::ParserTvDataCommand(const char *commandBuf, unsigned char *dataBuf)
{
    pthread_mutex_lock(&tvservice_mutex);

    int ret = 0;
    char cmdbuff[1024], tempData[1024];
    memset(cmdbuff, 0x0, 1024);
    memcpy(cmdbuff, commandBuf, 1024);
    const char *delimitation = ".";
    char *temp = strtok(cmdbuff, delimitation);
    int cmdID = atoi(temp);
    LOGD("%s: cmdID = %d\n", __FUNCTION__, cmdID);
    if (cmdID == HDMI_EDID_DATA_SET) {
        temp = strtok(NULL, delimitation);
        tv_source_input_t setSource = (tv_source_input_t)atoi(temp);
        memcpy(tempData, dataBuf, 256);
        /*LOGD("%s: edid data print start.\n", __FUNCTION__);
        for (int i=0;i<256;i++) {
            printf("0x%x",tempData[i]);
        }
        LOGD("%s: edid data print end.\n", __FUNCTION__);*/
        ret = mpTv->SetEDIDData(setSource, tempData);
    } else if (cmdID == HDMI_EDID_DATA_GET) {
        temp = strtok(NULL, delimitation);
        tv_source_input_t getSource = (tv_source_input_t)atoi(temp);
        ret = mpTv->GetEDIDData(getSource, tempData);
        memcpy(dataBuf, tempData, 256);
    } else {
        LOGD("%s: invalid data cmd.\n", __FUNCTION__);
    }

    pthread_mutex_unlock(&tvservice_mutex);
    return ret;
}

int TvService::SetTvServiceCallBack(sp<IBinder> callBack)
{
    LOGD("%s\n", __FUNCTION__);
    int ret = -1;
    if (callBack != nullptr) {
        LOGD("%s: callBack is %p.\n", __FUNCTION__, callBack);
        int cookie = -1;
        int clientSize = mTvServiceCallBack.size();
        for (int i = 0; i < clientSize; i++) {
            if (mTvServiceCallBack[i] == NULL) {
                cookie = i;
                mTvServiceCallBack[i] = callBack;
                break;
            } else {
                LOGD("%s: mTvServiceCallBack[%d] has been register.\n", __FUNCTION__, i);
            }
        }

        if (cookie < 0) {
            cookie = clientSize;
            mTvServiceCallBack[clientSize] = callBack;
        }
        ret = cookie;
    } else {
        LOGD("%s: callBack is NULL.\n", __FUNCTION__);
    }
    return ret;
}

int TvService::RemoveTvServiceCallBack(int callBackId)
{
    LOGD("%s\n", __FUNCTION__);

    int clientSize = mTvServiceCallBack.size();
    if (callBackId < 0) {
        LOGD("%s: invalid callBackId.\n", __FUNCTION__);
    } else {
        mTvServiceCallBack.erase(callBackId);
    }

    clientSize = mTvServiceCallBack.size();
    LOGD("%s: now has %d tvclient after remove.\n", __FUNCTION__, clientSize);
    return 0;
}
status_t TvService::onTransact(uint32_t code,
                                const Parcel& data, Parcel* reply,
                                uint32_t flags) {
    LOGD("%s: cmd is %d.\n", __FUNCTION__, code);
    unsigned char dataBuf[1024] = {0};
    int count = 0;
    switch (code) {
        case CMD_TV_ACTION: {
            const char* command = data.readCString();
            int ret = ParserTvCommand(command);
            reply->writeInt32(ret);
            break;
        }
        case DATA_SET_ACTION: {
            const char* command = data.readCString();
            int setDataSize = data.readInt32();
            LOGD("%s: data size is %d.\n", __FUNCTION__, setDataSize);
            for (count=0;count<setDataSize;count++) {
                dataBuf[count] = (char)data.readInt32();
            }
            int ret = ParserTvDataCommand(command, dataBuf);
            reply->writeInt32(ret);
            break;
        }
        case DATA_GET_ACTION: {
            const char* command = data.readCString();
            int ret = ParserTvDataCommand(command, dataBuf);
            int getDataSize = sizeof(dataBuf);
            reply->writeInt32(getDataSize);
            for (count=0;count<getDataSize;count++) {
                reply->writeInt32(dataBuf[count]);
            }
            reply->writeInt32(ret);
            break;
        }
        case CMD_SET_TV_CB: {
            int tvServiceCallBackID = SetTvServiceCallBack(data.readStrongBinder());
            reply->writeInt32(tvServiceCallBackID);
            break;
        }
        case CMD_CLR_TV_CB: {
            RemoveTvServiceCallBack(data.readInt32());
            break;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }

    return (0);
}

#ifdef __cplusplus
}
#endif
