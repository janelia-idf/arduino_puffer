#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_
#include <SPI.h>
#include <util/atomic.h>
#include <Tle7232g.h>
#include "TimeTriggeredScheduler.h"
#include "constants.h"
#include "NewhavenDisplay.h"

enum {SYS_ERR_BUF_SZ=50};

class SystemState {

 public:
  SystemState();

  void setErrMsg(char *);
  char errMsg[SYS_ERR_BUF_SZ];

  void initialize();

  bool setRelays(uint32_t relays);
  bool setRelayOn(int relay);
  bool setRelayOff(int relay);
  bool setAllRelaysOn();
  bool setAllRelaysOff();

  bool startRelayBlink(int relay, long period, int duty_cycle, long count);
  bool stopRelayBlink(int relay);
  bool stopAllRelaysBlink();

  bool addPulseCentered(int relay, long delay, long duration);

  bool setDigitalOutputHigh(int digital_output);
  bool setDigitalOutputLow(int digital_output);
  bool toggleDigitalOutput(int digital_output);
  bool pulseDigitalOutput(int digital_output, long duration);

  int getDigitalInput(int digital_input);
  int getAnalogInput(int analog_input);

  bool removeAllTasks();

  uint32_t getTime();
  bool setTime(uint32_t time);

  /* void update(); */
  bool getTaskDetails(uint8_t taskId, uint32_t& time, int& arg, long& period, long& count, bool& free, bool& enabled);

  bool setPuffOn(int relay);
  bool setPuffOff(int relay);
  void monitorPuffInput();
  bool mapAnalogInputToPuffDuration();

  bool mapAnalogInputs(int dummy_arg);
  bool startAnalogInputsMapping();
  bool stopAnalogInputsMapping();

  void initializeDisplay();
  void updateDisplay();
  void enableDisplay();
  void disableDisplay();


 private:
  TLE7232G relayDriver;
  struct blinkTasks_t {
    int onTaskId;
    int offTaskId;
  };
  blinkTasks_t relayBlinkTasks[constants::relayCount];
  int digitalOutputStates[constants::digitalOutputCount];

  NewhavenDisplay display;
  bool puffing;
  bool puffSwitchHeldDown;
  int puffDuration;
  int mapAnalogInputsTaskId;
  bool mappingEnabled;
  bool displayEnabled;
  unsigned long time_last_display_update;

};

extern SystemState systemState;

inline void inlineSetRelayOn(int relay) {systemState.setRelayOn(relay);}
inline void inlineSetRelayOff(int relay) {systemState.setRelayOff(relay);}
inline void inlineSetDigitalOutputHigh(int digital_output) {systemState.setDigitalOutputHigh(digital_output);}
inline void inlineSetDigitalOutputLow(int digital_output) {systemState.setDigitalOutputLow(digital_output);}
inline void inlineSetPuffOn(int relay) {systemState.setPuffOn(relay);}
inline void inlineSetPuffOff(int relay) {systemState.setPuffOff(relay);}
inline void inlineMapAnalogInputs(int dummy_arg) {systemState.mapAnalogInputs(dummy_arg);}

#endif
