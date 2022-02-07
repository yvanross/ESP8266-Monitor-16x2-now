#include "DeviceConfig.h"
  
void DeviceConfig::read(){
  AbstractDeviceConfig::read();

  if(!containKey(USE_WIFI))
    update(USE_WIFI,"true");

  if(!containKey(SSID))
    update(SSID,"BELL391");
  
if(!containKey(SSID_PASSPHRASE))
    update(SSID_PASSPHRASE,"D93D5AD2EA5C");

if(!containKey(CAN_SEND_EMAIL))
    update(CAN_SEND_EMAIL,"false");

if(!containKey(GMAIL))
    update(GMAIL,"yvan.ross.sms@gmail.com");
  
if(!containKey(GMAIL_PASSWORD))
    update(GMAIL_PASSWORD,"lgeteknpwtjfxolf");
    
  if(!containKey(SMS_ADDRESS))
    update(SMS_ADDRESS,"5147978894@msg.telus.com");
    
  if(!containKey(SENSOR_NAME))
    update(SENSOR_NAME, "Ultrasonic Distance");
  
  if(!containKey(INCHES))
    update(INCHES,"true");
  
  if(!containKey(SAMPLING_RATE_SECONDES))
    update(SAMPLING_RATE_SECONDES,"5");
    
  if(!containKey(SAMPLING_BUFFER_SIZE))
    update(SAMPLING_BUFFER_SIZE,"5");
    
  
  if(!containKey(DELAY_INTER_MEASURE))
    update(DELAY_INTER_MEASURE,"10");
  
  if(!containKey(STD_DEV_MAX))
    update(STD_DEV_MAX,"0.5");
    
  if(!containKey(NB_RETRY))
    update(NB_RETRY,"10");    // unsigned long _filter_size = 30;
    
 if(!containKey(NB_RETRY))
    update(NB_RETRY,"10");    // unsigned long _filter_size = 30;
  
  if(!containKey(DISTANT_BETWEEN_SENSOR_AND_TANK_BOTTON))
    update(DISTANT_BETWEEN_SENSOR_AND_TANK_BOTTON,"40");

if(!containKey(TANK_HEIGHT))
    update(TANK_HEIGHT,"30.0");

if(!containKey(THRESHOLD))
    update(THRESHOLD,"50,75");

}
  
uint8_t* DeviceConfig::getPeer(){
  char * pEnd;
  _peer[0] = strtol(getString(SMAC0).c_str(),&pEnd,16);
  _peer[1] = strtol(getString(SMAC1).c_str(),&pEnd,16);
  _peer[2] = strtol(getString(SMAC2).c_str(),&pEnd,16);
  _peer[3] = strtol(getString(SMAC3).c_str(),&pEnd,16);
  _peer[4] = strtol(getString(SMAC4).c_str(),&pEnd,16);
  _peer[5] = strtol(getString(SMAC5).c_str(),&pEnd,16);
  return _peer;
}
