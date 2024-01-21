#ifndef BITEHACK2024_MOTOR_H
#define BITEHACK2024_MOTOR_H

#include "Core.h"

namespace SNS {

template< pin_size_t IN1, pin_size_t IN2 >
class Motor {
public:
  Motor() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
  }

  void startForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }

  void startBackward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  void stop() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
};

} // namespace SNS

#endif // BITEHACK2024_MOTOR_H
