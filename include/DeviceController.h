#ifndef SensorControler_h
#define SensorControler_h

#include "Arduino.h"
// #include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
// #include <LittleFS.h> 
#include "DeviceConfig.h"
#include "Button.h"
#include "FlashingLed.h"
#include "Ota.h"
#include <DeviceConfig.h>
#include <WifiManager.h>
#include <pin.h>
#include <EmailHelper.h>
#include <AbstractDeviceConfig.h>
#include <Button.h>
#include <FlashingLed.h>

// #ifdef ESP32
//   #include <WiFi.h>
// #else
//   #include <ESP8266WiFi.h>
// #endif

class DeviceController{
  public: 
    DeviceController();
    float getChartPoint();
    void loop();
    String getDataForIndex();
    bool startWifi();
    unsigned long getSamplingFrequency();
    String getConfigJson();
    void readConfig();
    void updateConfig(String key, String value);
    void writeConfig();
    void startAccessPoint();
    void startOta();

  private:
    WifiManager *wifiManager;
    DeviceConfig *config;
    Ota* ota;
    Button* button;
    FlashingLed* flashingLed;
    unsigned long _message_id = 0;

};

#endif
