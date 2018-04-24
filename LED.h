/*
 * Controls an LED, either via PWM (with setBrightness) or digital on/off.
 */

#ifndef LED_h
#define LED_h

class LED {
protected:
  int pin;

public:
  LED(int pin);
  
  void turn(bool on);
};

#endif
