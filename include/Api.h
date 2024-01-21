#ifndef BITEHACK2024_API_H
#define BITEHACK2024_API_H

#include "Core.h"
#include <JsonWriter.h>
#include <WiFiS3.h>
#include <gsl/gsl>
#include <string_view>
#include "AnalogReader.h"
#include "Distance.h"
#include "Motor.h"
#include "ServoMechanism.h"
#include "Step.h"
#include <Arduino_FreeRTOS.h>
#include <gsl/gsl>

namespace SNS {

class Api {
public:
  Api();

  static void controllerTask(void* thisPtr);

private:
  void startTasksBalance();

  static void taskBalanceRotation(void* param);
  static void taskBalanceAngle(void* param);

  static void endpointMoveForward(Api* self, WiFiClient& client);
  static void endpointMoveBackward(Api* self, WiFiClient& client);
  static void endpointRotateLeft(Api* self, WiFiClient& client);
  static void endpointRotateRight(Api* self, WiFiClient& client);
  static void endpointStop(Api* self, WiFiClient& client);
  static void endpointScan(Api* self, WiFiClient& client);
  static void endpointGetDistance(Api* self, WiFiClient& client);
  static void endpointGetPhotoresistors(Api* self, WiFiClient& client);
  static void endpointGetVoltage(Api* self, WiFiClient& client);
  static void endpointNotFound(Api* self, WiFiClient& client);

  inline static constexpr gsl::czstring SSID = "BITEhack Robotyczna";
  inline static constexpr gsl::czstring PASS = "1234567891011";
  inline static constexpr auto          PORT = 80;

  WiFiServer server{ PORT };
  int        status{ WL_IDLE_STATUS };

  AnalogReader< PINS::PHOTORESISTORS::CCW >  photoresCcw{ 101 };
  AnalogReader< PINS::PHOTORESISTORS::UP >   photoresUp{ 102 };
  AnalogReader< PINS::PHOTORESISTORS::CW >   photoresCw{ 103 };
  AnalogReader< PINS::PHOTORESISTORS::DOWN > photoresDown{ 104 };

  AnalogReader< PINS::PANEL::PWR > voltage{ 100 };

  Distance< PINS::PROXIMITY::TRIG, PINS::PROXIMITY::ECHO > distance{};

  Motor< PINS::DCMOTORS::LEFT_IN1, PINS::DCMOTORS::LEFT_IN2 >   motorLeft{};
  Motor< PINS::DCMOTORS::RIGHT_IN3, PINS::DCMOTORS::RIGHT_IN4 > motorRight{};

  ServoMechanism< PINS::SERVOS::COLUMN > servo{};

  Step<
    PINS::STEPPERS::COLUMN_1,
    PINS::STEPPERS::COLUMN_2,
    PINS::STEPPERS::COLUMN_3,
    PINS::STEPPERS::COLUMN_4 >
    stepColumn{ 500 };

  using EndpointFn = void (*)(Api*, WiFiClient&);
};

} // namespace SNS

#endif // BITEHACK2024_API_H
