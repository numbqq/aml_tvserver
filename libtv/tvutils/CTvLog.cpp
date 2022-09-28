/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>


#include "CTvLog.h"

void get_time(char *buf)
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);  //the nses from 1970 to current
    tm nowTime;
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d:%03ld", nowTime.tm_year + 1900, nowTime.tm_mon+1, nowTime.tm_mday,
            nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec, time.tv_nsec/1000000);

    return;
}

int __tv_log_print(const char *moudle_tag, const char *level_tag, const char *class_tag, const char *fmt, ...)
{
    char buf[DEFAULT_LOG_BUFFER_LEN];
    char timeBuf[DEFAULT_LOG_BUFFER_LEN];
    get_time(timeBuf);

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, DEFAULT_LOG_BUFFER_LEN, fmt, ap);

    return printf("%s %d %ld %s %s [%s]: %s", timeBuf,
                                                getpid(),
                                                syscall(SYS_gettid),
                                                level_tag,
                                                moudle_tag,
                                                class_tag,
                                                buf);
}
