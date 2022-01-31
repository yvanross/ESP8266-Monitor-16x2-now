#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include <EmailHelper.h>
// #include <home.h>
#include <AbstractSensorConfig.h>
#include "SensorConfig.h"
#include <CircularBufferOfUnsignedLong.h>
#include "pin.h"
#include "Button.h"
#include "FlashingLed.h"
#include "Ota.h"
#include <SensorConfig.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <LittleFS.h> 
#include <WifiHelper.h>
#include <DisplayUtil.h>
#include <Sensor.h>
// Replace with your network credentials


String getConfigData();
// ********************************************
// configuration
// ********************************************
SensorConfig *config;
Button button(PIN_BUTTON_CONFIG,PIN_LED_BLUE);
FlashingLed flashingLed(PIN_LED_BLUE);
Ota ota;
WifiHelper *wifiHelper;
Sensor *sensor;


void initPin(){
  pinMode(PIN_BUTTON_CONFIG,INPUT_PULLDOWN_16);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_TRIGGER,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_LED_BLUE,LOW);
}

void initSerial(){
   Serial.begin(115200);
  while (!Serial){
    delay(100);
  } ;
  delay(500);
  Serial.println("Sensor start");
}

void setup() {
  initPin();
  initSerial();
 
  config = new SensorConfig();
  config->read();
  sensor = new Sensor(config->getUnsignedLong(SAMPLING_BUFFER_SIZE));
  wifiHelper = new WifiHelper(config,sensor);

  
  displayStaticFile();
  // wifiHelper->startWifiWithLed(config,&flashingLed);
  wifiHelper->startEspNow();
  
  // displayConfigInfo();
  // EmailHelper::sendEmail(
  //       config->getBoolean(CAN_SEND_EMAIL),
  //       config->getString(GMAIL),
  //       config->getString(GMAIL_PASSWORD),
  //       config->getString(SMS_ADDRESS),
  //       "Demarrage du capteur: " + config->getString(SENSOR_NAME),
  //       " Adresse ip " + WiFi.localIP().toString());
} 

void loop() {
  if(button.pressed(&flashingLed)){
    int count = button.pressCount();
    
    if(count == 1 ||count == 4 || count == 5 ){
      wifiHelper->startWifiWithLed(config,&flashingLed);
    }

    if(count == 2){
     wifiHelper->startAccessPointWithLed(&flashingLed);
    }
    if(count == 3){
      Serial.println("StartESPNow");
      wifiHelper->startEspNow();
      flashingLed.start(100);
    }

    if (count == 6){
      flashingLed.start(10);
      ota.setup(
        config->getString(SSID),
        config->getString(SSID_PASSPHRASE));
    }

  }
  flashingLed.flash();
  ota.handle();
  // if(sensor->read(5000)){
  //   unsigned long distance = sensor->readLastValue(10);  
  //   Serial.print(distance);
  //   Serial.println(" cm");

  //   if(wifiHelper->canSendDataToSlave()){
  //     Serial.println("Send data throught ESP-Now");
  //     wifiHelper->sendDataToSlave(distance);
  //   }
  // }
  delay(100);

  // if(WiFi.softAPgetStationNum() > 0)
    // Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());

 
}

