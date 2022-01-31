#include "WifiHelper.h"


SensorConfig* configPtr;
Sensor* sensorPtr;

WifiHelper::WifiHelper(SensorConfig* inConfig,Sensor *inSensor){
  server = new AsyncWebServer(80);
  configPtr = inConfig;
  sensorPtr = inSensor;
}

bool WifiHelper::startWifi(SensorConfig* config){
  Serial.println("Start Wifi");
  digitalWrite(PIN_LED_BLUE,HIGH);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(configPtr->getString(SSID),configPtr->getString(SSID_PASSPHRASE));
  unsigned long current_time = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - current_time < 30000) {
    Serial.print(".");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.print("Wifi server started at: "); 
    Serial.println(WiFi.localIP());
    return true;
  } else
    return false;
}


boolean WifiHelper::startAccessPoint(){
   Serial.println("Start Access point");
  WiFi.mode(WIFI_AP);
  bool result = WiFi.softAP("rossypro.com", "12345678");
  if(result == true){
    Serial.print("SOFTAP Ready = ");
    Serial.println(WiFi.softAPIP());
    
  } else {
    Serial.println("SOFT Failed!");
  }
  return result;
}


void WifiHelper::startAccessPointWithLed(FlashingLed* flashingLed){
  if(startAccessPoint()){
    flashingLed->start(200);
    enableSensorRoutes();
  } else
    flashingLed->stop();
}

void WifiHelper::startWifiWithLed(SensorConfig* config,FlashingLed* flashingLed){
  if(startWifi(config)){
    flashingLed->start(1000);
    enableSensorRoutes();
  } else {
    startAccessPointWithLed(flashingLed);
  }
}



void WifiHelper::enableSensorRoutes() {
  Serial.println("enableSensorRoutes");
  if(!LittleFS.begin()){
      Serial.println("ERROR:An Error has occurred while mounting LittleFS");
      // return;
  }

  // async request
  server->on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("test verify config");
    String ssid = configPtr->getString(SSID);

     request->send(LittleFS, "/index.html");
  });

  server->on("/clean", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Delete config.txt file and reset");
    configPtr->remove();
    request->redirect("192.168.2.237");
    ESP.reset();
  });

   server->on("/level", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",String(sensorPtr->readLastValue(10)).c_str());
  });

  server->on("/config_data", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("/config_data called");
    request->send_P(200, "text/plain",configPtr->toJson().c_str());
  });
  
  server->on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Settings called");
    configPtr->read();

    for(unsigned long i = 0; i < request->args(); i++){
      if(request->arg(i).length() > 0)
        configPtr->update(request->argName(i),request->arg(i));
    }
    configPtr->write();
    request->send(LittleFS, "/config.html");
  });
  
  server->on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("reset ESP8266");
    ESP.reset();
  });

  // JSON format response
  server->on("/ping", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Ping");
    String content = "{\"name\":\"" + configPtr->getString(SENSOR_NAME) + "\"}";
    // request->sendHeader("Access-Control-Allow-Origin", "*");
    request->send_P(162, "text/json", content.c_str());
  });

  server->serveStatic("/favicon.ico",LittleFS,"favicon.ico","max-age=43200");
  server->serveStatic("/style.css",LittleFS,"style.css","max-age=43200");
  server->serveStatic("/microajax.min.js",LittleFS,"microajax.js", "max-age=43200");
  server->serveStatic("/highcharts.js",LittleFS,"highcharts.js", "max-age=43200");
  server->serveStatic("/chart.min.js",LittleFS,"chart.min.js", "max-age=43200");
  server->serveStatic("/",LittleFS,"index.html", "max-age=43200");
  server->serveStatic("/index",LittleFS,"index.html", "max-age=43200");
  server->serveStatic("/config",LittleFS,"config.html", "max-age=43200");
  server->serveStatic("/chart",LittleFS,"chart.html", "max-age=43200");
   
  server->begin();
}

//************************************************************************************************************************************** */
//**** ESP-NOW CONFIGURATION */
static uint8_t PEER[]{0x46, 0x17, 0x93, 0x0F, 0xDD, 0x31};
WifiHelper* helper;
typedef struct struct_message {
    int id; // must be unique for each sender board
    unsigned long  distance;
    char sensorName[30];

} struct_message;
struct_message myData;


// callback when data is sent
void printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* incomingData, size_t count,void* arg) {
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],mac[4], mac[5]);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("id = ");Serial.println(myData.id);
  Serial.print("Distance = ");Serial.println(myData.distance);
  Serial.print("SensorName = ");Serial.println(myData.sensorName);

  // for (int i = 0; i < static_cast<int>(count); ++i) {
  //   Serial.print(static_cast<char>(buf[i]));
  // }
  // Serial.println();
  // helper->sendDataToSlave(99);
}

// // callback when data is sent
// void printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,void* arg) {
//   Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],mac[4], mac[5]);
//   for (int i = 0; i < static_cast<int>(count); ++i) {
//     Serial.print(static_cast<char>(buf[i]));
//   }
//   Serial.println();
//   // helper->sendDataToSlave(99);
// }


void WifiHelper::startEspNow(){
  _canSendDataToSlave =true;
  helper = this;
  // Set device as a Wi-Fi Station
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  // WiFi must be powered on to use ESP-NOW unicast.
  // It could be either AP or STA mode, and does not have to be connected.
  // For best results, ensure both devices are using the same WiFi channel.
  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());

  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  Serial.println();
  Serial.println("You can paste the following into the program for the other device:");
  Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }

}
bool WifiHelper::canSendDataToSlave(){
  return _canSendDataToSlave;
}

bool WifiHelper::sendDataToSlave(unsigned long distance){
 
  char msg[60];
  int len = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu",WiFi.softAPmacAddress().c_str(), distance);
  bool ok = WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);

  if (ok) {
    Serial.println("Sent with success");
    return true;
  }
  else {
    Serial.println("Error sending the data");
    return false;
  }
}