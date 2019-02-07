#ifndef _MESSAGE_HANDER_H_
#define _MESSAGE_HANDER_H_
#include <SerialReceiver.h>
#include "DictPrinter.h"
#include "constants.h"
#include "SavedVariables.h"

class MessageHandler : public SerialReceiver {

 public:
  void processMsg();

 private:
  DictPrinter dprint;
  void msgSwitchYard();
  void handleGetDevInfo();
  void handleGetCmds();

  bool checkNumberOfArgs(int num);
  bool checkSerialNumberArg(int serial_number);
  void systemCmdRsp(bool flag);

  void handleGetRspCodes();
  void handleSetSerialNumber();

  bool checkRelayArg(int relay);
  bool checkPeriodArg(uint32_t period);
  bool checkDutyCycleArg(int duty_cycle);
  bool checkDigitalOutputArg(int digital_output);
  bool checkDigitalInputArg(int digital_input);
  bool checkAnalogInputArg(int analog_input);

  void handleSetRelays();
  void handleSetRelayOn();
  void handleSetRelayOff();
  void handleSetAllRelaysOn();
  void handleSetAllRelaysOff();

  void handleStartRelayBlink();
  void handleStopRelayBlink();
  void handleStopAllRelaysBlink();

  void handleAddPulseCentered();

  void handleSetDigitalOutputHigh();
  void handleSetDigitalOutputLow();
  void handleToggleDigitalOutput();
  void handlePulseDigitalOutput();

  void handleGetDigitalInput();

  void handleRemoveAllTasks();

  void handleGetTime();
  void handleSetTime();

  void handleGetTaskDetails();

  // Development
  void handleDebug();
};

extern MessageHandler messageHandler;
#endif
