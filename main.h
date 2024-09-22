#ifndef PICOKEYBOARD_MAIN_H
#define PICOKEYBOARD_MAIN_H

#include "pico/stdlib.h"
#include "util/Scheduler.h"
#include "keys/KeyStateController.h"
#include "rgb/RgbController.h"
#include "keys/KeyActionController.h"

extern RgbController rgb;
extern Scheduler scheduler;
extern KeyStateController keyStateController;
extern KeyActionController keyActionController;

#endif //PICOKEYBOARD_MAIN_H
