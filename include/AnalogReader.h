#ifndef BITEHACK2024_ANALOGREADER_H
#define BITEHACK2024_ANALOGREADER_H

#include <cstring>
#include "Core.h"

namespace SNS {

template< pin_size_t PIN >
class AnalogReader {
public:
  explicit AnalogReader(TickType_t delay): readDelay(delay) {
    pinMode(PIN, INPUT);
  }

  inline void startTask() {
    static taskParams params{ readDelay, lastValue };
    xTaskCreate(
      task, (String("ANALOG_PIN") + PIN).c_str(), 50, &params, 3, nullptr);
  }

  [[nodiscard]] inline int getLastValue() const {
    return lastValue;
  }

private:
  static void task(void* params) {
    auto* args = static_cast< taskParams* >(params);
    while (true) {
      args->second = analogRead(PIN);
      vTaskDelay(args->first);
    }
  }

  TickType_t readDelay;
  int        lastValue{ -1 };

  using taskParams = std::pair< decltype(readDelay), decltype(lastValue)& >;
};

} // namespace SNS

#endif // BITEHACK2024_ANALOGREADER_H
