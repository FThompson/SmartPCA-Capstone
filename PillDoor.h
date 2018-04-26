/*
 * Opens or closes a device pill dispensing door.
 *
 * Code inspiration from Sweeper by BARRAGAN
 * http://www.arduino.cc/en/Tutorial/Sweep
 */

#ifndef PillDoor_h
#define PillDoor_h

#include <Servo.h>

class PillDoor {
  Servo servo;
  int pin;
  int pos;
  int startPos;
  int endPos;
  int increment;
  int updateInterval;
  unsigned long lastUpdate;
  bool positiveChange;
  bool dispensing = false;
  bool resetting = false;

public:
  PillDoor(int pin, int duration, int startPos, int endPos); // door movement duration in millis

  void attach();
  void dispense();
  void update();
  bool isDispensing();
};

#endif
