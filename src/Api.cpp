#include "Api.h"

namespace SNS {

Api::Api() {
  photoresNorth.startTask();
  photoresEast.startTask();
  photoresSouth.startTask();
  photoresWest.startTask();

  voltage.startTask();

  startTasksBalance();

  Serial.println("[*] Starting access point");

  status = WiFi.beginAP(SSID, PASS);
  while (status != WL_AP_LISTENING) {
    Serial.println("[!] Creating access point failed");
    delay(1000);
  }

  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  xTaskCreate(controllerTask, "Controller", 500, this, 1, nullptr);
  vTaskStartScheduler();
}

void Api::controllerTask(void* thisPtr) {
  auto* self = static_cast< Api* >(thisPtr);

  while (true) {
    WiFiClient client = self->server.available();
    if (!client) { continue; }

    Serial.println("[*] New client connected");
    String currentLine = "";
    EndpointFn endpoint = endpointNotFound;

    while (client.connected()) {
      delayMicroseconds(10);
      if (client.available()) {
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
            } else if (currentLine.indexOf("GET /scan") >= 0) {
              endpoint = endpointScan;
            } else if (currentLine.indexOf("GET /distance") >= 0) {
              endpoint = endpointGetDistance;
            } else if (currentLine.indexOf("GET /photoresistors") >= 0) {
              endpoint = endpointGetPhotoresistors;
            } else if (currentLine.indexOf("GET /voltage") >= 0) {
              endpoint = endpointGetVoltage;
            }

            currentLine = "";
          }
        } else if (c != '\r') { // if you got anything else but a carriage// return character,
          currentLine += c;     // add it to the end of the currentLine
        }
      }
    }

    client.stop();
    Serial.println("[*] Client disconnected");

    vTaskDelay(1);
  }
}

void Api::startTasksBalance() {
  xTaskCreate(taskBalanceRotation, "BalanceRotation", 0, this, 1, nullptr);
  xTaskCreate(taskBalanceAngle, "BalanceAngle", 0, this, 1, nullptr);
}

void Api::taskBalanceRotation(void* param) {
  auto* self = static_cast< Api* >(param);

  while (true) {
    // TODO: balance

    vTaskDelay(1);
  }
}

void Api::taskBalanceAngle(void* param) {
  auto* self = static_cast< Api* >(param);

  while (true) {
    // TODO: balance

    vTaskDelay(1);
  }
}

void Api::endpointMoveForward(Api* self, WiFiClient& client) {
  Serial.println("[*] Moving forward");

  self->motorLeft.startForward();
  self->motorRight.startForward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointMoveBackward(Api* self, WiFiClient& client) {
  Serial.println("[*] Moving backward");

  self->motorLeft.startBackward();
  self->motorRight.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointRotateLeft(Api* self, WiFiClient& client) {
  Serial.println("[*] Rotating left");

  self->motorRight.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointRotateRight(Api* self, WiFiClient& client) {
  Serial.println("[*] Rotating right");

  self->motorLeft.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointStop(Api* self, WiFiClient& client) {
  Serial.println("[*] Stopping");

  self->motorLeft.stop();
  self->motorRight.stop();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointScan(Api*  /*self*/, WiFiClient& client) {
  Serial.println("[*] Scanning");

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointGetDistance(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting distance");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject().property("distance", self->distance.getDistance()).endObject();

  client.println();
}

void Api::endpointGetPhotoresistors(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting photoresistors");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
    writer.beginObject()
      .property("north", self->photoresNorth.getLastValue())
      .property("east", self->photoresEast.getLastValue())
      .property("south", self->photoresSouth.getLastValue())
      .property("west", self->photoresWest.getLastValue())
      .endObject();

  client.println();
}

void Api::endpointGetVoltage(Api* self, WiFiClient& client) {
  Serial.println("[*] Getting voltage");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject().property("voltage", self->voltage.getLastValue()).endObject();

  client.println();
}

void Api::endpointNotFound(Api*  /*self*/, WiFiClient& client) {
  Serial.println("[!] Endpoint not found");

  client.println("HTTP/1.1 404 Not Found");
  client.println();
}

} // namespace SNS