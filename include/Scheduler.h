#ifndef BITEHACK2024_SCHEDULER_H
#define BITEHACK2024_SCHEDULER_H

#include <Arduino_FreeRTOS.h>
#include <gsl/gsl>
#include "Core.h"

namespace SNS::Scheduler {

using TaskFunction = void (*)(void*);

inline void init() {
  vTaskStartScheduler();
}

} // namespace SNS::Scheduler

#endif // BITEHACK2024_SCHEDULER_H
