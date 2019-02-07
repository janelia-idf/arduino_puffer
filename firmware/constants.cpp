#include "constants.h"
namespace constants {
  // Communications parameters
  const int baudrate = 9600;

  // Device parameters
  const int modelNumber = 2275;
  const int serialNumberMin = 0;
  const int serialNumberMax = 255;
  const int firmwareNumber = 1;

  // Pin assignment
  const int relayDriverCsPin = 49;
  const int relayDriverInPin = 48;
  const int digitalOutputPins[digitalOutputCount] = {};
  const int digitalInputPins[digitalInputCount] = {};
  const int analogInputPins[analogInputCount] = {2};
  const int analogInputPinAliases[analogInputCount] = {A2};
  const int puffInputPin = A3;
  const int puffOutputPin = A0;

  // Timestamp parameters
  const int clockTicksPerSecond = 1000;

  // Blink parameters
  const int dutyCycleMin = 0;
  const int dutyCycleMax = 100;

  // Analog input parameters
  const int analogValueMin = 0;
  const int analogValueMax = 1023;

  // Analog input map
  const int mapPuffDurationIndex = 0;
  const int mapRelayOnEnabled = true;
  const int mapPeriod = 250;

  // Display
  const int displayPeriod = 250;

  // Puff parameters
  const int puffDurationMin = 25;
  const int puffDurationMax = 2000;
  const int puffRelay = 0;

}
