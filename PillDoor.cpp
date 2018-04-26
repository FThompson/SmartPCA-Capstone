/*
 * Opens or closes a device pill dispensing door.
 *
 * Code inspiration from Sweeper by BARRAGAN
 * http://www.arduino.cc/en/Tutorial/Sweep
 */

#include "PillDoor.h"
#include "Arduino.h"
#include <Servo.h>

PillDoor::PillDoor(int pin, int duration, int startPos, int endPos) {
  this->pin = pin;
  this->startPos = startPos;
  this->endPos = endPos;
  int change = endPos - startPos;
  positiveChange = (change > 0);
  updateInterval = duration / abs(change);
  if (positiveChange) {
    increment = 1;
  } else {
    increment = -1;
  }
}

void PillDoor::attach() {
  servo.attach(pin);
  pos = startPos;
  servo.write(startPos);
}

void PillDoor::dispense() {
  if (!dispensing) {
    dispensing = true;
    resetting = false;
  }
}

// to be called every cycle regardless of currently dispensing or not
void PillDoor::update() {
  if (dispensing) {
    unsigned long ms = millis();
    if ((ms - lastUpdate) > updateInterval) {
      lastUpdate = ms;
      pos += increment;
      servo.write(pos);
      if (resetting) {
        if ((positiveChange && pos <= startPos) || (!positiveChange && pos >= startPos)) {
          increment = -increment;
          dispensing = false;
        }
      } else {
        if ((positiveChange && pos >= endPos) || (!positiveChange && pos <= endPos)) {
          increment = -increment;
          resetting = true;
        }
      }
    }
  }
}

bool PillDoor::isDispensing() {
  return dispensing;
}

