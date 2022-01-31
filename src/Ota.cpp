#include "Ota.h"

Ota::Ota(){}

void Ota::setup(const String ssid, const String password){
  Serial.println("OTA setup");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Serial.println("Wait for wifi connection");
  unsigned long startAt = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(1000);
    if(millis()-startAt > 30000) {
      Serial.println("Connection Failed! Rebooting...");
      ESP.restart();
    }  
  }

  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start ota");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd ota");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.print("OTA Ready: ");
  Serial.println(WiFi.localIP());
}

void Ota::handle(){
  ArduinoOTA.handle();
}