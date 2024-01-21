#ifndef BITEHACK2024_STEPPERS_H
#define BITEHACK2024_STEPPERS_H

#include <Stepper.h>

namespace SNS {

template< pin_size_t PIN1, pin_size_t PIN2, pin_size_t PIN3, pin_size_t PIN4 >
class Step {
public:
  explicit Step(int speedRPM) {
    stepper.setSpeed(speedRPM);
  }

  void step(int steps) {
    stepper.step(steps);
  }

private:
  Stepper stepper{ 200, PIN1, PIN2, PIN3, PIN4 };
};

} // namespace SNS

#endif // BITEHACK2024_STEPPERS_H
