/*
 * Opens or closes a device pill dispensing door.
 *
 * Code inspiration from Sweeper by BARRAGAN
 * http://www.arduino.cc/en/Tutorial/Sweep
 */

#include "PillDoor.h"
#include "Arduino.h"
#include <Servo.h>

PillDoor::PillDoor(int pin, int duration) {
  this->pin = pin;
  this->updateInterval = duration / 180;
}

void PillDoor::dispense() {
  dispensing = true;
  Serial.print("attaching on pin ");
  Serial.println(pin);
  this->servo.attach(pin);
}

// to be called every cycle regardless of currently dispensing or not
void PillDoor::update() {
  if (dispensing) {
    Serial.println("dispensing");
    unsigned long ms = millis();
    if ((ms - lastUpdate) > updateInterval) {
      lastUpdate = ms;
      pos += increment;
      servo.write(pos);
      if ((pos >= 180) || (pos <= 0)) {
        increment = -increment;
        dispensing = false;
        servo.detach();
      }
    }
  }
}

bool PillDoor::isDispensing() {
  return dispensing;
}

