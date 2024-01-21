#ifndef BITEHACK2024_SERVOMECHANISM_H
#define BITEHACK2024_SERVOMECHANISM_H

#include <Servo.h>

namespace SNS {

template< pin_size_t pinServo >
class ServoMechanism {
public:
  ServoMechanism() {
    servo.attach(pinServo);
  }

  inline void setAngle(uint8_t angle) {
    servo.write(angle);
  }

private:
  Servo servo{};
};

} // namespace SNS

#endif // BITEHACK2024_SERVOMECHANISM_H
