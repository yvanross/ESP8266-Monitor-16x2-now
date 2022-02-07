#include "Router.h"

DeviceController *deviceController; 

Router::Router(){
   server = new AsyncWebServer(80);
   deviceController = new DeviceController();
 
}
void Router::startWifi(){
  if(deviceController->startWifi())
    enableRoutes();
}

void Router::loop(){
  deviceController->loop();
}

void Router::enableRoutes() {
  
  if(!LittleFS.begin()){
      Serial.println("ERROR:An Error has occurred while mounting LittleFS");
      return;
  }

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });
  
  // server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(LittleFS, "/wifimanager.html");
  // });

  server->on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isPost()){
      // HTTP POST ssid value
      // if (p->name() == PARAM_INPUT_1) {
      //   ssid = p->value().c_str();
      //   Serial.print("SSID set to: ");
      //   Serial.println(ssid);
      //   // Write file to save value
      //   writeFile(SPIFFS, ssidPath, ssid.c_str());
      // }
      // // HTTP POST pass value
      // if (p->name() == PARAM_INPUT_2) {
      //   pass = p->value().c_str();
      //   Serial.print("Password set to: ");
      //   Serial.println(pass);
      //   // Write file to save value
      //   writeFile(SPIFFS, passPath, pass.c_str());
      // }
      // // HTTP POST ip value
      // if (p->name() == PARAM_INPUT_3) {
      //   ip = p->value().c_str();
      //   Serial.print("IP Address set to: ");
      //   Serial.println(ip);
      //   // Write file to save value
      //   writeFile(SPIFFS, ipPath, ip.c_str());
      // }
      //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
     }
    }
  });


  server->on("/sampling_frequency", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",String(deviceController->getSamplingFrequency()).c_str());
  });

  server->on("/index_data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", deviceController->getDataForIndex().c_str());
  });

   server->on("/level", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",String(deviceController->getChartPoint()).c_str());
  });

  server->on("/config_data", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("/config_data called");
    request->send_P(200, "text/plain",deviceController->getConfigJson().c_str());
  });
  
  server->on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("/Settings called");
    deviceController->readConfig();

    for(unsigned long i = 0; i < request->args(); i++){
      if(request->arg(i).length() > 0)
        deviceController->updateConfig(request->argName(i),request->arg(i));
    }
    deviceController->writeConfig();
    
    deviceController->startSensor();
    Serial.println("send /index.html");
    request->send(LittleFS, "/index.html");

  });
  
  server->on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("reset ESP8266 will restart sensor");
    ESP.restart();
  });

  
  server->on("/accesspoint", HTTP_GET, [](AsyncWebServerRequest *request){
    deviceController->startAccessPoint();
  });

  server->on("/ota", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("/ota");
    deviceController->startOta();
    request->send(LittleFS, "/index.html");
  });

  server->serveStatic("/favicon.ico",LittleFS,"favicon.ico","max-age=1");
  server->serveStatic("/style.css",LittleFS,"style.css","max-age=1");
  server->serveStatic("/microajax.min.js",LittleFS,"microajax.js", "max-age=1");
  server->serveStatic("/highcharts.js",LittleFS,"highcharts.js", "max-age=1");
  server->serveStatic("/chart.min.js",LittleFS,"chart.min.js", "max-age=1");
  server->serveStatic("/index",LittleFS,"index.html", "max-age=1");
  server->serveStatic("/config",LittleFS,"config.html", "max-age=1");
  server->serveStatic("/chart",LittleFS,"chart.html", "max-age=1");
   
  server->begin();
}
