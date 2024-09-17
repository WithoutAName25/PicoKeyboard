#ifndef PICOKEYBOARD_GLOBALMANAGERS_H
#define PICOKEYBOARD_GLOBALMANAGERS_H

#include "keys/KeyStateManager.h"
#include "util/Scheduler.h"

void initializeGlobalManagers(uint8_t numKeys);

Scheduler &getScheduler();

KeyStateManager &getKeyStateManager();

#endif //PICOKEYBOARD_GLOBALMANAGERS_H
