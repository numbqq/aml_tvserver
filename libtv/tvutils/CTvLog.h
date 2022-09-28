/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef _C_TV_LOG_H_
#define _C_TV_LOG_H_


//1024 + 128
#define DEFAULT_LOG_BUFFER_LEN 1152

extern int __tv_log_print(const char *moudle_tag, const char *level_tag ,const char *class_tag, const char *fmt, ...);

#ifdef LOG_MOUDLE_TAG

#undef LOGD
#ifndef LOGD
#define LOGD(...) \
    __tv_log_print(LOG_MOUDLE_TAG, "D", LOG_CLASS_TAG, __VA_ARGS__)
#endif

#undef LOGE
#ifndef LOGE
#define LOGE(...) \
    __tv_log_print(LOG_MOUDLE_TAG, "E", LOG_CLASS_TAG, __VA_ARGS__)
#endif

#undef LOGV
#ifndef LOGV
#define LOGV(...) \
    __tv_log_print(LOG_MOUDLE_TAG, "W", LOG_CLASS_TAG, __VA_ARGS__)
#endif

#undef LOGI
#ifndef LOGI
#define LOGI(...) \
    __tv_log_print(LOG_MOUDLE_TAG, "I", LOG_CLASS_TAG, __VA_ARGS__)
#endif

#else
#define LOGD(...) \
    __tv_log_print("TV", "D", " ", __VA_ARGS__)
#define LOGE(...) \
    __tv_log_print("TV", "E", " ", __VA_ARGS__)
#define LOGV(...) \
    __tv_log_print("TV", "W", " ", __VA_ARGS__)
#define LOGI(...) \
    __tv_log_print("TV", "I", " ", __VA_ARGS__)
#endif

#endif//end #ifndef _C_TV_LOG_H_