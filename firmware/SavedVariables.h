// SavedVariables.h
#ifndef SavedVariables_h
#define SavedVariables_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include <EEPROM.h>
#include "constants.h"


class SavedVariables {

 public:

  SavedVariables();
  int getSerialNumber();
  bool setSerialNumber(int serial_number);

 private:

  enum {eepromAddressInitialized = 0};
  enum {eepromAddressSerialNumber = 1};

  enum {initializedValue = 123};

  int _serialNumber;

  void initialize();

};

extern SavedVariables savedVariables;

#endif
