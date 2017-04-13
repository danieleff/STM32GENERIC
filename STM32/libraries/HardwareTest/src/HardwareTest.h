#ifndef HARDWARE_API_TESTER_H
#define HARDWARE_API_TESTER_H

#include <Arduino.h>
#include <vector>

class HardwareTest {
  public:
  
    void setLedPin(uint16_t ledPin, boolean activeOnLow = false);
    void setLed(boolean on);
    void toggleLed();
    
    void begin();
    void end();

    int fails;
    int totals;
    uint8_t ledPin;
    boolean ledActiveLow;
    
};

class TestGroup {
  public:
    TestGroup(String name, HardwareTest *test): name(name), fails(0), totals(0), test(test)  {};

    void assert(boolean result, const char *message, ...);

    void begin();
    void end();

    int fails;
    int totals;
    String name;
    HardwareTest *test;
	String x;
};


#endif
