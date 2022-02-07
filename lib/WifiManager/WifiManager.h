#ifndef WifiManager_h
#define WifiManager_h

// #include <WifiEspNow.h>
#include "Arduino.h"
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
// #include <pin.h>
// #include <DeviceConfig.h>
// #include <FlashingLed.h>
// #include <Sensor.h>
// #include <MessageNow.h>


class WifiManager {
  
  public:
    WifiManager();

    bool startWifi(String ssid, String password);
    bool startAccessPoint();
    void startEspNow();

    boolean cansendDataToPeer();
    bool sendDataToPeer(uint8_t* peer,uint8_t* message);
    // bool sendDataToPeer(uint8_t* peer, MessageNow::sensorName_t);
    // bool sendDataToPeer(uint8_t* peer, MessageNow::measure_t);
    // bool sendDataToPeer(uint8_t* peer, MessageNow::alarm16x2_t);
    // bool sendDataToPeer(uint8_t* peer, MessageNow::ack_t);
    
    
  private:
    bool successfully_send(bool ok);
    bool _cansendDataToPeer = false;
    unsigned long _message_number = 0;

};

#endif