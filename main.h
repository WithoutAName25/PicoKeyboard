#ifndef PICOKEYBOARD_MAIN_H
#define PICOKEYBOARD_MAIN_H

#include "pico/stdlib.h"
#include "util/Scheduler.h"
#include "keys/KeyStateController.h"

extern Scheduler scheduler;
extern KeyStateController keyStateController;

#endif //PICOKEYBOARD_MAIN_H
