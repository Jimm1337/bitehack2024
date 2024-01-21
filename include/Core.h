#ifndef BITEHACK2024_CORE_H
#define BITEHACK2024_CORE_H

#define configASSERT(x)                                                        \
  if ((x) == 0) vAssertCalled(__FILE__, __LINE__)

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

namespace SNS {

namespace PINS {

namespace PHOTORESISTORS {
inline constexpr pin_size_t NORTH = A0;
inline constexpr pin_size_t EAST  = A1;
inline constexpr pin_size_t SOUTH = A2;
inline constexpr pin_size_t WEST  = A3;
} // namespace PHOTORESISTORS

namespace DCMOTORS {
inline constexpr pin_size_t LEFT_IN1  = 2;
inline constexpr pin_size_t LEFT_IN2  = 3;
inline constexpr pin_size_t RIGHT_IN3 = 4;
inline constexpr pin_size_t RIGHT_IN4 = 5;
} // namespace DCMOTORS

namespace SERVOS {
inline constexpr pin_size_t COLUMN = 6;
} // namespace SERVOS

namespace STEPPERS {
inline constexpr pin_size_t COLUMN_1 = 7;
inline constexpr pin_size_t COLUMN_2 = 8;
inline constexpr pin_size_t COLUMN_3 = 9;
inline constexpr pin_size_t COLUMN_4 = 10;
} // namespace STEPPERS

namespace PANEL {
inline constexpr pin_size_t PWR  = A4;
} // namespace PANEL

namespace PROXIMITY {
inline constexpr pin_size_t ECHO = 11;
inline constexpr pin_size_t TRIG = 12;
} // namespace PROXIMITY

} // namespace PINS

} // namespace SNS

#endif // BITEHACK2024_CORE_H
