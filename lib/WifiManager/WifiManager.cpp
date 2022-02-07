#include "WifiManager.h"

WifiManager::WifiManager(){
}

bool WifiManager::startWifi(String ssid, String password){
  Serial.println("Start Wifi");
  digitalWrite(PIN_LED_BLUE,HIGH);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
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


boolean WifiManager::startAccessPoint(){
   Serial.println("Start Access point");
  WiFi.mode(WIFI_AP);
  bool result = WiFi.softAP("RossYpro.com", "12345678");
  if(result == true){
    Serial.print("SOFTAP Ready = ");
    Serial.println(WiFi.softAPIP());
    
  } else {
    Serial.println("SOFT Failed!");
  }
  return result;
}

//****************************************** */
//**** ESP-NOW CONFIGURATION */
static uint8_t PEER[]{0x44, 0x17, 0x93, 0x1e, 0x8e,0xf5};

// callback when data is sent
void printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,void* arg) {
  // Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],mac[4], mac[5]);
  for (int i = 0; i < static_cast<int>(count); ++i) {
    Serial.print(static_cast<char>(buf[i]));
  }
  Serial.println();
}

void WifiManager::startEspNow(){
  Serial.println("Start ESP-NOW network");
  _cansendDataToPeer =true;

  // Set device as a Wi-Fi Station
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
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
bool WifiManager::cansendDataToPeer(){
  return _cansendDataToPeer;
}

bool WifiManager::sendDataToPeer(uint8_t* peer,uint8_t* message){
  Serial.printf("send message to: %u of size %ul bytes", peer,sizeof(message)); // max 250 bytes
  bool ok = WifiEspNow.send(peer, reinterpret_cast<const uint8_t*>(&message), sizeof(message));
  return successfully_send(ok);
}



// bool WifiManager::sendDataToPeer(uint8_t* peer,MessageNow::sensorName_t message){
//   message.id = _message_number++;  
//   Serial.printf("send message to: %u of size %ul bytes", peer,sizeof(message)); // max 250 bytes
//   bool ok = WifiEspNow.send(peer, reinterpret_cast<const uint8_t*>(&message), sizeof(message));
//   return successfully_send(ok);
// }
// bool  WifiManager::sendDataToPeer(uint8_t* peer,MessageNow::measure_t message){
//  message.id = _message_number++;  
//   Serial.printf("send message to: %u of size %ul bytes", peer,sizeof(message)); // max 250 bytes
//   bool ok = WifiEspNow.send(peer, reinterpret_cast<const uint8_t*>(&message), sizeof(message));
//   return successfully_send(ok);

// }
// bool  WifiManager::sendDataToPeer(uint8_t* peer, MessageNow::alarm16x2_t message){
//  message.id = _message_number++;  
//   Serial.printf("send message to: %u of size %ul bytes", peer,sizeof(message)); // max 250 bytes
//   bool ok = WifiEspNow.send(peer, reinterpret_cast<const uint8_t*>(&message), sizeof(message));
//   return successfully_send(ok);

// }
// bool  WifiManager::sendDataToPeer(uint8_t* peer, MessageNow::ack_t message){
//  message.id = _message_number++;  
//   Serial.printf("send message to: %u of size %ul bytes", peer,sizeof(message)); // max 250 bytes
//   bool ok = WifiEspNow.send(peer, reinterpret_cast<const uint8_t*>(&message), sizeof(message));
//   return successfully_send(ok);
// }

bool WifiManager::successfully_send(bool ok){
if (ok) {
    Serial.println(" Sent with success");
    return true;
  }
  else {
    Serial.println(" Error sending the data");
    return false;
  }
}