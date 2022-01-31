#ifndef MyButton_h
#define MyButton_h
#include "FlashingLed.h"

class Button {
  public:
    Button(int pin, int led);
    bool pressed(FlashingLed* led);
    int pressCount();  

  private:
    void startPress(FlashingLed* led);
    unsigned long endPress();
    void flashLongPressReached();

    int _pin;
    int _led;
    int _pressAt = 0;
    int _longPressDelay = 1000;  // 3sec
    int _pressCount = 0;
    
};

#endif