#include "TUDTask.h"
#include "tusb.h"

void TUDTask::execute(absolute_time_t timestamp) { tud_task(); }
