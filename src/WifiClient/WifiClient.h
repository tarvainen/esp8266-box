#include <Arduino.h>

class WifiClient
{
  private:
    char* ssid;
    char* password;

  public:
    void configure(char*, char*);
    void connect();
    void disconnect();
    int status();
    String statusText();
};
