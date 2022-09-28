/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */

#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "CVpp"

#include "CVpp.h"
#include <CTvLog.h>
#include "tvutils.h"

CVpp *CVpp::mInstance;
CVpp *CVpp::getInstance()
{
    if (NULL == mInstance) mInstance = new CVpp();
    return mInstance;
}

CVpp::CVpp()
{

}

CVpp::~CVpp()
{

}

int CVpp::VPP_setVideoColor(bool needColor)
{
    int ret = 0 ;
    LOGD("VPP_setVideoColor %d\n", needColor);
    if (needColor) {
        ret = setVideoColor(1);//default blue
    } else {
        ret = setVideoColor(3);//set black test pattern and disable video
    }
    return ret;
}

int CVpp::setVideoColor(int color)
{
    LOGD("%s:  %d\n", __FUNCTION__, color);
    int ret = 0;
    switch (color) {
        case VIDEO_LAYER_COLOR_BLACK:
            tvWriteSysfs(VIDEO_DISABLE_VIDEO, "1");
            ret = setVideoColor(0, 16, 128, 128);
            break;
        case VIDEO_LAYER_COLOR_BLUE:
            tvWriteSysfs(VIDEO_DISABLE_VIDEO, "1");
            ret = setVideoColor(0, 41, 240, 110);
            break;
        default:
            ret = setVideoColor(0, 16, 128, 128);
            tvWriteSysfs(VIDEO_DISABLE_VIDEO, "2");
            break;
    }
    return ret;
}

int CVpp::setVideoColor (int vdin_blending_mask, int y, int u, int v )
{
    int ret = 0;
    unsigned long value = vdin_blending_mask << 24;
    value |= ( unsigned int ) ( y << 16 ) | ( unsigned int ) ( u << 8 ) | ( unsigned int ) ( v );

    char val[64] = {0};
    sprintf(val, "0x%lx", ( unsigned long ) value);
    ret = tvWriteSysfs(VIDEO_TEST_SCREEN, val);
    return ret;
}

int CVpp::setVideoScreenMode ( int value )
{
    char val[64] = {0};
    sprintf(val, "%d", value);
    tvWriteSysfs(VIDEO_SCREEN_MODE, val);
    return 0;
}

int CVpp::getVideoScreenMode()
{
    char buf[32] = {0};

    tvReadSysfs(VIDEO_SCREEN_MODE, buf);
    return atoi(buf);
}


