#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <EMailSender.h>
#include <EmailHelper.h>
#include <home.h>
#include <Lcd2.h>
#include <WifiHelper.h>
#include "pin.h"
#include "SensorConfig.h"
#include <CircularBufferOfFloat.h>


void handleHome();
void handlePing();
void handleConfigForm();
void handleScan();
void handleClean();
void handleReset();
void handleSetting();
void enableSensorRoutes();
void filldynamicdata();
// ********************************************
// configuration
// ********************************************
SensorConfig *config;
WifiHelper *wifiHelper;
ESP8266WebServer* server;
CircularBufferOfFloat* levels;
CircularBufferOfFloat* times;

Lcd2 lcd(0x27, 16, 2); // set the lcd i2c address
unsigned long last_read_sensor_time = 0;

void handleTest(){
  Serial.println("handleTest");
  int nbArg = server->args();
  String result = "<ul>";
  for(int i=0; i<nbArg; i++){
    result += "<li>" + server->argName(i) + "=>" + server->arg(i) + "</li>";
  } 
  result += "</ul>";
  // result += "<a href='http://192.168.2.236'>Go back</a>";
  
    String content = "<!DOCTYPE HTML><html>" + result + "</html>";
    server->send(200, "text/html", content);
}

void handleHome() {
  Serial.println("handlHome");
  Serial.println(millis());
  String s = HOME_page;
  s.replace(String("$$refresh_rate$$"), String(config->getSamplingRate()/1000));
  Serial.println(millis());
  s.replace(String("$$labels$$"), times->getChartTime());
  Serial.println(millis());
  s.replace(String("$$levels$$"), levels->getChartData());
  Serial.println(millis());
  s.replace(String("$$label1$$"), config->getSensorName() + " %");
  Serial.println(millis());
  server->send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}


void handleConfigForm(){
  Serial.println("handleConfigForm");
  Serial.println(millis());
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
  String content = "<!DOCTYPE HTML>\r\n<html>";
  String st = wifiHelper->getNetworkList();
  content += "<form action=\"/\" style=\"display:inline;\"><button>Page principale</button></form>";
  content += "<form action=\"/scan\" style=\"display:inline;\"><input type=\"submit\" value=\"scan\"></form>";
  content += "<form action=\"/reset\" style=\"display:inline;\"><input type=\"submit\" value=\"reset\"></form>";
  content += ipStr;
  content += "<p>" + st + "</p>";
  content += "<form method='get' action='setting'>";
  content += config->toHtmlForm();
  content += "<input type='submit'></form>";
  content += "</html>";
  Serial.println(millis());
  server->send(200, "text/html", content);
}

void handleScan(){
    wifiHelper->setupAccessPoint(lcd);
    String content = "<!DOCTYPE HTML>\r\n<html>go back";
    server->send(200, "text/html", content);
}


void handleClean(){
    Serial.println("Delete config.txt file and reset");
    config->remove();
    ESP.reset();
}

void handleReset(){
    Serial.println("reset ESP8266");
    ESP.reset();
}

void handleSetting(){
    if (config->write(server,Serial)) {
      Serial.println(config->toString());     
      handleConfigForm();
    } else {
      // Serial.println("ERROR: fail to write" + config->getMessage());
      String content = "{\"Error\":\"404 not found\"}";
      Serial.println("Sending 404");
      server->sendHeader("Access-Control-Allow-Origin", "*");
      server->send(404, "application/json", content);
    }
}

void handlePing(){
  Serial.println("handlePing");
  String content = "{\"name\":\"" + config->getSensorName() + "\"}";
  // server->sendHeader("Access-Control-Allow-Origin", "*");
  server->send(162, "text/json", content);
}

void enableSensorRoutes() {
  server->on(F("/"), HTTP_GET, handleHome);
  server->on(F("/config"), HTTP_GET, handleConfigForm);
  server->on("/scan",handleScan);
  server->on("/clean", handleClean);
  server->on("/reset", handleReset);
  server->on("/setting", handleSetting );
  server->on("/ping",handlePing);
  server->on ( "/filldynamicdata", filldynamicdata );
  server->onNotFound(handleNotFound);
  server->begin();
}
void enableSetupRoutes(){
  WiFi.softAP("rossypro.com", "");
  
  Serial.println("");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("rossypro.com");
  lcd.setCursor(0,1);
  lcd.println(WiFi.softAPIP());

  enableSensorRoutes();  // todo limit response pages.
  // server->on(F("/"), HTTP_GET, handleConfigForm);
  // server->on("/setting", handleSetting );
  // server->on("/clean", handleClean);
  // server->on("/reset", handleReset);
  // server->onNotFound(handleNotFound);
  // server->begin();
}
boolean detectRunningSound(){
  return levels->getLastInputFiltered() > config->getThreshold();
}

boolean state_running = false;
unsigned long count_running =0;
unsigned long count_stopped;

void readSoundLevel(){
  // Serial.print("Read sound level ");
   // Serial.println(sensorValue);
  int sensorValue = analogRead(A0);
  analogWrite(LED_WHITE,sensorValue); 
  levels->addData(sensorValue * config->getGain(),config->getFilterSize());
  times->addData(millis(),config->getFilterSize());

 
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
    String values ="Le seuil est ajusté à " + String(config->getThreshold());
    values += "mydynamicdata|" + (String) + "This is my Dynamic Value" + "|div\n";   // Build a string, like this:  ID|VALUE|TYPE
    server->send ( 200, "text/plain", values);   
}


void setup() {
  pinMode(LED_BLUE, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_WHITE,INPUT);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_WHITE,HIGH);
  analogWriteRange(255); //255 default
  // analogWriteFreq(new_frequency);
  
  Serial.begin(115200);
  Serial.println("Startup");
  delay(1000);
  config = new SensorConfig();
  config->readConfig();
  server = new ESP8266WebServer(80);
  wifiHelper = new WifiHelper(config);
  levels = new CircularBufferOfFloat(config->getSamplingBufferSize());
  times = new CircularBufferOfFloat(config->getSamplingBufferSize());
  

  
  lcd.start();
  
  if (!wifiHelper->connectToWifi(lcd)){
    wifiHelper->setupAccessPoint(lcd);
    enableSetupRoutes();
  } else {
    wifiHelper->wifiWebServerStart(lcd);
    enableSensorRoutes();
      EmailHelper::sendEmail(
        config->canSendEmail(),
        config->getGmail(),
        config->getGmailPassword(),
        config->getSmsAddress(),
        "Demarrage des capteurs sur le reseau " + config->getSsid() +  " et utilisation de l'adresse ip ",
        WiFi.localIP().toString()
      );

  }
}
// unsigned long last_handle = 0;
void loop() {
  
   if ((WiFi.status() == WL_CONNECTED)) {
     digitalWrite(LED_BLUE,LOW);
     readSoundLevel();
  } else {
    digitalWrite(LED_BLUE,HIGH);
    wifiHelper->waitConnectionOnAccessPoint();
  }
 
  server->handleClient();
  
}
