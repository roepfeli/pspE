#pragma once

#include <pspctrl.h>

namespace ctrl
{

typedef SceCtrlData Pad;

bool init();

void destroy();

void poll(Pad* pad);
} // namespace ctrl