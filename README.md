# ESP8266-BOX

Simple utility for the ESP8266 module to make development and proof of concepts faster. Main concept is to be able to control connectivity features using only simple commands without having to write custom code for your ESP8266 and just go fast track by plugging it in to your Arduino and going online with that!

Features:

- [x] WiFi
- [x] MQTT
- [ ] HTTP Client
- [ ] HTTP Server
- [ ] MDNS
- [ ] GPIO(?)

## Table of Contents

* [ESP8266-BOX](#esp8266-box)
* [Table of Contents](#table-of-contents)
* [Requirements](#requirements)
* [Installation](#installation)
* [Available commands](#available-commands)
* [Examples](#examples)
  * [Publish message to MQTT](#publish-message-to-mqtt)
  * [Connect with Arduino](#connect-with-arduino)

## Requirements

TODO

## Installation

Download the source code and flash it to your ESP8266 module.

## Available commands

Commands are sent to the ESP8266 module over the serial connection. You can play with them using the Arduino IDE's Serial Monitor.

```
# WiFi management
wifi-status
wifi-configure -ssid <ssid> -password <password>
wifi-connect
wifi-disconnect

# MQTT
mqtt-status
mqtt-configure -username <username> -password <password> -hostname <hostname>
mqtt-connect
mqtt-disconnect
mqtt-publish -topic <topic> -payload <payload>
mqtt-subscribe -topic <topic>
mqtt-unsubscribe -topic <topic>
```

## Examples

### Publish message to MQTT

```
# connect wifi
wifi-configure -ssid mywifissid -password mywifipassword
wifi-connect

# connect mqtt
mqtt-configure -username rabbitmq -password rabbitmq -hostname localhost
mqtt-connect

# publish message
mqtt-publish -topic offtopic -payload helloworld
```

### Connect with Arduino

TODO
