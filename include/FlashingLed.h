#ifndef FlashingLed_h
#define FlashingLed_h

class FlashingLed {
  public:
    FlashingLed(int led_pin);
    void start(int duration);
    void flash();
    void stop();

  private:
    int _duration;
    int _startAt;
    int _ledPin;
    int _state;
};

#endif