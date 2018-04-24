/*
 * Controls an LED, either via PWM (with setBrightness) or digital on/off.
 */

#include "LED.h"
#include "Arduino.h"

LED::LED(int pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LED::turn(bool on) {
  digitalWrite(pin, on ? HIGH : LOW);
}
