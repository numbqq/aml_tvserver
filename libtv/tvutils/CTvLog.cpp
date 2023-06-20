/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>


#include "CTvLog.h"

constexpr int LOG_LEVEL_ERROR = 1;
constexpr int LOG_LEVEL_WARNING = 2;
constexpr int LOG_LEVEL_INFO = 3;
constexpr int LOG_LEVEL_DEBUG = 4;
constexpr int LOG_LEVEL_DEFAULT = 5;

int tv_log_level = LOG_LEVEL_INFO;

static int __level_tag_to_int(const char *level_tag)
{
    switch (level_tag[0])
    {
        case 'D': return LOG_LEVEL_DEBUG;
        case 'I': return LOG_LEVEL_INFO;
        case 'W': return LOG_LEVEL_WARNING;
        case 'E': return LOG_LEVEL_ERROR;
        default: return LOG_LEVEL_DEFAULT;
    }
}

void init_tv_logging()
{
    if (const char* env_p = std::getenv("TV_LOG_LEVEL"))
    {
        long int level = atol(env_p);
        if (level < LOG_LEVEL_ERROR || level > LOG_LEVEL_DEFAULT)
            level = LOG_LEVEL_DEFAULT;
        tv_log_level = level;
    }
}

void get_time(char *buf)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    tm nowTime;
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d:%03ld", nowTime.tm_year + 1900, nowTime.tm_mon+1, nowTime.tm_mday,
            nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec, time.tv_usec/1000);

    return;
}

int __tv_log_print(const char *moudle_tag, const char *level_tag, const char *class_tag, const char *fmt, ...)
{
    if (level_tag && __level_tag_to_int(level_tag) > tv_log_level)
        return 0;

    char buf[DEFAULT_LOG_BUFFER_LEN];
    char timeBuf[DEFAULT_LOG_BUFFER_LEN];
    get_time(timeBuf);

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, DEFAULT_LOG_BUFFER_LEN, fmt, ap);

    fprintf(stderr, "%s %d %ld %s %s [%s]: %s", timeBuf,
                                            getpid(),
                                            syscall(SYS_gettid),
                                            level_tag,
                                            moudle_tag,
                                            class_tag,
                                            buf);

    va_end(ap);
    return 0;
}
