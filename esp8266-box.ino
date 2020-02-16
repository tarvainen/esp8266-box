#include <Arduino.h>
#include <SimpleCLI.h>
#include "src/MQTT/MQTT.h"
#include "src/WiFiClient/WifiClient.h"

SimpleCLI cli;

WiFiClient wifiClient;
PubSubClient pubSub(wifiClient);
MQTT mqttClient(pubSub);

WifiClient wifi;

Command cmdWifiConfigure;
Command cmdWifiConnect;
Command cmdWifiStatus;
Command cmdWifiDisconnect;

Command cmdMqttConfigure;
Command cmdMqttConnect;
Command cmdMqttDisconnect;
Command cmdMqttStatus;
Command cmdMqttPublish;
Command cmdMqttSubscribe;
Command cmdMqttUnsubscribe;

int wifiStatus;
int mqttStatus;

char* to_char(String string) {
  char* result = new char[string.length() + 1]();

  string.toCharArray(result, string.length() + 1);

  return result;
}

void cli_error_callback(cmd_error* e) {
  CommandError cmdError(e);

  Serial.print("ERROR: ");
  Serial.println(cmdError.toString());

  if (cmdError.hasCommand()) {
    Serial.print("Did you mean \"");
    Serial.print(cmdError.getCommand().toString());
    Serial.println("\"?");
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("mqtt-receive -topic ");
  Serial.print(topic);

  Serial.print(" -payload ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void setup() {
  Serial.begin(9600);

  cli.setOnError(cli_error_callback);

  pubSub.setCallback(mqtt_callback);

  cmdWifiConfigure = cli.addCommand("wifi-configure");
  cmdWifiConfigure.addArg("ssid", "");
  cmdWifiConfigure.addArg("password", "");
  cmdWifiConfigure.setDescription("Configure wifi SSID and Password");

  cmdWifiConnect = cli.addCmd("wifi-connect");
  cmdWifiConnect.setDescription("Connect to wifi");

  cmdWifiStatus = cli.addCmd("wifi-status");
  cmdWifiStatus.setDescription("Request send wifi status");

  cmdWifiDisconnect = cli.addCmd("wifi-disconnect");
  cmdWifiStatus.setDescription("Disconnect wifi");

  cmdMqttConfigure = cli.addCmd("mqtt-configure");
  cmdMqttConfigure.addArg("username", "");
  cmdMqttConfigure.addArg("password", "");
  cmdMqttConfigure.addArg("hostname", "");
  cmdMqttConnect.setDescription("Configure MQTT connection");

  cmdMqttConnect = cli.addCmd("mqtt-connect");
  cmdMqttConnect.setDescription("Connect to MQTT broker");

  cmdMqttDisconnect = cli.addCmd("mqtt-disconnect");
  cmdMqttDisconnect.setDescription("Disconnect from MQTT broker");

  cmdMqttStatus = cli.addCmd("mqtt-status");
  cmdMqttStatus.setDescription("Request send MQTT status");

  cmdMqttPublish = cli.addCmd("mqtt-publish");
  cmdMqttPublish.addArg("topic", "");
  cmdMqttPublish.addArg("payload", "");
  cmdMqttPublish.setDescription("Publish a message to MQTT topic");

  cmdMqttSubscribe = cli.addCmd("mqtt-subscribe");
  cmdMqttSubscribe.addArg("topic", "");
  cmdMqttSubscribe.setDescription("Subscribe MQTT topic");

  cmdMqttUnsubscribe = cli.addCmd("mqtt-unsubscribe");
  cmdMqttUnsubscribe.addArg("topic", "");
  cmdMqttUnsubscribe.setDescription("Unsubscribe MQTT topic");
}

void print_wifi_status() {
    char statusText[255];
    sprintf(statusText, "info -wifistatustext %s -wifistatuscode %u", to_char(wifi.statusText()), wifi.status());

    Serial.println(statusText);
}

void print_mqtt_status() {
    char statusText[255];
    sprintf(statusText, "info -mqttstatustext %s -mqttstatuscode %u", to_char(mqttClient.statusText()), mqttClient.status());

    Serial.println(statusText);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input.length() > 0) {
        cli.parse(input);
    }

    if (cli.available()) {
      Command cmd = cli.getCmd();

      if (cmd == cmdWifiConfigure) {
        char* ssid = to_char(cmd.getArgument("ssid").getValue());
        char* password = to_char(cmd.getArgument("password").getValue());

        wifi.configure(ssid, password);
      } else if (cmd == cmdWifiConnect) {
        wifi.connect();
      } else if (cmd == cmdWifiStatus) {
        print_wifi_status();
      } else if (cmd == cmdWifiDisconnect) {
        wifi.disconnect();
      } else if (cmd == cmdMqttConfigure) {
        char* username = to_char(cmd.getArgument("username").getValue());
        char* password = to_char(cmd.getArgument("password").getValue());
        char* hostname = to_char(cmd.getArgument("hostname").getValue());

        mqttClient.configure(username, password, hostname);
      } else if (cmd == cmdMqttConnect) {
        mqttClient.connect();
      } else if (cmd == cmdMqttDisconnect) {
        mqttClient.disconnect();
      } else if (cmd == cmdMqttStatus) {
        print_mqtt_status();
      } else if (cmd == cmdMqttPublish) {
        char* topic = to_char(cmd.getArgument("topic").getValue());
        const char* payload = to_char(cmd.getArgument("payload").getValue());

        mqttClient.publish(topic, payload);
      } else if (cmd == cmdMqttSubscribe) {
        char* topic = to_char(cmd.getArgument("topic").getValue());

        mqttClient.subscribe(topic);
      } else if (cmd == cmdMqttUnsubscribe) {
        char* topic = to_char(cmd.getArgument("topic").getValue());

        mqttClient.unsubscribe(topic);
      }
    }
  }

  // If MQTT connected, process subscriptions etc.
  mqttClient.process();

  // Notify about WIFI status changes
  if (wifi.status() != wifiStatus) {
    print_wifi_status();
    wifiStatus = wifi.status();
  }

  // Notify about MQTT connection changes
  if (mqttClient.status() != mqttStatus) {
    print_mqtt_status();
    mqttStatus = mqttClient.status();
  }
}
