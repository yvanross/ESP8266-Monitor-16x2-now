#ifndef SensorConfig_h
#define SensorConfig_h

#include "Arduino.h"
#include <AbstractSensorConfig.h>


const String SSID = "ssid";
const String SSID_PASSPHRASE = "ssid_passphrase";
const String GMAIL = "gmail";
const String GMAIL_PASSWORD = "gmail_password";
const String SMS_ADDRESS = "sms_address";
const String SAMPLING_RATE_SECONDES = "sampling_rate_secondes";
const String SENSOR_NAME = "Vaccum";
const String CAN_SEND_EMAIL = "can_send_email";
const String SAMPLING_BUFFER_SIZE = "sampling_buffer_size";
const String FILTER_SIZE = "filter_size";
const String THRESHOLD = "threshold";
const String GAIN = "gain";
const String USE_ANALOG = "use_analog";


class SensorConfig :  public AbstractSensorConfig {
  public: 
    SensorConfig() : AbstractSensorConfig(){};
    void read();

  private:
   
};

#endif
