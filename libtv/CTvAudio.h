/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */

#ifndef _CTVAUDIO_H
#define _CTVAUDIO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <hardware/audio.h>
#include <system/audio.h>

class CTvAudio {
public:
    static CTvAudio *getInstance();
    void create_audio_patch(int device_type);
    void release_audio_patch();
private:
    struct audio_port_config source;
    struct audio_port_config sink;
    audio_patch_handle_t patch_handle;
    audio_hw_device_t *device;
    bool iSInit;
    static CTvAudio *mInstance;
    CTvAudio();
    ~CTvAudio();
};
#endif