#include "SavedVariables.h"

SavedVariables::SavedVariables()
{
  initialize();
};

void SavedVariables::initialize() {
  // Check to see if EEPROM values have been initialized for the very first time, if not, initialize
  if (EEPROM.read(eepromAddressInitialized) != initializedValue) {
    EEPROM.write(eepromAddressInitialized,initializedValue);
    EEPROM.write(eepromAddressSerialNumber,constants::serialNumberMin);
  }

  this->_serialNumber = EEPROM.read(eepromAddressSerialNumber);
}

int SavedVariables::getSerialNumber() {
  return _serialNumber;
}

bool SavedVariables::setSerialNumber(int serial_number) {
  this->_serialNumber = serial_number;
  EEPROM.write(eepromAddressSerialNumber,serial_number);
  return true;
}


SavedVariables savedVariables;
