#ifndef WifiHelper_h
#define WifiHelper_h

#include <WifiEspNow.h>
#include "Arduino.h"
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <pin.h>
#include <SensorConfig.h>
#include <FlashingLed.h>
#include <Sensor.h>
class WifiHelper {
  public:
    WifiHelper(SensorConfig* config, Sensor* sensor);
    bool startWifi(SensorConfig* config);
    boolean startAccessPoint();
    void startAccessPointWithLed(FlashingLed* flashingLed);
    void startWifiWithLed(SensorConfig* config,FlashingLed* flashingLed);
    void startEspNow();
    bool sendDataToSlave(unsigned long value);
    boolean canSendDataToSlave();
    void enableSensorRoutes();
    SensorConfig * getConfig();
    String getConfigData();

  private:
    AsyncWebServer* server;
    bool _canSendDataToSlave = false;
   

};

#endif