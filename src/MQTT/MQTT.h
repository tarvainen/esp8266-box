#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Arduino.h>

using namespace std;

class MQTT
{
  private:
    char* username;
    char* password;
    char* hostname;
    boolean shouldTryReconnect = false;
    PubSubClient *pubSub;
    String subscriptions[50];

  public:
    MQTT(PubSubClient&);
    void configure(char*, char*, char*);
    boolean connect();
    void process();
    void disconnect();
    void publish(char*, const char*);
    void subscribe(char*);
    void unsubscribe(char*);
    int status();
    String statusText();
};
