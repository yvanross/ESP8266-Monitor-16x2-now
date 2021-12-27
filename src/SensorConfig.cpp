#include "SensorConfig.h"
  
void SensorConfig::read(){
  AbstractSensorConfig::read();
  if(!containKey(SSID))
    update(SSID,"BELL391");
  
if(!containKey(SSID_PASSPHRASE))
    update(SSID_PASSPHRASE,"D93D5AD2EA5C");

if(!containKey(GMAIL))
    update(GMAIL,"yvan.ross.sms@gmail.com");
  
if(!containKey(GMAIL_PASSWORD))
    update(GMAIL_PASSWORD,"lgeteknpwtjfxolf");
    
  if(!containKey(SMS_ADDRESS))
    update(SMS_ADDRESS,"5147978894@msg.telus.com");
    
  if(!containKey(SENSOR_NAME))
    update(SENSOR_NAME, "Vaccum Pump");
  
  if(!containKey(CAN_SEND_EMAIL))
    update(CAN_SEND_EMAIL,"false");

  if(!containKey(SAMPLING_BUFFER_SIZE))
    update(SAMPLING_BUFFER_SIZE,"60");
  
  if(!containKey(SAMPLING_RATE_SECONDES))
    update(SAMPLING_RATE_SECONDES,"5");
    
  if(!containKey(FILTER_SIZE))
    update(FILTER_SIZE,"1");    // unsigned long _filter_size = 30;
    
  if(!containKey(THRESHOLD))
    update(THRESHOLD,"50");

  if(!containKey(GAIN))
    update(GAIN,"1");

  if(getUnsignedLong(FILTER_SIZE) > getUnsignedLong(SAMPLING_BUFFER_SIZE))
    update(FILTER_SIZE,String(getUnsignedLong(SAMPLING_BUFFER_SIZE)));

  if(!containKey(USE_ANALOG))
    update(USE_ANALOG,"true");

}


