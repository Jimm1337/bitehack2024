#include "Api.h"
#include "Scheduler.h"

// piny rezystoryfoto
//  A0 - g, A1 - d, A2 - p, A3 - l

void setup() {
  Serial.begin(9600);

  SNS::Scheduler::init();
}

void loop() {
  static SNS::Api controller{};
  controller.loop();
}