#include "Api.h"

namespace SNS {

Api::Api() {
  Serial.println("[*] Starting access point");

  status = WiFi.begin(SSID, PASS);

  server.begin();

  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  photoresCcw.startTask();  // Stack 80B
  photoresUp.startTask();   // Stack 80B
  photoresCw.startTask();   // Stack 80B
  photoresDown.startTask(); // Stack 80B
  voltage.startTask();      // Stack 80B

  startTasksBalance(); // Stack 400B

  xTaskCreate(
    controllerTask, "Controller", 500, this, 2, nullptr); // Stack 800B

  vTaskStartScheduler();
}

void Api::controllerTask(void* thisPtr) {
  auto* self = static_cast< Api* >(thisPtr);

  while (true) {
    taskBalanceAngle(self);
    //    taskBalanceRotation(self);

    WiFiClient client = self->server.available();
    if (!client) { continue; }

    Serial.println("[*] New client connected");
    String     currentLine = "";
    EndpointFn endpoint    = endpointNotFound;

    while (client.connected()) {
      delayMicroseconds(10);
      if (!client.available()) { break; }

      char c = client.read();
      if (c == '\n') { // if the byte is a newline character

        // if the current line is blank, you got two newline characters in a
        // row. that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          endpoint(self, client);

          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while controllerTask:
          break;
        } else { // if you got a newline, then clear currentLine:
          if (currentLine.indexOf("GET /forward") >= 0) {
            endpoint = endpointMoveForward;
          } else if (currentLine.indexOf("GET /backward") >= 0) {
            endpoint = endpointMoveBackward;
          } else if (currentLine.indexOf("GET /left") >= 0) {
            endpoint = endpointRotateLeft;
          } else if (currentLine.indexOf("GET /right") >= 0) {
            endpoint = endpointRotateRight;
          } else if (currentLine.indexOf("GET /stop") >= 0) {
            endpoint = endpointStop;
          } else if (currentLine.indexOf("GET /distance") >= 0) {
            endpoint = endpointGetDistance;
          } else if (currentLine.indexOf("GET /photoresistors") >= 0) {
            endpoint = endpointGetPhotoresistors;
          } else if (currentLine.indexOf("GET /voltage") >= 0) {
            endpoint = endpointGetVoltage;
          }

          currentLine = "";
        }
      } else if (c != '\r') { // if you got anything else but a carriage//
                              // return character,
        currentLine += c;     // add it to the end of the currentLine
      }
    }

    client.stop();
    Serial.println("[*] Client disconnected");

    vTaskDelay(3);
  }
}

void Api::startTasksBalance() {
  //  xTaskCreate(taskBalanceRotation, "BalanceRotation", 100, this, 3,
  //  nullptr); xTaskCreate(taskBalanceAngle, "BalanceAngle", 100, this, 3,
  //  nullptr);
}

void Api::taskBalanceRotation(void* param) {
  auto* self = static_cast< Api* >(param);

  const auto lightEast = self->photoresCcw.getLastValue();
  const auto lightWest = self->photoresCw.getLastValue();
  const auto diff      = lightEast - lightWest;

  //  Serial.print(String("Diff: ") + diff);
  //  Serial.println(String(" Angle: ") + self->stepColumn.getPosition());

  if (abs(diff) > 10) {
    if (diff > 0 && self->stepColumn.getPosition() < 9999) {
      self->stepColumn.step(5);
    } else if (self->stepColumn.getPosition() > -9999) {
      self->stepColumn.step(-5);
    }
  }
}

void Api::taskBalanceAngle(void* param) {
  auto*      self      = static_cast< Api* >(param);
  const auto lightUp   = self->photoresUp.getLastValue();
  const auto lightDown = self->photoresDown.getLastValue();
  const auto diff      = lightUp - lightDown;

  if (abs(diff) > 25) {
    if (diff < 0 && self->servo.getAngle() <= 70) {
      self->servo.setAngle(self->servo.getAngle() + 1);
    } else if (self->servo.getAngle() > 0) {
      self->servo.setAngle(self->servo.getAngle() - 1);
    }
  }
}

void Api::endpointMoveForward(Api* self, WiFiClient& client) {
  Serial.println("[*] Moving forward");

  vTaskSuspendAll();
  self->motorLeft.startForward();
  self->motorRight.startForward();
  xTaskResumeAll();

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void Api::endpointMoveBackward(Api* self, WiFiClient& client) {
  Serial.println("[*] Moving backward");

  vTaskSuspendAll();
  self->motorLeft.startBackward();
  self->motorRight.startBackward();
  xTaskResumeAll();

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void Api::endpointRotateLeft(Api* self, WiFiClient& client) {
  Serial.println("[*] Rotating left");

  vTaskSuspendAll();
  self->motorLeft.startBackward();
  self->motorRight.startForward();
  xTaskResumeAll();

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void Api::endpointRotateRight(Api* self, WiFiClient& client) {
  Serial.println("[*] Rotating right");

  vTaskSuspendAll();
  self->motorLeft.startForward();
  self->motorRight.startBackward();
  xTaskResumeAll();

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void Api::endpointStop(Api* self, WiFiClient& client) {
  Serial.println("[*] Stopping");

  vTaskSuspendAll();
  self->motorLeft.stop();
  self->motorRight.stop();
  xTaskResumeAll();

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void Api::endpointGetDistance(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting distance");

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject()
    .property("distance", self->distance.getDistance())
    .endObject();

  client.println();
}

void Api::endpointGetPhotoresistors(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting photoresistors");

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject()
    .property("north", self->photoresCcw.getLastValue())
    .property("east", self->photoresUp.getLastValue())
    .property("south", self->photoresCw.getLastValue())
    .property("west", self->photoresDown.getLastValue())
    .endObject();

  client.println();
}

void Api::endpointGetVoltage(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting voltage");

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject()
    .property("voltage", self->voltage.getLastValue())
    .endObject();

  client.println();
}

void Api::endpointNotFound(Api* /*self*/, WiFiClient& client) {
  Serial.println("[!] Endpoint not found");

  client.println("HTTP/1.1 404 Not Found");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

} // namespace SNS