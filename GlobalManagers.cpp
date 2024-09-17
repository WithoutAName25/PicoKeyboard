#include "GlobalManagers.h"

static std::unique_ptr<Scheduler> scheduler;
static std::unique_ptr<KeyStateManager> keyStateManager;

void initializeGlobalManagers(uint8_t numKeys) {
    scheduler = std::make_unique<Scheduler>();
    keyStateManager = std::make_unique<KeyStateManager>(numKeys);
}

Scheduler &getScheduler() {
    return *scheduler;
}

KeyStateManager &getKeyStateManager() {
    return *keyStateManager;
}
