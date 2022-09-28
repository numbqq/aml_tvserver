/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */

#ifndef _C_VPP_H
#define _C_VPP_H
#include "CTvin.h"
#include "tvutils.h"
#include "TvCommon.h"

#define VIDEO_DISABLE_VIDEO "/sys/class/video/disable_video"
#define VIDEO_SCREEN_MODE   "/sys/class/video/screen_mode"
#define VIDEO_TEST_SCREEN   "/sys/class/video/test_screen"

typedef enum video_layer_color_e{
    VIDEO_LAYER_COLOR_BLACK   = 0,
    VIDEO_LAYER_COLOR_BLUE    = 1,
    VIDEO_LAYER_COLOR_MAX,
} video_layer_color_t;

class CVpp {

public:
    CVpp();
    ~CVpp();

    int VPP_setVideoColor(bool needColor);
    int setVideoColor (int color);
    int setVideoColor (int vdin_blending_mask, int y, int u, int v );
    int setVideoScreenMode ( int value );
    int getVideoScreenMode();

    static CVpp *getInstance();
private:
    static CVpp * mInstance;
};


#endif
