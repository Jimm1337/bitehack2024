#ifndef BITEHACK2024_DISTANCE_H
#define BITEHACK2024_DISTANCE_H

namespace SNS {

template < pin_size_t pinTrigger, pin_size_t pinEcho >
class Distance {
public:
  Distance() {
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);
  }

  // CM
  float getDistance() {
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);

    float duration = pulseIn(pinEcho, HIGH);
    return (duration / 2) * 0.0343F;
  }
};

}

#endif // BITEHACK2024_DISTANCE_H
