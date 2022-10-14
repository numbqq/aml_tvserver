/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "CAmVideo"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "CAmVideo.h"
#include "CTvLog.h"

CAmVideo::CAmVideo()
{
    mAmVideoFd = AmVideoOpenMoudle();
}

CAmVideo::~CAmVideo()
{
    AmVideoCloseMoudle();
}

int CAmVideo::AmVideoOpenMoudle(void)
{
    int fd = open(AM_VIDEO_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Open %s error(%s)!\n", AM_VIDEO_PATH, strerror(errno));
        return -1;
    }

    return fd;
}

int CAmVideo::AmVideoCloseMoudle(void)
{
    if (mAmVideoFd != -1) {
        close (mAmVideoFd);
        mAmVideoFd = -1;
    }

    return 0;
}

int CAmVideo::AmVideoDeviceIOCtl(int request, ...)
{
    int ret = -1;
    va_list ap;
    void *arg;
    va_start (ap, request);
    arg = va_arg (ap, void *);
    va_end (ap);

    ret = ioctl(mAmVideoFd, request, arg);

    return ret;
}

int CAmVideo::SetVideoLayerStatus(int status)
{
    int ret = -1;
    ret = AmVideoDeviceIOCtl(AMSTREAM_IOC_SET_VIDEO_DISABLE_MODE, &status);
    if (ret < 0) {
        LOGE("%s failed.\n", __FUNCTION__);
    } else {
        LOGD("%s success.\n", __FUNCTION__);
    }

    return ret;
}

int CAmVideo::GetVideoLayerStatus(int *status)
{
    int ret = -1;
    if (status == NULL) {
        LOGE("%s: param is NULL.\n", __FUNCTION__);
    } else {
        int tempVal = 0;
        ret = AmVideoDeviceIOCtl(AMSTREAM_IOC_GET_VIDEO_DISABLE_MODE, &tempVal);
        if (ret < 0) {
            LOGE("%s failed.\n", __FUNCTION__);
        } else {
            LOGD("%s success, status is %d.\n", __FUNCTION__, tempVal);
        }
        *status = tempVal;
    }
    return ret;
}

int CAmVideo::SetVideoGlobalOutputMode(int mode)
{
    int ret = -1;
    ret = AmVideoDeviceIOCtl(AMSTREAM_IOC_GLOBAL_SET_VIDEO_OUTPUT, &mode);
    if (ret < 0) {
        LOGE("%s failed.\n", __FUNCTION__);
    } else {
        LOGD("%s success.\n", __FUNCTION__);
    }

    return ret;
}

int CAmVideo::GetVideoGlobalOutputMode(int *mode)
{
    int ret = -1;
    if (mode == NULL) {
        LOGE("%s: param is NULL.\n", __FUNCTION__);
    } else {
        int tempVal = 0;
        ret = AmVideoDeviceIOCtl(AMSTREAM_IOC_GLOBAL_GET_VIDEO_OUTPUT, &tempVal);
        if (ret < 0) {
            LOGE("%s failed.\n", __FUNCTION__);
        } else {
            LOGD("%s success, status is %d.\n", __FUNCTION__, tempVal);
        }
        *mode = tempVal;
    }
    return ret;
}

