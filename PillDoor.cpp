/*
 Opens or closes a device pill dispensing door.

 Code inspiration from Sweeper by BARRAGAN
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include "PillDoor.h"
#include "Arduino.h"
#include <Servo.h>

PillDoor::PillDoor(int duration) {
  this->updateInterval = duration / 180;
}

void PillDoor::attach(int pin) {
  this->servo.attach(pin);
  this->servo.write(0); // avoid startup glitch movement
}

void PillDoor::dispense() {
  dispensing = true;
}

// to be called every cycle regardless of currently dispensing or not
void PillDoor::update() {
  if (dispensing) {
    unsigned long ms = millis();
    if ((ms - lastUpdate) > updateInterval) {
      lastUpdate = ms;
      pos += increment;
      servo.write(pos);
      if ((pos >= 180) || (pos <= 0)) {
        increment = -increment;
        dispensing = false;
      }
    }
  }
}

bool PillDoor::isDispensing() {
  return dispensing;
}

