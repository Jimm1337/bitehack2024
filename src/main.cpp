#include "Api.h"
#include "Scheduler.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  static SNS::Api api{};
}