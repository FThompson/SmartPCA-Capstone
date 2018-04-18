/*
 Controls an LED, either via PWM (with setBrightness) or digital on/off.
*/

#ifndef LED_h
#define LED_h

class LED {
  int pin;

public:
  LED(int pin);
  
  void setBrightness(int brightness); // brightness 0-256
  void turnOn();
  void turnOff();
};

#endif
