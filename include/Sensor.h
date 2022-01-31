#ifndef Sensor_h
#define Sensor_h

#include <CircularBufferOfUnsignedLong.h>

class Sensor {
public:
  Sensor(unsigned long buffer_size);
  unsigned long readLastValue(unsigned long filterSize);
  bool read(unsigned long frequency);

private:
  CircularBufferOfUnsignedLong* levels;
  unsigned long bufferSize;
};

#endif