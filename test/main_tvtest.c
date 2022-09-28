#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvTest-c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <TvClientWrapper.h>
#include <CTvClientLog.h>

static int WriteSysfs(const char *path, const char *cmd)
{
    int fd;
    fd = open(path, O_CREAT|O_RDWR | O_TRUNC, 0777);

    if (fd >= 0) {
        write(fd, cmd, strlen(cmd));
        close(fd);
        return 0;
    }

    return -1;
}

static int DisplayInit()
{
    WriteSysfs("/sys/class/graphics/fb0/osd_display_debug", "1");
    WriteSysfs("/sys/class/graphics/fb0/blank", "1");
    return 0;

}

static void TvEventCallback(event_type_t eventType, void *eventData)
{
    if (eventType == TV_EVENT_TYPE_SIGLE_DETECT) {
        SignalDetectCallback_t *signalDetectEvent = (SignalDetectCallback_t *)(eventData);
        LOGD("%s: source: %d, signalFmt: %d, transFmt: %d, status: %d, isDVI: %d.\n", __FUNCTION__,
                                                   signalDetectEvent->SourceInput,
                                                   signalDetectEvent->SignalFmt,
                                                   signalDetectEvent->TransFmt,
                                                   signalDetectEvent->SignalStatus,
                                                   signalDetectEvent->isDviSignal);
    } else if (eventType == TV_EVENT_TYPE_SOURCE_CONNECT) {
        SourceConnectCallback_t *sourceConnectEvent = (SourceConnectCallback_t *)(eventData);
        LOGD("%s: source: %d, connectStatus: %d\n", __FUNCTION__,
                  sourceConnectEvent->SourceInput, sourceConnectEvent->ConnectionState);
    } else {
        LOGD("%s: invalid event.\n", __FUNCTION__);
    }
}

int main(int argc, char **argv) {
    struct TvClientWrapper_t * pTvClientWrapper = GetInstance();
    setTvEventCallback(TvEventCallback);
    char Command[1];
    int run = 1;
    tv_source_input_t CurrentSource = SOURCE_HDMI1;
    DisplayInit();

    printf("#### please select cmd####\n");
    printf("#### select 1 to hdmi 1 ####\n");
    printf("#### select 2 to hdmi 2 ####\n");
    printf("#### select 3 to hdmi 3 ####\n");
    printf("#### select 4 to edid 1.4 (this is default) ####\n");
    printf("#### select 5 to edid 2.0 ####\n");
    printf("#### select 6 to edid auto ####\n");
    printf("#### select 7 to AV1 ####\n");
    printf("#### select 8 to AV2 ####\n");
    printf("#### select q to stop####\n");
    printf("##########################\n");
    while (run) {
        scanf("%s", Command);
        switch (Command[0]) {
          case 'q': {
            StopTv(pTvClientWrapper, CurrentSource);
            WriteSysfs("/sys/class/graphics/fb0/blank", "0");
            ReleaseInstance(&pTvClientWrapper);
            run = 0;
            break;
          }
          case '1': {
              StopTv(pTvClientWrapper, CurrentSource);
              CurrentSource = SOURCE_HDMI1;
              WriteSysfs("/sys/class/graphics/fb0/blank", "0");
              StartTv(pTvClientWrapper, CurrentSource);
              break;
          }
         case '2': {
              StopTv(pTvClientWrapper, CurrentSource);
              CurrentSource = SOURCE_HDMI2;
              WriteSysfs("/sys/class/graphics/fb0/blank", "0");
              StartTv(pTvClientWrapper, CurrentSource);
              break;
          }
          case '3': {
              StopTv(pTvClientWrapper, CurrentSource);
              CurrentSource = SOURCE_HDMI3;
              WriteSysfs("/sys/class/graphics/fb0/blank", "0");
              StartTv(pTvClientWrapper, CurrentSource);
              break;
          }
          case '4': {
              SetEdidVersion(pTvClientWrapper, CurrentSource, 0);
              break;
          }
          case '5': {
              SetEdidVersion(pTvClientWrapper, CurrentSource, 1);
              break;
          }
          case '6': {
              SetEdidVersion(pTvClientWrapper, CurrentSource, 2);
              break;
          }
          case '7': {
              StopTv(pTvClientWrapper, CurrentSource);
              CurrentSource = SOURCE_AV1;
              WriteSysfs("/sys/class/graphics/fb0/blank", "0");
              StartTv(pTvClientWrapper, CurrentSource);
              break;
          }
         case '8': {
              StopTv(pTvClientWrapper, CurrentSource);
              CurrentSource = SOURCE_AV2;
              WriteSysfs("/sys/class/graphics/fb0/blank", "0");
              StartTv(pTvClientWrapper, CurrentSource);
              break;
          }
          default: {
              LOGD("invalid cmd.\n");
              break;
          }
        }
        fflush (stdout);
    }

    return 0;
}
