#include <Arduino.h>
#include "Button.h"

Button::Button(int pin,int led){
  _pin = pin;
  _led = led;
  pinMode(pin,INPUT_PULLDOWN_16);
}


bool Button::pressed(FlashingLed* led){
  startPress(led);
  flashLongPressReached();  
  if(endPress() == 0)
    return false;

  Serial.println("return pressed == true");
  return true;
}

int Button::pressCount(){
  // Serial.print("PressCount = ");
  // Serial.println(_pressCount);
  int status = _pressCount;
  _pressCount = 0;
  return status;

}

void Button::flashLongPressReached(){
  if(_pressAt != 0 && millis() - _pressAt > _longPressDelay){
    digitalWrite(_led,LOW);
     delay(500);
     digitalWrite(_led,HIGH);
    _pressCount++;
    // Serial.printf("flashLongPressReached = %ld\n",_pressCount);
    _pressAt = millis();
  }
}

void Button::startPress(FlashingLed *flashingLed){
  if(digitalRead(_pin) && _pressAt == 0){
    flashingLed->stop();
    // Serial.println("StartPress");
    digitalWrite(_led,HIGH);
    _pressAt = millis();
    _pressCount = 1;
  }
}
unsigned long Button::endPress(){
  if(!digitalRead(_pin) && _pressAt != 0){
    // Serial.println("endPress");
    digitalWrite(_led,LOW);
    unsigned long holdTime = millis() - _pressAt;
    _pressAt = 0;
    return holdTime;
  }
  return 0;
}
