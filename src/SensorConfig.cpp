#include "SensorConfig.h"

 // String _gmail = "yvan.ross.sms@gmail.com";
    // String _gmail_password = "lgeteknpwtjfxolf";
    // String _ssid_passphrase = "D93D5AD2EA5C";
    // String _sms_address = "5147978894@msg.telus.com";
    // String _sensor_name = "Pump";
    // boolean _can_send_email = false;
    // boolean _can_play_alarm = false;
    // unsigned long _sampling_rate_secondes = 1; 
    // unsigned long _sampling_buffer_size = 60;   
    // unsigned long _filter_size = 30;
    // unsigned long _threshold = 50;
    // unsigned long _gain =1 ;
  // _can_send_email = readProperty(obj, CAN_SEND_EMAIL, _can_send_email);
  // _sampling_buffer_size = readProperty(obj, SAMPLING_BUFFER_SIZE, _sampling_buffer_size);
  // _filter_size = readProperty(obj, FILTER_SIZE, _filter_size);
  // _threshold = readProperty(obj,THRESHOLD,_threshold);
  // _gain = readProperty(obj,GAIN,_gain);
  // if(_sampling_buffer_size == 0) _sampling_buffer_size = 60;
  // if(_filter_size == 0) _filter_size = 30;
  // if(_threshold == 0) _threshold = 10;
  // if(_gain == 0) _gain = 1;
  
void SensorConfig::read(){
  AbstractSensorConfig::read();
  if(getString(SSID) == NULL)
    update(SSID,"BELL391");
  
  if(getString(SSID_PASSPHRASE) == NULL)
    update(SSID_PASSPHRASE,"D93D5AD2EA5C");

  if(getString(GMAIL) == NULL)
    update(GMAIL,"yvan.ross.sms@gmail.com");
  
  if(getString(GMAIL_PASSWORD) == NULL)
    update(GMAIL_PASSWORD,"lgeteknpwtjfxolf");
    
  if(getString(SMS_ADDRESS)==NULL)
    update(SMS_ADDRESS,"5147978894@msg.telus.com");
    
  if(getString(SENSOR_NAME) == NULL)
    update(SENSOR_NAME, "Vaccum Pump");
  
  if(getBoolean(CAN_SEND_EMAIL) == NULL)
    update(CAN_SEND_EMAIL,"false");

  if(getUnsignedLong(SAMPLING_BUFFER_SIZE) == NULL)
    update(SAMPLING_BUFFER_SIZE,"60");
  
  if(getUnsignedLong(SAMPLING_RATE_SECONDES) == NULL)
    update(SAMPLING_RATE_SECONDES,"5");
    
  if(getUnsignedLong(FILTER_SIZE) == NULL)
    update(FILTER_SIZE,"1");    // unsigned long _filter_size = 30;
    
  if(getUnsignedLong(THRESHOLD) == NULL)
    update(THRESHOLD,"50");

  if(getUnsignedLong(GAIN) == NULL)
    update(GAIN,"1");

  if(getUnsignedLong(FILTER_SIZE) > getUnsignedLong(SAMPLING_BUFFER_SIZE))
    update(FILTER_SIZE,String(getUnsignedLong(SAMPLING_BUFFER_SIZE)));


}


