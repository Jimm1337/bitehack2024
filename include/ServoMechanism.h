#ifndef BITEHACK2024_SERVOMECHANISM_H
#define BITEHACK2024_SERVOMECHANISM_H

#include <Servo.h>

namespace SNS {

template< pin_size_t pinServo >
class ServoMechanism {
public:
  ServoMechanism() {
    servo.attach(pinServo);
    setAngle(0);
  }

  inline void setAngle(int angle) {
    servo.write(angle);
    this->angle = angle;
  }

  inline int getAngle() const {
    return angle;
  }

private:
  Servo servo{};
  int   angle{ 0 };
};

} // namespace SNS

#endif // BITEHACK2024_SERVOMECHANISM_H
