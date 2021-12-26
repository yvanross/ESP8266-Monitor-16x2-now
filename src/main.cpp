#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
// #include <EMailSender.h>
#include <EmailHelper.h>
#include <home.h>
#include <WifiHelper.h>
#include "pin.h"
#include "SensorConfig.h"
#include <CircularBufferOfUnsignedLong.h>
#include <FS.h>
#include <LittleFS.h> 
// #include <AbstractSensorConfig.h>



void handlePing();
void enableSensorRoutes();
void filldynamicdata();
String getConfigData();

// ********************************************
// configuration
// ********************************************
SensorConfig *config;
WifiHelper *wifiHelper;
AsyncWebServer* server;
CircularBufferOfUnsignedLong* levels;
CircularBufferOfUnsignedLong* times;

unsigned long last_read_sensor_time = 0;

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  // message += server->uri();
  // message += "\nMethod: ";
  // message += (server->method() == HTTP_GET) ? "GET" : "POST";
  // message += "\nArguments: ";
  // message += server->args();
  // message += "\n";
  // for (uint8_t i = 0; i < server->args(); i++) {
  //   message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  // }
  // server->send(404, "text/plain", message);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
   return "allo";
  } else if (var == "TEMPERATURE"){
    return "temperateure";
  }
  return " ";
}


String readLastValue(){
  unsigned long value = levels->getLastInputFiltered();
  // Serial.print("     readLastValue = ");
  // Serial.println(value);
 return String(value);
}
String getConfigData(){
  return config->toJson();
}
void enableSensorRoutes() {
  delay(2000);
  Serial.println("enableSensorRoutes");
  delay(10);
    if(!LittleFS.begin()){
        Serial.println("ERROR:An Error has occurred while mounting LittleFS");
        // return;
    }

  // async request
 server->on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("test verify config");
    String ssid = config->getString(SSID);

     request->send(LittleFS, "/index.html");
  });

  server->on("/clean", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Delete config.txt file and reset");
    config->remove();
    request->redirect("192.168.2.237");
    ESP.reset();
  });

   server->on("/level", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",readLastValue().c_str());
  });

  server->on("/config_data", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("/config_data called");
    Serial.println(getConfigData());
    request->send_P(200, "text/plain",getConfigData().c_str());
  });
  
  server->on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Settings called");
    config->read();

    for(unsigned long i = 0; i < request->args(); i++){
      if(request->arg(i).length() > 0)
        config->update(request->argName(i),request->arg(i));
    }
    config->write();
    request->send(LittleFS, "/config.html");
  });
  
  server->on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("reset ESP8266");
    ESP.reset();
  });

  server->on("/accesspoint", HTTP_GET, [](AsyncWebServerRequest *request){
    // WiFi.mode(WIFI_STA);
    WiFi.disconnect();
//  wifiHelper->setupAccessPoint();
     request->redirect("192.168.4.1");
  });

  // JSON format response
  server->on("/ping", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Ping");
    String content = "{\"name\":\"" + config->getString(SENSOR_NAME) + "\"}";
    // request->sendHeader("Access-Control-Allow-Origin", "*");
    request->send_P(162, "text/json", content.c_str());
  });

  // server->on ("/filldynamicdata", filldynamicdata );
  server->serveStatic("/favicon.ico",LittleFS,"favicon.ico","max-age=43200");
  server->serveStatic("/style.css",LittleFS,"style.css","max-age=43200");
  server->serveStatic("/microajax.min.js",LittleFS,"microajax.js", "max-age=43200");
  server->serveStatic("/highcharts.js",LittleFS,"highcharts.js", "max-age=43200");
  server->serveStatic("/chart.min.js",LittleFS,"chart.min.js", "max-age=43200");
  server->serveStatic("/",LittleFS,"index.html", "max-age=43200");
  server->serveStatic("/index",LittleFS,"index.html", "max-age=43200");
  server->serveStatic("/config",LittleFS,"config.html", "max-age=43200");
  server->serveStatic("/chart",LittleFS,"chart.html", "max-age=43200");
   
  //  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   Serial.println("Index.html requested");
  //   request->send(LittleFS, "/index.html");
  // });
  //  server->on("/index", HTTP_GET, [](AsyncWebServerRequest *request){
  //   Serial.println("Index.html requested");
  //   request->send(LittleFS, "/index.html");
  // });
  
  //  server->on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
  //    Serial.println("/config.hml requested");
  //   request->send(LittleFS, "/config.html");
  // });
  // server->on("/chart", HTTP_GET, [](AsyncWebServerRequest *request){
  //   Serial.println("/chart.hml requested");
  //   request->send(LittleFS, "/chart.html");
  // });
 
 
  // server->onNotFound(handleNotFound);

  server->begin();
}

boolean detectRunningSound(){
  return levels->getLastInputFiltered() > config->getUnsignedLong(THRESHOLD);
}

boolean state_running = false;
unsigned long count_running =0;
unsigned long count_stopped;

void readSoundLevel(){
  int sensorValue = analogRead(A0);
  analogWrite(LED_WHITE,sensorValue); 

  levels->addData(sensorValue * config->getUnsignedLong(GAIN),config->getUnsignedLong(FILTER_SIZE));
  times->addData(millis(),1UL);

  if(detectRunningSound()){
    count_running++;
    count_stopped = 0;
    if(!state_running && count_running > 1000){
      digitalWrite(LED_GREEN,HIGH);
      Serial.print(millis());
      Serial.print( " - ");
      Serial.println("CHANGE STATE TO RUNNING");
      state_running =true;
    }
  } else {
    count_running = 0;
    count_stopped++;
    if(state_running && count_stopped > 1000){
      digitalWrite(LED_GREEN,LOW);
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println("CHANGE STATE TO STOPPED");
      state_running = false;
    }
  }

}

void filldynamicdata(){ 
  Serial.println("fillDynamicData called");       
    String values ="Le seuil est ajusté à " + String(config->getUnsignedLong(THRESHOLD));
    values += "mydynamicdata|" + (String) + "This is my Dynamic Value" + "|div\n";   // Build a string, like this:  ID|VALUE|TYPE
    // server->send ( 200, "text/plain", values);   
}


void setup() {
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  } ;
  Serial.println("Setup");
  pinMode(BUTTON_CONFIG,INPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_WHITE,OUTPUT);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_WHITE,HIGH);
  analogWriteRange(255); //255 default
  
 
  config = new SensorConfig();
  config->read();
  server = new AsyncWebServer(80);
  wifiHelper = new WifiHelper();
  levels = new CircularBufferOfUnsignedLong(config->getUnsignedLong(SAMPLING_BUFFER_SIZE));
  times = new CircularBufferOfUnsignedLong(config->getUnsignedLong(SAMPLING_BUFFER_SIZE));
  
  Serial.print("ChipID: ");
  Serial.println(ESP.getChipId());
  Serial.print("FreeHeap: ");
  Serial.println(ESP.getFreeHeap());

  // display data files
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
  Serial.print("config = ");
  Serial.println(config->toJson());
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();

  Serial.print("Connect to wifi with ssid: ");
  Serial.print(config->getString(SSID));
  Serial.print(" and passphrase:  " );
  Serial.println(config->getString(SSID_PASSPHRASE));

  // IPAddress local_IP(192, 168, 2, 201);
  // IPAddress gateway(192, 168, 2, 1);
  // IPAddress subnet(255, 255, 0, 0);
  // WiFi.hostname("RPC-SOUND1");      // DHCP Hostname (useful for finding device for static lease)
  
  // if(!WiFi.config(local_IP,gateway,subnet))
  //   Serial.println("STA Failed to configure");
  
  // IPAddress soft_IP(192, 168, 4, 201);
  // if(!WiFi.softAPConfig(soft_IP,gateway,subnet))
    // Serial.println("AP Failed to configure");
  
  // WiFi.mode(WIFI_AP_STA);
  
  // begin access point
  // WiFi.softAP("rossypro.com", "");  

  // begin wifi
  WiFi.disconnect();
  WiFi.begin(config->getString(SSID),config->getString(SSID_PASSPHRASE));
  
  Serial.println("Wifi begin connection called");
  Serial.println(WiFi.localIP().toString());
  // digitalWrite(blue_led_pin,HIGH);
  
  // if (!wifiHelper->connectToWifi(LED_BLUE,config->getString(SSID),config->getString(SSID_PASSPHRASE))){
  //   wifiHelper->setupAccessPoint();
  //   enableSetupRoutes();
  // // } else {
  //   digitalWrite(LED_BLUE,LOW);
    // wifiHelper->wifiWebServerStart(LED_BLUE, config->getString(SSID),config->getString(SSID_PASSPHRASE));
  //   enableSensorRoutes();
  //     EmailHelper::sendEmail(
  //       config->getBoolean(CAN_SEND_EMAIL),
  //       config->getString(GMAIL),
  //       config->getString(GMAIL_PASSWORD),
  //       config->getString(SMS_ADDRESS),
  //       "Demarrage des capteurs sur le reseau " + config->getString(SSID) +  " et utilisation de l'adresse ip ",
  //       WiFi.localIP().toString()
  //     );

  // }
 
  enableSensorRoutes();

} 

// unsigned long last_handle = 0
unsigned long display_status_counter = 0;
bool access_point_running = false;
unsigned long secondes = 0;
unsigned long blue_led_frequency = 3000;
unsigned long config_request_count = 0;
unsigned long wifi_wait_for_connection =0;

void loop() {


  if(digitalRead(BUTTON_CONFIG) && !access_point_running){
    Serial.println("BUTTON_CONFIG clicked");
    digitalWrite(LED_GREEN,HIGH);
    if(config_request_count++ > 2){
      digitalWrite(LED_BLUE,HIGH);
      digitalWrite(LED_GREEN,LOW);
      blue_led_frequency = blue_led_frequency/4UL;
      Serial.println("Turning the AccessPoint On rossypro.com");
    
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
       WiFi.softAP("rossypro.com", "");
      access_point_running = true;
      enableSensorRoutes();
    } else {
      Serial.println("delay 500");
      delay(500);
    }

  } else {
    config_request_count = 0;
  }

  if ((WiFi.status() == WL_CONNECTED)) {
     digitalWrite(LED_BLUE,LOW);
     readSoundLevel();
  } else {
    if(millis()- wifi_wait_for_connection < blue_led_frequency/2UL)
      digitalWrite(LED_BLUE,HIGH);
    else
        digitalWrite(LED_BLUE,LOW);
    
    if(millis() - wifi_wait_for_connection > blue_led_frequency){
      wifi_wait_for_connection = millis();      
    }

  }

  if(millis() - display_status_counter > 10000){
      Serial.print(millis());
      Serial.print(" Wifi status: ");Serial.print(WiFi.status());
      Serial.print(" Local IP: "); Serial.print(WiFi.localIP());
      Serial.print("  SoftAP IP: "); Serial.println(WiFi.softAPIP());
      Serial.println(times->getChartTime());
      Serial.println(levels->getChartData());
      display_status_counter = millis();
  }
 
}
