/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#ifndef _CAM_VIDEO_H
#define _CAM_VIDEO_H

#define AM_VIDEO_PATH           "/dev/amvideo"

// ***************************************************************************
// ********************* enum type definition ********************************
// ***************************************************************************
typedef enum video_layer_status_e {
    VIDEO_LAYER_STATUS_ENABLE,
    VIDEO_LAYER_STATUS_DISABLE,
    VIDEO_LAYER_STATUS_ENABLE_AND_CLEAN,
    VIDEO_LAYER_STATUS_MAX,
} videolayer_status_t;

typedef enum video_global_output_mode_e {
    VIDEO_GLOBAL_OUTPUT_MODE_DISABLE,
    VIDEO_GLOBAL_OUTPUT_MODE_ENABLE,
    VIDEO_GLOBAL_OUTPUT_MODE_MAX,
} video_global_output_mode_t;

// ***************************************************************************
// ********************* struct type definition ******************************
// ***************************************************************************


// ***************************************************************************
// ********************* IOCTL command definition ****************************
// ***************************************************************************
#define AMSTREAM_IOC_MAGIC  'S'
#define AMSTREAM_IOC_GLOBAL_GET_VIDEO_OUTPUT    _IOR(AMSTREAM_IOC_MAGIC, 0x21, int)
#define AMSTREAM_IOC_GLOBAL_SET_VIDEO_OUTPUT    _IOW(AMSTREAM_IOC_MAGIC, 0x22, int)
#define AMSTREAM_IOC_GET_VIDEO_DISABLE_MODE     _IOR(AMSTREAM_IOC_MAGIC, 0x48, int)
#define AMSTREAM_IOC_SET_VIDEO_DISABLE_MODE     _IOW(AMSTREAM_IOC_MAGIC, 0x49, int)
#define AMSTREAM_IOC_GET_OMX_INFO               _IOR(AMSTREAM_IOC_MAGIC, 0xb2, unsigned int)
#define AMSTREAM_IOC_GLOBAL_GET_VIDEOPIP_OUTPUT _IOR(AMSTREAM_IOC_MAGIC, 0x2b, int)
#define AMSTREAM_IOC_GLOBAL_SET_VIDEOPIP_OUTPUT _IOW(AMSTREAM_IOC_MAGIC, 0x2c, int)
#define AMSTREAM_IOC_GET_VIDEOPIP_DISABLE       _IOR(AMSTREAM_IOC_MAGIC, 0x2d, int)
#define AMSTREAM_IOC_SET_VIDEOPIP_DISABLE       _IOW(AMSTREAM_IOC_MAGIC, 0x2e, int)
#define AMSTREAM_IOC_SET_ZORDER                 _IOW(AMSTREAM_IOC_MAGIC, 0x38, unsigned int)
#define AMSTREAM_IOC_SET_PIP_ZORDER             _IOW(AMSTREAM_IOC_MAGIC, 0x36, unsigned int)

class CAmVideo {
public:
    CAmVideo();
    ~CAmVideo();
    int SetVideoLayerStatus(int status);
    int GetVideoLayerStatus(int *status);
    int SetVideoGlobalOutputMode(int mode);
    int GetVideoGlobalOutputMode(int *mode);
private:
    int AmVideoOpenMoudle(void);
    int AmVideoCloseMoudle(void);
    int AmVideoDeviceIOCtl(int request, ...);
    int mAmVideoFd;
};
#endif
