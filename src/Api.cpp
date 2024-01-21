#include "Api.h"

namespace SNS {

Api::Api() {
  photoresNorth.startTask();
  photoresEast.startTask();
  photoresSouth.startTask();
  photoresWest.startTask();

  voltage.startTask();

  status = WiFi.beginAP(SSID, PASS);
  while (status != WL_AP_LISTENING) {
    Serial.println("[!] Creating access point failed");
    delay(1000);
  }

  server.begin();
}

void Api::loop() {
  WiFiClient client = server.available();
  if (!client) { return; }

  Serial.println("[*] New client connected");

  String currentLine;
  while (client.connected()) {
    delayMicroseconds(10);
    if (client.available()) {
      char c = client.read();
      Serial.print(c);

      if (c == '\n') {
        if (currentLine.length() == 0) {
          if (currentLine.indexOf("/forward") != 0) {
            endpointMoveForward(client);
          } else if (currentLine.indexOf("/backward") != 0) {
            endpointMoveBackward(client);
          } else if (currentLine.indexOf("/left") != 0) {
            endpointRotateLeft(client);
          } else if (currentLine.indexOf("/right") != 0) {
            endpointRotateRight(client);
          } else if (currentLine.indexOf("/stop") != 0) {
            endpointStop(client);
          } else if (currentLine.indexOf("/scan") != 0) {
            endpointScan(client);
          } else if (currentLine.indexOf("/distance") != 0) {
            endpointGetDistance(client);
          } else if (currentLine.indexOf("/photoresistors") != 0) {
            endpointGetPhotoresistors(client);
          } else if (currentLine.indexOf("/voltage") != 0) {
            endpointGetVoltage(client);
          } else {
            client.println("HTTP/1.1 404 Not Found");
            client.println();
          }
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }

    client.stop();
    Serial.println("[*] Client disconnected");
  }
}

void Api::endpointMoveForward(WiFiClient& client) {
  Serial.println("[*] Moving forward");

  motorLeft.startForward();
  motorRight.startForward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointMoveBackward(WiFiClient& client) {
  Serial.println("[*] Moving backward");

  motorLeft.startBackward();
  motorRight.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointRotateLeft(WiFiClient& client) {
  Serial.println("[*] Rotating left");

  motorRight.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointRotateRight(WiFiClient& client) {
  Serial.println("[*] Rotating right");

  motorLeft.startBackward();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointStop(WiFiClient& client) {
  Serial.println("[*] Stopping");

  motorLeft.stop();
  motorRight.stop();

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointScan(WiFiClient& client) {
  Serial.println("[*] Scanning");

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointGetDistance(WiFiClient& client) {
  Serial.println("[*] Getting distance");

  client.println("HTTP/1.1 200 OK");
  client.println();
}

void Api::endpointGetPhotoresistors(WiFiClient& client) {
  Serial.println("[*] Getting photoresistors");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject()
    .property("north", photoresNorth.getLastValue())
    .property("east", photoresEast.getLastValue())
    .property("south", photoresSouth.getLastValue())
    .property("west", photoresWest.getLastValue())
    .endObject();
}

void Api::endpointGetVoltage(WiFiClient& client) {
  Serial.println("[*] Getting voltage");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();

  JsonWriter writer{ &client };
  writer.beginObject().property("voltage", voltage.getLastValue()).endObject();
}

} // namespace SNS