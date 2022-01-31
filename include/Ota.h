#ifndef Ota_h
#define Ota_h
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class Ota {
  public: 
    Ota();
    void setup(const String ssid, const String password);
    void handle();
};

#endif