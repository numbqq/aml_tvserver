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

static int run = 1;

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
//    WriteSysfs("/sys/class/graphics/fb0/osd_display_debug", "1");
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

static void signal_handler(int s)
{
    void *retval;
    run = 0;

	WriteSysfs("/sys/class/graphics/fb0/blank", "0");
    signal(s, SIG_DFL);
    raise(s);
}

int main(int argc, char **argv) {
    struct TvClientWrapper_t * pTvClientWrapper = GetInstance();
    setTvEventCallback(TvEventCallback);
    tv_source_input_t CurrentSource = SOURCE_HDMI2;
	StopTv(pTvClientWrapper, CurrentSource);

	sleep(1);

    DisplayInit();

    StartTv(pTvClientWrapper, CurrentSource);

	signal(SIGINT, signal_handler);

	while (run)
	{
		sleep(1);
	}

    return 0;
}
