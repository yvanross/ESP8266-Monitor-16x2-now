#include <DeviceController.h>
#include <DisplayUtil.h>

DeviceController::DeviceController(){
  config = new DeviceConfig();
  config->read(); 
  Serial.println("SENSOR read result");
  Serial.println(config->toJson());
  button = new Button(PIN_BUTTON_CONFIG,PIN_LED_BLUE);
  flashingLed = new FlashingLed(PIN_LED_BLUE);
  
  ota = new Ota();
  wifiManager = new WifiManager();  
}


bool DeviceController::startWifi(){
 bool started =  wifiManager->startWifi(
    config->getString(SSID),
    config->getString(SSID_PASSPHRASE));
    if(started){
        flashingLed->start(BLINK_WIFI);
        if(config->getBoolean(CAN_SEND_EMAIL) == true){
            EmailHelper::sendEmail(
                config->getString(GMAIL),
                config->getString(GMAIL_PASSWORD),
                config->getString(SMS_ADDRESS),
                "Demarrage du capteur: " + config->getString(SENSOR_NAME),
                " Adresse ip " + WiFi.localIP().toString());
          }
    } else {
      if(wifiManager->startAccessPoint()){
        started = true;
        flashingLed->start(BLINK_ACCESS_POINT);
      } else {
        flashingLed->stop();
        displayConfigInfo(config);
        config->toString();
      }
    }
    

   
  return started;
}



String DeviceController::getDataForIndex(){
  String data = String(config->getBoolean(INCHES)) + "|" +
    String(config->getUnsignedLong(SAMPLING_RATE_SECONDES)*1000) ;
  return data;
}


float DeviceController::getChartPoint(){
  return 0.0;
}

void DeviceController::loop(){
  if(button->pressed(flashingLed)){
    int count = button->pressCount();
    
    if(count == 1 ||count == 4 || count == 5 ){
      if(wifiManager->startWifi(
      config->getString(SSID),
      config->getString(SSID_PASSPHRASE)))
      flashingLed->start(BLINK_WIFI);
      else
        count = 2;

    }

    if(count == 2){
      if(wifiManager->startAccessPoint())
        flashingLed->start(BLINK_ACCESS_POINT);
    }
    if(count == 3){
      Serial.println("StartESPNow");
      wifiManager->startEspNow();
      flashingLed->start(BLINK_ESP_NOW);
    }

    if (count == 6){
      flashingLed->start(BLINK_OTA);
      ota->setup(
        config->getString(SSID),
        config->getString(SSID_PASSPHRASE));
    }

  }
  flashingLed->flash();
  ota->handle();
  
}

unsigned long DeviceController::getSamplingFrequency(){
    return config->getUnsignedLong(SAMPLING_RATE_SECONDES) * 1000;
}

String DeviceController::getConfigJson(){
  return config->toJson();
}

void DeviceController::readConfig(){
  config->read();
}

void DeviceController::updateConfig(String key, String value){
  config->update(key,value);
}

void DeviceController::writeConfig(){
  config->write();
}

void DeviceController::startAccessPoint(){
  wifiManager->startAccessPoint();
}

void DeviceController::startOta(){
  wifiManager->startAccessPoint();
}