#include "WifiClient.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>

void WifiClient::configure(char* ssid, char* password) {
  this->ssid = ssid;
  this->password = password;
}

void WifiClient::connect() {
  WiFi.begin(this->ssid, this->password);
}

void WifiClient::disconnect() {
  WiFi.disconnect();
}

int WifiClient::status() {
  return WiFi.status();
}

String WifiClient::statusText() {
  String status;

  switch (WiFi.status()) {
    case WL_CONNECTED:
      status = "connected";
      break;
    case WL_NO_SHIELD:
      status = "no-Wifi-shield";
      break;
    case WL_IDLE_STATUS:
      status = "idling";
      break;
    case WL_CONNECT_FAILED:
      status = "connect-failed";
      break;
    case WL_NO_SSID_AVAIL:
      status = "no-ssid-available";
      break;
    case WL_SCAN_COMPLETED:
      status = "scan-completed";
      break;
    case WL_CONNECTION_LOST:
      status = "connection-lost";
      break;
    case WL_DISCONNECTED:
      status = "disconnected";
      break;
    default:
      status = "unknown";
      break;
  }

  return status;
}
