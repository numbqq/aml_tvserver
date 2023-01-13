#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvTest-c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <TvClientWrapper.h>
#include <CTvClientLog.h>
#include <signal.h>

static int run = 1;

tv_source_input_t CurrentSource;
struct TvClientWrapper_t * pTvClientWrapper;

static void Signalhandler(int sig){ // handler ctrl + C operation
    StopTv(pTvClientWrapper, CurrentSource);
    ReleaseInstance(&pTvClientWrapper);
    exit(0);
}

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
    //WriteSysfs("/sys/class/graphics/fb0/osd_display_debug", "1");
    WriteSysfs("/sys/class/graphics/fb0/blank", "1");
//    WriteSysfs("/sys/kernel/debug/dri/0/vpu/blank", "1");
    return 0;

}

static void TvEventCallback(event_type_t eventType, void *eventData)
{
    if (eventType == TV_EVENT_TYPE_SIGLE_DETECT) {
        SignalDetectCallback_t *signalDetectEvent = (SignalDetectCallback_t *)(eventData);
        printf("%s: source: %d, signalFmt: %d, transFmt: %d, status: %d, isDVI: %d, Hdrinfo: %ud.\n", __FUNCTION__,
                                                   signalDetectEvent->SourceInput,
                                                   signalDetectEvent->SignalFmt,
                                                   signalDetectEvent->TransFmt,
                                                   signalDetectEvent->SignalStatus,
                                                   signalDetectEvent->isDviSignal,
                                                   signalDetectEvent->Hdrinfo);
    } else if (eventType == TV_EVENT_TYPE_SOURCE_CONNECT) {
        SourceConnectCallback_t *sourceConnectEvent = (SourceConnectCallback_t *)(eventData);
        printf("%s: source: %d, connectStatus: %d\n", __FUNCTION__,
                  sourceConnectEvent->SourceInput, sourceConnectEvent->ConnectionState);
    } else {
        printf("%s: invalid event.\n", __FUNCTION__);
    }
}

static void signal_handler(int s)
{
    void *retval;

    StopTv(pTvClientWrapper, CurrentSource);
	WriteSysfs("/sys/class/graphics/fb0/blank", "0");
//	WriteSysfs("/sys/kernel/debug/dri/0/vpu/blank", "0");
    ReleaseInstance(&pTvClientWrapper);
    run = 0;
    signal(s, SIG_DFL);
    raise(s);
    exit(0);
}

int main(int argc, char **argv) {
    pTvClientWrapper = GetInstance();
    setTvEventCallback(TvEventCallback);
    signal(SIGINT, signal_handler);
    tv_source_input_t CurrentSource = SOURCE_HDMI2;
    StopTv(pTvClientWrapper, CurrentSource);
    sleep(1);
    DisplayInit();

   StartTv(pTvClientWrapper, CurrentSource);

   while (run)
   {
       sleep(1);
   }

    return 0;
}
