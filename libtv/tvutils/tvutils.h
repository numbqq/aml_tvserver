/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */

#ifndef _TV_UTILS_H_
#define _TV_UTILS_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <map>

#ifndef DVB_SUCCESS
#define DVB_SUCCESS     (0)
#endif

#ifndef DVB_FAILURE
#define DVB_FAILURE     (-1)
#endif

#define SYS_STR_LEN                         1024
#define PROPERTY_VALUE_MAX                  92
#define SYS_VIDEO_INUSE_PATH                "/sys/class/video/video_inuse"
#define AUDIO_STD_PATH                      "/sys/kernel/debug/aml_atvdemod/aud_std"
#define AUDIO_OUTMODE_PATH                  "/sys/kernel/debug/aml_atvdemod/aud_mode"
#define ATVDEMODE_DEBUG_PATH                "/sys/class/aml_atvdemod/atvdemod_debug"
#define AUDIO_STREAM_OUTMODE_PATH           "/sys/kernel/debug/aml_atvdemod/signal_audmode"

int tvReadSysfs(const char *path, char *value);
int tvWriteSysfs(const char *path, const char *value);
int tvWriteSysfs(const char *path, int value, int base=10);
extern bool isFileExist(const char *file_name);
extern int readSysfs(const char *path, char *value);
extern void writeSysfs(const char *path, const char *value);
extern int GetFileAttrIntValue(const char *fp, int flag = O_RDWR);
int ReadDataFromFile(char *fileName, int offset, int nsize, char *dataBuf);
int SaveDataToFile(char *fileName, int offset, int nsize, char *dataBuf);

#endif  //_TV_UTILS_H_
