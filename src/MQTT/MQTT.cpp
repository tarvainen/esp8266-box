#include "MQTT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

using namespace std;

MQTT::MQTT(PubSubClient& pubSub) {
  this->pubSub = &pubSub;
}

void MQTT::configure(char* username, char* password, char* hostname) {
  this->username = username;
  this->password = password;
  this->hostname = hostname;

  this->pubSub->setServer(hostname, 1883);
}

boolean MQTT::connect() {
  this->pubSub->connect("esp8266-box", this->username, this->password);
  this->shouldTryReconnect = true;
}

void MQTT::process() {
  if (this->shouldTryReconnect) {
    if (!this->pubSub->connected()) {
      this->pubSub->connect("esp8266-box", this->username, this->password);

      for (int i = 0; i < 50; i++) {
        if (this->subscriptions[i] != "") {
          this->subscribe((char*)this->subscriptions[i].c_str());
        }
      }
    }

    this->pubSub->loop();
  }
}

void MQTT::disconnect() {
  this->pubSub->disconnect();
  this->shouldTryReconnect = false;
}

void MQTT::publish(char* topic, const char* payload) {
  this->pubSub->publish(topic, payload);
}

void MQTT::subscribe(char* topic) {
  this->pubSub->subscribe(topic);

  for (int i = 0; i < 50; i++) {
    if (this->subscriptions[i] == "") {
      this->subscriptions[i] = topic;
      break;
    }
  }
}

void MQTT::unsubscribe(char* topic) {
  this->pubSub->unsubscribe(topic);

  for (int i = 0; i < 50; i++) {
    if (this->subscriptions[i] == topic) {
      this->subscriptions[i] = "";
      break;
    }
  }
}

int MQTT::status() {
  return this->pubSub->state();
}

String MQTT::statusText() {
  String status;

  switch (this->pubSub->state()) {
    case MQTT_CONNECTION_TIMEOUT:
      status = "timeout";
      break;
    case MQTT_CONNECTION_LOST:
      status = "connection-lost";
      break;
    case MQTT_CONNECT_FAILED:
      status = "connect-failed";
      break;
    case MQTT_DISCONNECTED:
      status = "disconnected";
      break;
    case MQTT_CONNECTED:
      status = "connected";
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      status = "bad-protocol";
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      status = "bad-client-id";
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      status = "unavailable";
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      status = "bad-credentials";
      break;
    case MQTT_CONNECT_UNAUTHORIZED:
      status = "unauthorized";
      break;
    default:
      status = "unknown";
      break;
  }

  return status;
}
