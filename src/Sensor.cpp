#include "Sensor.h"
#include <pin.h>

Sensor::Sensor(unsigned long buffer_size){
  levels = new CircularBufferOfUnsignedLong(buffer_size);
  bufferSize = buffer_size;
}

unsigned long Sensor::readLastValue(unsigned long filterSize){
  unsigned long filter = min(filterSize, bufferSize);
  unsigned long value = levels->getLastInputFiltered(filter);
  return value;
}

unsigned long last_sensor_read = 0;
bool Sensor::read(unsigned long frequency){
  if(millis()-last_sensor_read > frequency) {
    last_sensor_read = millis();
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGGER, LOW);
    unsigned long duration = pulseIn(PIN_ECHO, HIGH);
    unsigned long distance = duration / 29 / 2;
    // Serial.print(distance);
    // Serial.println(" cm");
    levels->addData(distance);
    return true;
  }
  return false;
}