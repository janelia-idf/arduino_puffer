#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "string.h"
#include "SystemState.h"

SystemState::SystemState() {
  setErrMsg("");
}

void SystemState::setErrMsg(char *msg) {
  strncpy(errMsg,msg,SYS_ERR_BUF_SZ);
}

void SystemState::initialize() {
  relayDriver = TLE7232G(constants::relayDriverCsPin,constants::relayDriverInPin);

  // Initialize SPI:
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  relayDriver.init(1);
  setAllRelaysOff();

  tts.init();

  for (int relay; relay < constants::relayCount; relay++) {
    relayBlinkTasks[relay].onTaskId = -1;
    relayBlinkTasks[relay].offTaskId = -1;
  }

  for (int digital_output; digital_output < constants::digitalOutputCount; digital_output++) {
    pinMode(constants::digitalOutputPins[digital_output], OUTPUT);
    digitalWrite(constants::digitalOutputPins[digital_output], LOW);
    digitalOutputStates[digital_output] = LOW;
  }

  for (int digital_input; digital_input < constants::digitalInputCount; digital_input++) {
    pinMode(constants::digitalInputPins[digital_input], INPUT);
    digitalWrite(constants::digitalInputPins[digital_input], HIGH);
  }

  pinMode(constants::puffInputPin, INPUT);
  digitalWrite(constants::puffInputPin, HIGH);

  pinMode(constants::puffOutputPin, OUTPUT);
  digitalWrite(constants::puffOutputPin, LOW);

  puffing = false;
  puffSwitchHeldDown = false;
  puffDuration = (constants::puffDurationMin + constants::puffDurationMax)/2;

  startAnalogInputsMapping();

  // Display
  display = NewhavenDisplay();
  initializeDisplay();

}

bool SystemState::setRelays(uint32_t relays) {
  relayDriver.setChannels(relays);
  return true;
}

bool SystemState::setRelayOn(int relay) {
  relayDriver.setChannelOn(relay);
  return true;
}

bool SystemState::setRelayOff(int relay) {
  relayDriver.setChannelOff(relay);
  return true;
}

bool SystemState::setAllRelaysOn() {
  uint32_t relays = 0;
  relays = ~relays;
  setRelays(relays);
  return true;
}

bool SystemState::setAllRelaysOff() {
  uint32_t relays = 0;
  setRelays(relays);
  return true;
}

bool SystemState::startRelayBlink(int relay, long period, int duty_cycle, long count=-1) {
  stopRelayBlink(relay);
  if ((0 < duty_cycle) && (duty_cycle < 100)) {
    int onTaskId = (int)tts.addTaskUsingDelay(100,inlineSetRelayOn,relay,period,count,false);
    int offTaskId = (int)tts.addTaskUsingOffset((uint8_t)onTaskId,(period*duty_cycle)/100,inlineSetRelayOff,relay,period,count,false);

    // Only save continuous blink patterns. Let patterns with count
    // expire naturally. This means multiple blink patterns could
    // overlap until count expires and that finite blink patterns
    // cannot be stopped unless all tasks are removed.

    if (count < 0) {
      relayBlinkTasks[relay].onTaskId = onTaskId;
      relayBlinkTasks[relay].offTaskId = offTaskId;
    }
  } else if (100 <= duty_cycle) {
    setRelayOn(relay);
  }
  return true;
}

bool SystemState::stopRelayBlink(int relay) {
  int onTaskId = relayBlinkTasks[relay].onTaskId;
  int offTaskId = relayBlinkTasks[relay].offTaskId;
  if (0 <= onTaskId) {
    tts.removeTask((uint8_t)onTaskId);
    relayBlinkTasks[relay].onTaskId = -1;
  }
  if (0 <= offTaskId) {
    tts.removeTask((uint8_t)offTaskId);
    relayBlinkTasks[relay].offTaskId = -1;
  }
  setRelayOff(relay);
  return true;
}

bool SystemState::stopAllRelaysBlink() {
  for (uint8_t relay; relay < constants::relayCount; relay++) {
    stopRelayBlink(relay);
  }
  return true;
}

bool SystemState::addPulseCentered(int relay, long delay, long duration) {
  long rising = delay - duration/2;
  if (rising < 0) {
    rising = 0;
  }
  int onTaskId = (int)tts.addTaskUsingDelay(rising,inlineSetRelayOn,relay,-1,-1,false);
  tts.addTaskUsingOffset((uint8_t)onTaskId,duration,inlineSetRelayOff,relay,-1,-1,false);
  return true;
}

bool SystemState::setDigitalOutputHigh(int digital_output) {
  digitalWrite(constants::digitalOutputPins[digital_output], HIGH);
  digitalOutputStates[digital_output] = HIGH;
  return true;
}

bool SystemState::setDigitalOutputLow(int digital_output) {
  digitalWrite(constants::digitalOutputPins[digital_output], LOW);
  digitalOutputStates[digital_output] = LOW;
  return true;
}

bool SystemState::toggleDigitalOutput(int digital_output) {
  if (digitalOutputStates[digital_output] == LOW) {
    setDigitalOutputHigh(digital_output);
  } else {
    setDigitalOutputLow(digital_output);
  }
  return true;
}

bool SystemState::pulseDigitalOutput(int digital_output, long duration) {
  int onTaskId = (int)tts.addTaskUsingDelay(0,inlineSetDigitalOutputHigh,digital_output,-1,-1,false);
  tts.addTaskUsingOffset((uint8_t)onTaskId,duration,inlineSetDigitalOutputLow,digital_output,-1,-1,false);
  return true;
}

int SystemState::getDigitalInput(int digital_input) {
  return digitalRead(constants::digitalInputPins[digital_input]);
}

int SystemState::getAnalogInput(int analog_input) {
  return analogRead(constants::analogInputPins[analog_input]);
}

bool SystemState::removeAllTasks() {
  return tts.removeAllTasks();
}

uint32_t SystemState::getTime() {
  return tts.ms();
}

bool SystemState::setTime(uint32_t time=0) {
  tts.setTime(time);
  return true;
}

bool SystemState::getTaskDetails(uint8_t taskId, uint32_t& time, int& arg, long& period, long& count, bool& free, bool& enabled) {
  tts.getTaskDetails(taskId, time, arg, period, count, free, enabled);
  return true;
}

bool SystemState::setPuffOn(int relay) {
  setRelayOn(relay);
  digitalWrite(constants::puffOutputPin, HIGH);
  return true;
}

bool SystemState::setPuffOff(int relay) {
  setRelayOff(relay);
  digitalWrite(constants::puffOutputPin, LOW);
  puffing = false;
  return true;
}

void SystemState::monitorPuffInput() {
  if (!puffing && !puffSwitchHeldDown) {
    if (digitalRead(constants::puffInputPin) == LOW) {
      puffing = true;
      puffSwitchHeldDown = true;
      int onTaskId = (int)tts.addTaskUsingDelay(0,inlineSetPuffOn,constants::puffRelay,-1,-1,false);
      tts.addTaskUsingOffset((uint8_t)onTaskId,puffDuration,inlineSetPuffOff,constants::puffRelay,-1,-1,false);
    }
  } else if (!puffing && (digitalRead(constants::puffInputPin) == HIGH)) {
    puffSwitchHeldDown = false;
  }
}

bool SystemState::mapAnalogInputToPuffDuration() {
  int analogValue = getAnalogInput(constants::mapPuffDurationIndex);
  puffDuration = map(analogValue,
                     constants::analogValueMin,
                     constants::analogValueMax,
                     constants::puffDurationMin,
                     constants::puffDurationMax);
  return true;
}

bool SystemState::mapAnalogInputs(int dummy_arg) {
  mapAnalogInputToPuffDuration();
  return true;
}

bool SystemState::startAnalogInputsMapping() {
  this->mapAnalogInputsTaskId = (int)tts.addTaskUsingDelay(1000,
                                                           inlineMapAnalogInputs,
                                                           0,
                                                           constants::mapPeriod,
                                                           -1,
                                                           false);
  this->mappingEnabled = true;
  return true;
}

bool SystemState::stopAnalogInputsMapping() {
  if (mappingEnabled) {
    tts.removeTask((uint8_t)mapAnalogInputsTaskId);
    mappingEnabled = false;
  }
  return true;
}

void SystemState::updateDisplay() {
  if (displayEnabled) {
    unsigned long time_now = millis();
    if ((time_now - time_last_display_update) > constants::displayPeriod) {
      time_last_display_update = time_now;

      display.setCursor(3,0);
      display.print("                    ");
      display.setCursor(3,8);
      display.print(String(puffDuration));
    }
  }
}

void SystemState::initializeDisplay() {
  display.init();
  display.clearScreen();
  delay(100);
  display.print("Arduino Puffer");
  display.setCursor(2,0);
  display.print("Puff Duration (ms):");
  time_last_display_update = millis();
  displayEnabled = true;
}

SystemState systemState;


