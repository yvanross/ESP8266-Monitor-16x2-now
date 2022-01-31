#include <Arduino.h>
#include <FS.h>
#include <SensorConfig.h>
#include <LittleFS.h> 
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

void displayStaticFile(){
  if (!LittleFS.begin()) {
    Serial.println("main.cpp: LittleFS init failed");
  }

  Dir root = LittleFS.openDir("/");
  Serial.println("READING ROOT..");
  while (root.next()) {
    String fileName = root.fileName();
    File f = root.openFile("r");
    Serial.printf("   %s: %d\r\n", fileName.c_str(), f.size());
  }
}
void displayConfigInfo(SensorConfig* config){
  Serial.print("ChipID: ");Serial.println(ESP.getChipId());
  Serial.print("config: ");Serial.println(config->toJson());
   Serial.print("Connect to wifi with ssid: ");
  Serial.print(config->getString(SSID));
  Serial.print(" and passphrase:  " );
  Serial.println(config->getString(SSID_PASSPHRASE));
}

unsigned long display_status_counter = 0;
void DisplayStatus(){
  if(millis() - display_status_counter > 10000){
      Serial.println(millis());
      Serial.print(" Wifi status: ");Serial.print(WiFi.status());
      Serial.print(" Local IP: "); Serial.print(WiFi.localIP());
      Serial.print(" SoftAP IP: "); Serial.println(WiFi.softAPIP());
      Serial.print(" FreeHeap: "); Serial.println(ESP.getFreeHeap());
      Serial.print("Mac address: "); Serial.println(WiFi.macAddress());
      // Serial.println(levels->getChartData());
      display_status_counter = millis();
  }
}
