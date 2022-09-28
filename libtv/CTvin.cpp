/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */

#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "CTvin"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "CTvin.h"
#include "TvConfigManager.h"
#include "tvutils.h"
#include "CTvLog.h"

CTvin *CTvin::mInstance;

CTvin *CTvin::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new CTvin();
    }

    return mInstance;
}

CTvin::CTvin()
{
    mDecoderStarted = false;
    mVdin0DevFd = VDIN_OpenModule();
    mAfeDevFd = AFE_OpenModule();
    memset(&mTvinParam, 0, sizeof(tvin_parm_t));
    memset(mSourceInputToPortMap, SOURCE_INVALID, sizeof(mSourceInputToPortMap));
}

CTvin::~CTvin()
{
    VDIN_CloseModule();
    AFE_CloseModule();
}

int CTvin::VDIN_OpenModule()
{
    int fd = open (VDIN_DEV_PATH, O_RDWR );
    if ( fd < 0 ) {
        LOGE("Open %s error(%s)!\n", VDIN_DEV_PATH, strerror(errno));
        return -1;
    }
    LOGD ( "%s: Open %s module fd = [%d]\n",__FUNCTION__, VDIN_DEV_PATH, fd );
    return fd;
}

int CTvin::VDIN_CloseModule()
{
    if ( mVdin0DevFd != -1 ) {
        close ( mVdin0DevFd );
        mVdin0DevFd = -1;
    }

    return 0;
}

int CTvin::VDIN_DeviceIOCtl ( int request, ... )
{
    int tmp_ret = -1;
    va_list ap;
    void *arg;

    if (mVdin0DevFd < 0) {
        mVdin0DevFd = VDIN_OpenModule();
    }

    if ( mVdin0DevFd >= 0 ) {
        va_start ( ap, request );
        arg = va_arg ( ap, void * );
        va_end ( ap );

        tmp_ret = ioctl ( mVdin0DevFd, request, arg );
        LOGD ( "%s: ret = %d\n",__FUNCTION__,tmp_ret);
        return tmp_ret;
    }

    return -1;
}

int CTvin::VDIN_OpenPort ( tvin_port_t port )
{
    tvin_parm_s vdinParam;
    vdinParam.port = port;
    vdinParam.index = 0;
    int rt = VDIN_DeviceIOCtl ( TVIN_IOC_OPEN, &vdinParam );
    if ( rt < 0 ) {
        LOGE("Vdin open port, error(%s)!\n", strerror(errno));
    }

    return rt;
}

int CTvin::VDIN_ClosePort()
{
    int rt = VDIN_DeviceIOCtl ( TVIN_IOC_CLOSE );
    if ( rt < 0 ) {
        LOGE("Vdin close port, error(%s)!\n", strerror(errno));
    }

    return rt;
}

int CTvin::VDIN_StartDec(tvin_parm_s *vdinParam)
{
    int ret = -1;
    if ( vdinParam == NULL ) {
        return ret;
    }

    LOGD("VDIN_StartDec: index = [%d] port = [0x%x] format = [0x%x]\n",
        vdinParam->index, ( unsigned int ) vdinParam->port, ( unsigned int ) ( vdinParam->info.fmt ));
    ret = VDIN_DeviceIOCtl(TVIN_IOC_START_DEC, vdinParam);
    if ( ret < 0 ) {
        LOGE("Vdin start decode, error(%s)!\n", strerror ( errno ));
    }

    return ret;
}

int CTvin::VDIN_StopDec()
{
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_STOP_DEC );
    if (ret < 0) {
        LOGE("Vdin stop decode, error(%s).\n", strerror ( errno ));
    }
    return ret;
}

int CTvin::VDIN_GetSignalEventInfo(struct vdin_event_info_s *SignalEventInfo)
{
    int ret = VDIN_DeviceIOCtl(TVIN_IOC_G_EVENT_INFO, SignalEventInfo);
    if (ret < 0) {
        LOGE("%s error(%s), ret = %d.\n", __FUNCTION__, strerror(errno), ret);
    }

    return ret;
}

int CTvin::VDIN_GetSignalInfo ( struct tvin_info_s *SignalInfo )
{
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_G_SIG_INFO, SignalInfo );
    if ( ret < 0 ) {
        LOGE("%s failed, error(%s).\n", __FUNCTION__, strerror ( errno ));
    }
    return ret;
}

int CTvin::VDIN_SetVdinParam (tvin_parm_s *vdinParam)
{
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_S_PARM, vdinParam );
    if ( ret < 0 ) {
        LOGE ( "Vdin set signal param, error(%s).\n", strerror ( errno ) );
    }

    return ret;
}

int CTvin::VDIN_GetVdinParam(tvin_parm_s *vdinParam)
{
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_G_PARM, vdinParam );
    if ( ret < 0 ) {
        LOGE ( "Vdin get signal param, error(%s).\n", strerror ( errno ) );
    }

    return ret;
}

int CTvin::VDIN_SetColorRangeMode(tvin_color_range_t range_mode)
{
    LOGD("mode = %d\n", range_mode);
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_SET_COLOR_RANGE, &range_mode );
    if ( ret < 0 ) {
        LOGE ( "Vdin Set ColorRange Mode error(%s)!\n", strerror(errno ));
    }

    return ret;
}

int CTvin::VDIN_GetColorRangeMode(void)
{
    int range_mode = TVIN_COLOR_RANGE_AUTO;
    int ret = VDIN_DeviceIOCtl ( TVIN_IOC_GET_COLOR_RANGE, &range_mode );
    if ( ret < 0 ) {
        LOGE ( "Vdin Get ColorRange Mode error(%s)!\n", strerror(errno ));
    }
    LOGD("%s: mode = %d\n", __FUNCTION__, range_mode);

    return range_mode;
}

// AFE
int CTvin::AFE_OpenModule ( void )
{
    int fd = open ( AFE_DEV_PATH, O_RDWR );
    if ( fd < 0 ) {
        LOGE ( "Open tvafe module, error(%s).\n", strerror ( errno ) );
        return -1;
    }
    LOGD ( "%s: Open %s module fd = [%d]\n",__FUNCTION__, AFE_DEV_PATH, fd );
    return fd;
}

int CTvin::AFE_CloseModule ( void )
{
    if ( mAfeDevFd >= 0 ) {
        close ( mAfeDevFd );
        mAfeDevFd = -1;
    }

    return 0;
}

int CTvin::AFE_DeviceIOCtl ( int request, ... )
{
    int tmp_ret = -1;
    va_list ap;
    void *arg;

    if (mAfeDevFd < 0) {
        mAfeDevFd = AFE_OpenModule();
    }

    if ( mAfeDevFd >= 0 ) {
        va_start ( ap, request );
        arg = va_arg ( ap, void * );
        va_end ( ap );

        tmp_ret = ioctl ( mAfeDevFd, request, arg );
        LOGD ( "%s: ret = %d\n",__FUNCTION__,tmp_ret);
        AFE_CloseModule();

        return tmp_ret;
    }

    return -1;
}

int CTvin::Tvin_OpenPort(tvin_port_t source_port)
{
    LOGD ("%s, source_port = %x!\n", __FUNCTION__,  source_port);

    int ret = VDIN_OpenPort(source_port);

    return ret;
}

int CTvin::Tvin_ClosePort(tvin_port_t source_port)
{
    LOGD ("%s, source_port = %x!\n", __FUNCTION__,  source_port);

    int ret = VDIN_ClosePort();

    return ret;
}

tv_source_input_type_t CTvin::Tvin_SourceInputToSourceInputType ( tv_source_input_t source_input )
{
    tv_source_input_type_t ret = SOURCE_TYPE_MPEG;
    switch (source_input) {
        case SOURCE_TV:
            ret = SOURCE_TYPE_TV;
            break;
        case SOURCE_AV1:
        case SOURCE_AV2:
            ret = SOURCE_TYPE_AV;
            break;
        case SOURCE_YPBPR1:
        case SOURCE_YPBPR2:
            ret = SOURCE_TYPE_COMPONENT;
            break;
        case SOURCE_VGA:
            ret = SOURCE_TYPE_VGA;
            break;
        case SOURCE_HDMI1:
        case SOURCE_HDMI2:
        case SOURCE_HDMI3:
        case SOURCE_HDMI4:
            ret = SOURCE_TYPE_HDMI;
            break;
        case SOURCE_DTV:
            ret = SOURCE_TYPE_DTV;
            break;
        case SOURCE_IPTV:
            ret = SOURCE_TYPE_IPTV;
            break;
        case SOURCE_SPDIF:
            ret = SOURCE_TYPE_SPDIF;
            break;
        default:
            ret = SOURCE_TYPE_MPEG;
            break;
    }

    return ret;
}

tv_source_input_type_t CTvin::Tvin_SourcePortToSourceInputType ( tvin_port_t source_port )
{
    tv_source_input_t source_input = Tvin_PortToSourceInput(source_port);
    return Tvin_SourceInputToSourceInputType(source_input);
}

tvin_port_t CTvin::Tvin_GetSourcePortBySourceType ( tv_source_input_type_t source_type )
{
    tv_source_input_t source_input;

    switch (source_type) {
        case SOURCE_TYPE_TV:
            source_input = SOURCE_TV;
            break;
        case SOURCE_TYPE_AV:
            source_input = SOURCE_AV1;
            break;
        case SOURCE_TYPE_COMPONENT:
            source_input = SOURCE_YPBPR1;
            break;
        case SOURCE_TYPE_VGA:
            source_input = SOURCE_VGA;
            break;
        case SOURCE_TYPE_HDMI:
            source_input = SOURCE_HDMI1;
            break;
        case SOURCE_TYPE_IPTV:
            source_input = SOURCE_IPTV;
            break;
        case SOURCE_TYPE_DTV:
            source_input = SOURCE_DTV;
            break;
        case SOURCE_TYPE_SPDIF:
            source_input = SOURCE_SPDIF;
            break;
        default:
            source_input = SOURCE_MPEG;
            break;
    }

    return Tvin_GetSourcePortBySourceInput(source_input);
}

tvin_port_t CTvin::Tvin_GetSourcePortBySourceInput ( tv_source_input_t source_input )
{
    tvin_port_t source_port = TVIN_PORT_NULL;

    if ( source_input < SOURCE_TV || source_input >= SOURCE_MAX ) {
        source_port = TVIN_PORT_NULL;
    } else {
        source_port = ( tvin_port_t ) mSourceInputToPortMap[ ( int ) source_input];
    }

    return source_port;
}

tv_source_input_t CTvin::Tvin_PortToSourceInput ( tvin_port_t port )
{
    for ( int i = SOURCE_TV; i < SOURCE_MAX; i++ ) {
        if ( mSourceInputToPortMap[i] == (int)port ) {
            return (tv_source_input_t)i;
        }
    }

    return SOURCE_MAX;
}

tvin_port_id_t CTvin::Tvin_GetHdmiPortIdBySourceInput(tv_source_input_t source_input)
{
    tvin_port_id_t portId = TVIN_PORT_ID_MAX;
    if ((source_input > SOURCE_HDMI4) || (source_input < SOURCE_HDMI1)) {
        LOGD("%s: not HDMI Source.\n", __FUNCTION__);
    } else {
        tvin_port_t portValue = Tvin_GetSourcePortBySourceInput(source_input);
        switch (portValue) {
        case TVIN_PORT_HDMI0:
            portId = TVIN_PORT_ID_1;
            break;
        case TVIN_PORT_HDMI1:
            portId = TVIN_PORT_ID_2;
            break;
        case TVIN_PORT_HDMI2:
            portId = TVIN_PORT_ID_3;
            break;
        case TVIN_PORT_HDMI3:
            portId = TVIN_PORT_ID_4;
            break;
        default:
            portId = TVIN_PORT_ID_MAX;
            break;
        }
    }

    LOGD("%s: source: %d, portId: %d.\n", __FUNCTION__, source_input, portId);
    return portId;
}

int CTvin::Tvin_GetFrontendInfo(tvin_frontend_info_t *frontendInfo)
{
    int ret = -1;
    if (frontendInfo == NULL) {
        LOGE("%s: frontendInfo is NULL!\n", __FUNCTION__);
    } else {
        ret = VDIN_DeviceIOCtl(TVIN_IOC_G_FRONTEND_INFO, frontendInfo);
        if (ret < 0) {
            LOGE("%s error(%s)!\n", __FUNCTION__, strerror(errno));
        }
    }

    return ret;
}

int CTvin::Tvin_SetColorRangeMode(tvin_color_range_t range_mode)
{
    return VDIN_SetColorRangeMode(range_mode);
}

int CTvin::Tvin_GetColorRangeMode()
{
    return VDIN_GetColorRangeMode();
}

unsigned int CTvin::Tvin_TransPortStringToValue(const char *port_str)
{
    if (strcasecmp(port_str, "TVIN_PORT_CVBS0") == 0) {
        return TVIN_PORT_CVBS0;
    } else if (strcasecmp(port_str, "TVIN_PORT_CVBS1") == 0) {
        return TVIN_PORT_CVBS1;
    } else if (strcasecmp(port_str, "TVIN_PORT_CVBS2") == 0) {
        return TVIN_PORT_CVBS2;
    } else if (strcasecmp(port_str, "TVIN_PORT_CVBS3") == 0) {
        return TVIN_PORT_CVBS3;
    } else if (strcasecmp(port_str, "TVIN_PORT_COMP0") == 0) {
        return TVIN_PORT_COMP0;
    } else if (strcasecmp(port_str, "TVIN_PORT_COMP1") == 0) {
        return TVIN_PORT_COMP1;
    } else if (strcasecmp(port_str, "TVIN_PORT_VGA0") == 0) {
        return TVIN_PORT_VGA0;
    } else if (strcasecmp(port_str, "TVIN_PORT_HDMI0") == 0) {
        return TVIN_PORT_HDMI0;
    } else if (strcasecmp(port_str, "TVIN_PORT_HDMI1") == 0) {
        return TVIN_PORT_HDMI1;
    } else if (strcasecmp(port_str, "TVIN_PORT_HDMI2") == 0) {
        return TVIN_PORT_HDMI2;
    } else if (strcasecmp(port_str, "TVIN_PORT_HDMI3") == 0) {
        return TVIN_PORT_HDMI3;
    }

    return TVIN_PORT_NULL;
}

void CTvin::Tvin_LoadSourceInputToPortMap()
{
    const char *config_value = NULL;
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_ATV, "TVIN_PORT_CVBS3");
    mSourceInputToPortMap[SOURCE_TV] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_AV1, "TVIN_PORT_CVBS1");
    mSourceInputToPortMap[SOURCE_AV1] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_AV2, "TVIN_PORT_CVBS2");
    mSourceInputToPortMap[SOURCE_AV2] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_YPBPR1, "TVIN_PORT_COMP0");
    mSourceInputToPortMap[SOURCE_YPBPR1] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_YPBPR2, "TVIN_PORT_COMP1");
    mSourceInputToPortMap[SOURCE_YPBPR2] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_HDMI1, "TVIN_PORT_HDMI0");
    mSourceInputToPortMap[SOURCE_HDMI1] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_HDMI2, "TVIN_PORT_HDMI2");
    mSourceInputToPortMap[SOURCE_HDMI2] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_HDMI3, "TVIN_PORT_HDMI1");
    mSourceInputToPortMap[SOURCE_HDMI3] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_HDMI4, "TVIN_PORT_HDMI3");
    mSourceInputToPortMap[SOURCE_HDMI4] = Tvin_TransPortStringToValue(config_value);
    config_value = ConfigGetStr(CFG_SECTION_SRC_INPUT, CFG_TVCHANNEL_VGA, "TVIN_PORT_VGA0");
    mSourceInputToPortMap[SOURCE_VGA] = Tvin_TransPortStringToValue(config_value);
    mSourceInputToPortMap[SOURCE_MPEG] = TVIN_PORT_MPEG0;
    mSourceInputToPortMap[SOURCE_DTV] = TVIN_PORT_DTV;
    mSourceInputToPortMap[SOURCE_IPTV] = TVIN_PORT_BT656;
    mSourceInputToPortMap[SOURCE_SPDIF] = TVIN_PORT_CVBS3;
}

int CTvin::Tvin_StartDecoder(tvin_info_t info)
{
    if (mDecoderStarted) {
        LOGD("decoder already started.\n");
        return 0;
    } else {
        mTvinParam.info = info;

        if (VDIN_StartDec(&mTvinParam) >= 0 ) {
            LOGD("StartDecoder succeed.\n");
            mDecoderStarted = true;
            return 0;
        } else {
            LOGE("StartDecoder failed.\n");
            mDecoderStarted = false;
            return -1;
        }
    }
}

int CTvin::Tvin_StopDecoder()
{
    if (!mDecoderStarted) {
        LOGD("Decoder don't started!\n");
        return 0;
    } else {
        if ( VDIN_StopDec() >= 0 ) {
            LOGD("StopDecoder ok!\n");
            mDecoderStarted = false;
            return 0;
        } else {
            LOGE("StopDecoder failed!\n");
            mDecoderStarted = false;
            return -1;
        }
    }
}

int CTvin::Tvin_SwitchSnow(bool enable)
{
    int ret = -1;
    if ( enable ) {
        LOGD("%s: set snow enable\n", __FUNCTION__ );
        ret = AFE_DeviceIOCtl( TVIN_IOC_S_AFE_SONWON );
        ret = VDIN_DeviceIOCtl( TVIN_IOC_SNOWON );
    } else {
        LOGD("%s: set snow disable\n", __FUNCTION__ );
        ret = AFE_DeviceIOCtl( TVIN_IOC_S_AFE_SONWOFF );
        ret = VDIN_DeviceIOCtl( TVIN_IOC_SNOWOFF );
    }

    return ret;
}

int CTvin::Tvin_GetSignalEventInfo(vdin_event_info_s *SignalEventInfo)
{
    int ret = -1;
    if (SignalEventInfo == NULL) {
        LOGE("Tvin_GetSignalEventInfo: SignalEventInfo is NULL.\n");
    } else {
        ret = VDIN_GetSignalEventInfo(SignalEventInfo);
    }

    return ret;
}

int CTvin::Tvin_GetSignalInfo(tvin_info_s *SignalInfo)
{
    int ret = -1;
    if (SignalInfo == NULL) {
        LOGE("Tvin_GetSignalInfo: SignalInfo is NULL.\n");
    } else {
        ret = VDIN_GetSignalInfo(SignalInfo);
    }

    return ret;
}

int CTvin::Tvin_GetVdinDeviceFd(void)
{
    int ret = -1;
    if (mVdin0DevFd < 0) {
        ret = VDIN_OpenModule();
    } else {
        ret = mVdin0DevFd;
    }

    return ret;
}

int CTvin::VDIN_AddPath ( const char *videopath )
{
    if (strlen(videopath) > 1024) {
        LOGE("video path too long\n");
        return -1;
    }

    char str[1024 + 1] = {0};
    sprintf (str, "%s", videopath);
    return tvWriteSysfs(SYS_VFM_MAP_PATH, str);
}

int CTvin::VDIN_RemovePath(tv_path_type_t pathtype)
{
    int ret = -1;
    switch (pathtype) {
        case TV_PATH_TYPE_DEFAULT:
            ret = tvWriteSysfs(SYS_VFM_MAP_PATH, "rm default");
            break;
        case TV_PATH_TYPE_TVIN:
            ret = tvWriteSysfs(SYS_VFM_MAP_PATH, "rm tvpath");
            break;
        default:
            LOGE("invalie videopath type!\n");
            break;
    }

    return ret;
}

int CTvin::Tvin_AddVideoPath(int selPath)
{
    int ret = -1;
    std::string vdinPath;
    std::string suffixVideoPath("deinterlace amvideo");
    bool amlvideo2Exist = isFileExist(AMLVIDEO2_DEV_PATH);
    switch ( selPath ) {
    case TV_PATH_VDIN_AMLVIDEO2_PPMGR_DEINTERLACE_AMVIDEO:
        if (amlvideo2Exist)
            vdinPath = "add tvpath vdin0 amlvideo2.0 ";
        else
            vdinPath = "add tvpath vdin0 ";
        break;

    case TV_PATH_DECODER_AMLVIDEO2_PPMGR_DEINTERLACE_AMVIDEO:
        if (amlvideo2Exist)
            vdinPath = "add default decoder amlvideo2.0 ppmgr ";
        else
            vdinPath = "add default decoder ppmgr ";
        break;
    default:
        break;
    }

    vdinPath += suffixVideoPath;
    ret = VDIN_AddPath (vdinPath.c_str());
    return ret;
}

int CTvin::Tvin_RemoveVideoPath(tv_path_type_t pathtype)
{
    int ret = -1;
    int i = 0, dly = 10;
    for ( i = 0; i < 50; i++ ) {
        ret = VDIN_RemovePath(pathtype);
        if ( ret > 0 ) {
            LOGE("remove default path ok, %d ms gone.\n", (dly * i));
            break;
        } else {
            LOGE("remove default path faild, %d ms gone.\n", (dly * i));
            usleep(dly * 1000);
        }
    }
      return ret;
}

int CTvin::Tvin_CheckVideoPathComplete(tv_path_type_t path_type)
{
    int ret = -1;
    FILE *fp = NULL;
    char path[100] = {0};
    char decoder_str[20] = "default {";
    char tvin_str[20] = "tvpath {";
    char *str_find = NULL;
    char di_str[16] = "deinterlace";
    char ppmgr_str[16] = "ppmgr";
    char amvideo_str[16] = "amvideo";

    fp = fopen(SYS_VFM_MAP_PATH, "r");
    if (!fp) {
        LOGE("%s, can not open %s!\n", SYS_VFM_MAP_PATH);
        return ret;
    }

    while (fgets(path, sizeof(path)-1, fp)) {
        if (path_type == TV_PATH_TYPE_DEFAULT) {
            str_find = strstr(path, decoder_str);
        } else if (path_type == TV_PATH_TYPE_TVIN) {
            str_find = strstr(path, tvin_str);
        } else {
            break;
        }

        if (str_find != NULL) {
            if ((strstr(str_find, di_str)) &&
                 (strstr(str_find, ppmgr_str)) &&
                 (strstr(str_find, amvideo_str))) {
                LOGD("VideoPath is complete!\n");
                ret = 0;
            } else {
                LOGD("VideoPath is not complete!\n");
                ret = -1;
            }
            break;
        }
    }

    fclose(fp);
    fp = NULL;

    return ret;
}

