/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */
#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "CTvAudio"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CTvLog.h"
#include "CTvAudio.h"
#include "audio_if.h"

CTvAudio *CTvAudio::mInstance;

CTvAudio::CTvAudio()
{
    iSInit = true;
    int ret;
    ret = audio_hw_load_interface(&device);
    if (ret) {
        LOGE("%s %d error:%d\n", __func__, __LINE__, ret);
        iSInit = false;
    }
    LOGI("hw version: %x\n", device->common.version);
    LOGI("hal api version: %x\n", device->common.module->hal_api_version);
    LOGI("module id: %s\n", device->common.module->id);
    LOGI("module name: %s\n", device->common.module->name);

    if (device->get_supported_devices) {
        uint32_t support_dev = 0;
        support_dev = device->get_supported_devices(device);
        LOGI("supported device: %x\n", support_dev);
    }

    int inited = device->init_check(device);
    if (inited) {
        LOGE("device not inited, quit\n");
        iSInit = false;
    }
}

CTvAudio::~CTvAudio()
{
    if (iSInit) {
        audio_hw_unload_interface(device);
        iSInit = false;
    }
}
CTvAudio *CTvAudio::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new CTvAudio();
    }

    return mInstance;
}

void CTvAudio::create_audio_patch(int device_type)
{
    int ret;
    if (iSInit) {
        /* create the audio patch*/
        memset(&source, 0 , sizeof(struct audio_port_config));
        source.id = 1;
        source.role = AUDIO_PORT_ROLE_SOURCE;
        source.type = AUDIO_PORT_TYPE_DEVICE;
        source.ext.device.type = device_type;

        memset(&sink, 0 , sizeof(struct audio_port_config));
        sink.id = 2;
        sink.role = AUDIO_PORT_ROLE_SINK;
        sink.type = AUDIO_PORT_TYPE_DEVICE;
        sink.ext.device.type = AUDIO_DEVICE_OUT_SPEAKER;

        LOGD("create mix --> speaker patch...\n");
        ret = device->create_audio_patch(device, 1, &source, 1, &sink, &patch_handle);
        if (ret) {
            LOGE("fail ret:%d\n",ret);
        } else {
            LOGD("success\n");
        }
    }
}

void CTvAudio::release_audio_patch()
{
    if ((patch_handle) && (iSInit)) {
        int ret;
        LOGD("destroy patch...\n");
        ret = device->release_audio_patch(device, patch_handle);
        if (ret) {
            LOGE("fail ret:%d\n",ret);
        } else {
            LOGD("success\n");
        }
    }
}
