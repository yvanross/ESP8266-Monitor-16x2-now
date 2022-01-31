#include "FlashingLed.h"
#include <Arduino.h>

FlashingLed::FlashingLed(int led_pin){
  _ledPin = led_pin;
  pinMode(led_pin,OUTPUT);
}

void FlashingLed::start(int duration){
  // Serial.println("FlashingLed start");
  _duration = duration;
  _startAt = millis();
  _state = HIGH;
  digitalWrite(_ledPin,_state);
  
}

void FlashingLed::flash(){

  if( _startAt != 0 && (millis() - _startAt > _duration)) {
    _startAt = millis();
    _state = _state == HIGH ? LOW : HIGH;
    digitalWrite(_ledPin,_state);
  }
}

void FlashingLed::stop(){
  // Serial.println("FlashingLed stop");
  _startAt = 0;
  digitalWrite(_ledPin,LOW);
}