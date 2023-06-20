/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */

#include "CTvEvent.h"

#ifdef __cplusplus
extern "C" {
#endif

CTvEvent::CTvEvent(int type)
{
    mEventType = type;
}

CTvEvent::~CTvEvent()
{

}

int CTvEvent::getEventType()
{
    return mEventType;
}

#ifdef __cplusplus
}
#endif
