#include <Arduino.h>
#include "pin.h"
#include <Router.h>

Router *router;

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

// TODO authentification
// TODO Démarrage selon le mode sélectionner dans la configuration
// TODO OTA avec password 
void setup() {
  initPin();
  initSerial();
  router = new Router();
  router->startWifi();
} 

void loop() {
  router->loop();
}

