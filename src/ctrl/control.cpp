#include <control.hpp>
#include <pspctrl.h>

namespace ctrl
{
bool init()
{
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    return true;
}

void destroy() { }

void poll(Pad* pad)
{
    sceCtrlReadBufferPositive(pad, 1);
}

} // namespace ctrl