#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

namespace constants {
  enum {relayCount=8};
  enum {digitalOutputCount=0};
  enum {digitalInputCount=0};
  enum {analogInputCount=1};

  // Communications parameters
  extern const int baudrate;

  // Device parameters
  extern const int modelNumber;
  extern const int serialNumberMin;
  extern const int serialNumberMax;
  extern const int firmwareNumber;

  // Pin assignment
  extern const int relayDriverCsPin;
  extern const int relayDriverInPin;
  extern const int digitalOutputPins[digitalOutputCount];
  extern const int digitalInputPins[digitalInputCount];
  extern const int analogInputPins[analogInputCount];
  extern const int analogInputPinAliases[analogInputCount];
  extern const int puffInputPin;
  extern const int puffOutputPin;

  // Scheduler parameters
  extern const int clockTicksPerSecond;

  // Blink parameters
  extern const int dutyCycleMin;
  extern const int dutyCycleMax;

  // Analog input parameters
  extern const int analogValueMin;
  extern const int analogValueMax;

  // Analog input map
  extern const int mapPuffDurationIndex;
  extern const int mapPuffDurationEnabled;
  extern const int mapPeriod;

  // Display parameters
  extern const int displayPeriod;

  // Puff parameters
  extern const int puffDurationMin;
  extern const int puffDurationMax;
  extern const int puffRelay;

}
#endif
