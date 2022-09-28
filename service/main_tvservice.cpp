/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include "TvService.h"

int main(int argc, char **argv) {
    TvService *mpTvService = TvService::GetInstance();
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    sm->addService(String16("tvservice"), mpTvService);
    proc->startThreadPool();
    while (1) {
        usleep(30*1000*1000);
    }
}
