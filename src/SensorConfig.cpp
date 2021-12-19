#include "Arduino.h"
#include "SensorConfig.h"
#include "LittleFS.h" // LittleFS is declared
#include <ArduinoJson.h>  //https://arduinojson.org/v6/doc/
#include <ESP8266WebServer.h>



void SensorConfig::enableEmail(boolean state){
  _can_send_email = state;
}

bool SensorConfig::readConfig(){
  String data = readData();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, data);
  JsonObject obj = doc.as<JsonObject>();
  if (error) {
    Serial.println("ERROR sensor config: " + String(error.f_str()));
    return false;
  }
  _ssid = readProperty(obj, SSID, _ssid);
  _ssid_passphrase = readProperty(obj, SSID_PASSPHRASE, _ssid_passphrase);
  _gmail = readProperty(obj, GMAIL, _gmail);
  _gmail_password = readProperty(obj, GMAIL_PASSWORD, _gmail_password);
  _sms_address = readProperty(obj, SMS_ADDRESS, _sms_address);
  _sampling_rate_secondes = readProperty(obj, SAMPLING_RATE_SECONDES, _sampling_rate_secondes);
  _sensor_name = readProperty(obj, SENSOR_NAME, _sensor_name);
  _can_send_email = readProperty(obj, CAN_SEND_EMAIL, _can_send_email);
  _sampling_buffer_size = readProperty(obj, SAMPLING_BUFFER_SIZE, _sampling_buffer_size);
  _filter_size = readProperty(obj, FILTER_SIZE, _filter_size);
  _threshold = readProperty(obj,THRESHOLD,_threshold);
  _gain = readProperty(obj,GAIN,_gain);
  if(_sampling_buffer_size == 0) _sampling_buffer_size = 60;
  if(_filter_size == 0) _filter_size = 30;
  if(_threshold == 0) _threshold = 10;
  if(_gain == 0) _gain = 1;

  
  return true;
}


bool SensorConfig::write(ESP8266WebServer* server,HardwareSerial serial) {
  updatePropFromArgs(server);
 
  DynamicJsonDocument doc(1024);
  doc[GMAIL] = _gmail;
  doc[GMAIL_PASSWORD] = _gmail_password;
  doc[SSID_PASSPHRASE] = _ssid_passphrase;
  doc[SMS_ADDRESS] = _sms_address;
  doc[SSID] = _ssid;
  doc[SENSOR_NAME] = _sensor_name;
  doc[SAMPLING_RATE_SECONDES] = _sampling_rate_secondes;
  doc[CAN_SEND_EMAIL] = _can_send_email;
  doc[SAMPLING_BUFFER_SIZE] = _sampling_buffer_size;
  doc[FILTER_SIZE] = _filter_size;
  doc[THRESHOLD] = _threshold;
  doc[GAIN] = _gain;
  
  String data;
 size_t size =  serializeJson(doc, data);
  serial.println("SensorConfig::write");
  serial.println( "serialization size = " + String(size) );
  serial.println("Write to file:" + data);
  File file = LittleFS.open("/config.txt", "w");
  file.print(data);
  file.close();
  return true;
}

void SensorConfig::updatePropFromArgs(ESP8266WebServer* server){
  _gmail = writeProperty(server, GMAIL, _gmail);
  _gmail_password = writeProperty(server, GMAIL_PASSWORD, _gmail_password);
  _ssid_passphrase = writeProperty(server, SSID_PASSPHRASE, _ssid_passphrase);
  _sms_address = writeProperty(server, SMS_ADDRESS, _sms_address);
  _ssid = writeProperty(server, SSID, _ssid);  
  _sensor_name = writeProperty(server, SENSOR_NAME, _sensor_name);
  _sampling_rate_secondes = writeProperty(server,SAMPLING_RATE_SECONDES,_sampling_rate_secondes);
  _can_send_email = writeProperty(server,CAN_SEND_EMAIL,_can_send_email);
  _sampling_buffer_size = writeProperty(server,SAMPLING_BUFFER_SIZE,_sampling_buffer_size);
  _filter_size = writeProperty(server,FILTER_SIZE,_filter_size);
  _threshold = writeProperty(server,THRESHOLD,_threshold);
  _gain = writeProperty(server,GAIN,_gain);
  
  
}


String SensorConfig::toString(){
  String content = "\nCurrent configuration\n";
  content += "   "+GMAIL+ ": " + _gmail + "\n";
  content += "   "+GMAIL_PASSWORD+": " + _gmail_password + "\n";
  content += "   "+SAMPLING_RATE_SECONDES+": " + String(_sampling_rate_secondes) + "sec \n";
  content += "   "+SMS_ADDRESS+": " + _sms_address + "\n";
  content += "   "+SSID+": " + _ssid + "\n";
  content += "   "+SSID_PASSPHRASE+": " + _ssid_passphrase + "\n";
  content += "   "+CAN_SEND_EMAIL+": " + String(_can_send_email) + "\n";
  content += "   "+SAMPLING_BUFFER_SIZE+": " + String(_sampling_buffer_size) + "\n";
  content += "   "+FILTER_SIZE+": " + String(_filter_size) + "\n";
  content += "   "+THRESHOLD+": " + String(_threshold) + "\n";
  content += "   "+GAIN+": " + String(_gain) + "\n";
  
  
  return content;
};

String SensorConfig::toHtmlForm(){
  String content = "";
  content += "<p><label>Gmail account/</label>&nbsp:&nbsp<input name='"+GMAIL+"' length=32>&nbsp"+ _gmail + "</p>";
  content += "<p><label>Gmail password</label>&nbsp:&nbsp<input name='"+GMAIL_PASSWORD+"' length=32>&nbsp"+ _gmail_password + "</p>";
  content += "<p><label>Sampling rate (sec)</label>&nbsp:&nbsp<input name='"+SAMPLING_RATE_SECONDES+"' length=32>&nbsp" + String(_sampling_rate_secondes) + "sec </p>";
  content += "<p><label>Sms email</label>&nbsp:&nbsp<input name='"+SMS_ADDRESS+"' length=32>&nbsp"+ _sms_address + "</p>";
  content += "<p><label>SSID</label>&nbsp:&nbsp<input name='"+SSID+"' length=32>&nbsp"+ _ssid + "</p>";
  content += "<p><label>Ssid passphrase</label>&nbsp:&nbsp<input name='"+SSID_PASSPHRASE+"' length=32>&nbsp"+ _ssid_passphrase+ "</p>";
  content += "<p><label>Can send email</label>&nbsp:&nbsp<input name='"+CAN_SEND_EMAIL+"' length=32>&nbsp" + String(_can_send_email) + "  true/false</p>";
  content += "<p><label>Sampling buffer size</label>&nbsp:&nbsp<input name='"+SAMPLING_BUFFER_SIZE+"' length=32>&nbsp" + String(_sampling_buffer_size) + "  < 100</p>";
  content += "<p><label>Filter size</label>&nbsp:&nbsp<input name='"+FILTER_SIZE+"' length=32>&nbsp" + String(_filter_size) + " filter_size < sampling buffer size</p>";
  content += "<p><label>Threshold</label>&nbsp:&nbsp<input name='"+THRESHOLD+"' length=32>&nbsp" + String(_threshold) + " 0< threshold<1024</p>";
  content += "<p><label>Gain</label>&nbsp:&nbsp<input name='"+GAIN+"' length=32>&nbsp" + String(_gain) + " >0</p>";
  return content;
}
