#ifndef SensorConfig_h
#define SensorConfig_h

#include "Arduino.h"
#include <ArduinoJson.h>  //https://arduinojson.org/v6/doc/
#include <ESP8266WebServer.h>
#include "AbstractSensorConfig.h"


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


class SensorConfig :  public AbstractSensorConfig {
  public: 
    SensorConfig() : AbstractSensorConfig(){};
    // methods to overwrite
    bool write(ESP8266WebServer* server,HardwareSerial);
    String toString();
    String toHtmlForm();
    bool readConfig();
    

    // current sensor methods
    String getSensorName() {return _sensor_name;}
    String getGmail(){return _gmail;}
    String getGmailPassword() {return _gmail_password;}
    String getssid_passphrase() {return _ssid_passphrase;}
    String getSmsAddress(){return _sms_address;}
    String getSsid() {return _ssid;}
  
    boolean can_play_alarm(){return _can_play_alarm;}
    boolean canSendEmail(){return _can_send_email;}
    unsigned long getSamplingRate(){return _sampling_rate_secondes * 1000;}
    unsigned long getSamplingBufferSize(){return _sampling_buffer_size;}
    unsigned long getFilterSize(){return _filter_size;}
    unsigned long getThreshold(){return _threshold;}
    unsigned long getGain(){return _gain;}
    void enableEmail(boolean state); 

  private:
    String _gmail = "yvan.ross.sms@gmail.com";
    String _gmail_password = "lgeteknpwtjfxolf";
    String _ssid_passphrase = "D93D5AD2EA5C";
    String _sms_address = "5147978894@msg.telus.com";
    String _ssid = "BELL391";
    String _sensor_name = "Pump";
    boolean _can_send_email = false;
    boolean _can_play_alarm = false;
    unsigned long _sampling_rate_secondes = 1; 
    unsigned long _sampling_buffer_size = 60;   
    unsigned long _filter_size = 30;
    unsigned long _threshold = 50;
    unsigned long _gain =1 ;
  
    void updatePropFromArgs(ESP8266WebServer* server);
};

#endif
