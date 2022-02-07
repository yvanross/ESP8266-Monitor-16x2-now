#ifndef DeviceConfig_h
#define DeviceConfig_h

#include "Arduino.h"
#include <AbstractDeviceConfig.h>

const String USE_WIFI = "USE_WIFI";

// conntion réseau
const String SSID = "SSID";
const String SSID_PASSPHRASE = "SSID_PASSPHRASE";

// notification par courriel et sms
const String CAN_SEND_EMAIL = "CAN_SEND_EMAIL";
const String GMAIL = "GMAIL";
const String GMAIL_PASSWORD = "GMAIL_PASSWORD";
const String SMS_ADDRESS = "SMS_ADDRESS";


// Sensor
const String SENSOR_NAME = "SENSOR_NAME";
const String INCHES = "INCHES";
const String SAMPLING_RATE_SECONDES = "SAMPLING_RATE_SECONDES";
const String SAMPLING_BUFFER_SIZE = "SAMPLING_BUFFER_SIZE";
const String DELAY_INTER_MEASURE = "DELAY_INTER_MEASURE";
const String STD_DEV_MAX = "STD_DEV_MAX";
const String NB_RETRY = "NB_RETRY";

// tank
const String DISTANT_BETWEEN_SENSOR_AND_TANK_BOTTON = "DISTANCE_BETWEEN_SENSOR_AND_TANK_BOTTOM";
const String TANK_HEIGHT = "TANK_HEIGHT";

// alarme
const String THRESHOLD = "THRESHOLD";

// router mac address
const String SMAC0 = "SMAC0";
const String SMAC1 = "SMAC1";
const String SMAC2 = "SMAC2";
const String SMAC3 = "SMAC3";
const String SMAC4 = "SMAC4";
const String SMAC5 = "SMAC5";

// gps
const String LONGITUDE = "LONGITUDE";
const String LATITUDE = "LATITUDE";


class DeviceConfig :  public AbstractDeviceConfig {
  public: 
    DeviceConfig() : AbstractDeviceConfig(){};
    void read();
    uint8_t* getPeer();
    
  private:
   uint8_t _peer[6];

};

#endif
