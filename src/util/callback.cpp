#include <pspkernel.h>

static bool s_isRunning = true;

static int exit_callback(int arg1, int arg2, void* common)
{
    s_isRunning = false;
    return 0;
}

static int callbackThread(SceSize args, void* argp)
{
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

    return 0;
}

namespace util
{

int setupCallbacks()
{
    int thid = 0;

    thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xfa0, 0, 0);

    if (thid >= 0)
    {
        sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}

} // namespace util