#ifndef BITEHACK2024_API_H
#define BITEHACK2024_API_H

#include <JsonWriter.h>
#include <WiFiS3.h>
#include <gsl/gsl>
#include <string_view>
#include "AnalogReader.h"
#include "Core.h"
#include "Motor.h"
#include "Step.h"

namespace SNS {

class Api {
public:
  Api();

  void loop();

private:
  void endpointMoveForward(WiFiClient& client);
  void endpointMoveBackward(WiFiClient& client);
  void endpointRotateLeft(WiFiClient& client);
  void endpointRotateRight(WiFiClient& client);
  void endpointStop(WiFiClient& client);
  void endpointScan(WiFiClient& client);
  void endpointGetDistance(WiFiClient& client);
  void endpointGetPhotoresistors(WiFiClient& client);
  void endpointGetVoltage(WiFiClient& client);

  inline static constexpr gsl::czstring SSID = "bitehack2024";
  inline static constexpr gsl::czstring PASS = "bitehack2024";
  inline static constexpr auto          PORT = 80;

  WiFiServer server{ PORT };
  int        status{ WL_IDLE_STATUS };

  AnalogReader< PINS::PHOTORESISTORS::NORTH > photoresNorth{ 100 };
  AnalogReader< PINS::PHOTORESISTORS::EAST >  photoresEast{ 100 };
  AnalogReader< PINS::PHOTORESISTORS::SOUTH > photoresSouth{ 100 };
  AnalogReader< PINS::PHOTORESISTORS::WEST >  photoresWest{ 100 };

  AnalogReader< PINS::PANEL::PWR > voltage{ 100 };

  Motor< PINS::DCMOTORS::LEFT_IN1, PINS::DCMOTORS::LEFT_IN2 >   motorLeft{};
  Motor< PINS::DCMOTORS::RIGHT_IN3, PINS::DCMOTORS::RIGHT_IN4 > motorRight{};

  Step<
    PINS::STEPPERS::COLUMN_1,
    PINS::STEPPERS::COLUMN_2,
    PINS::STEPPERS::COLUMN_3,
    PINS::STEPPERS::COLUMN_4 >
    stepColumn{180};
};

} // namespace SNS

#endif // BITEHACK2024_API_H
