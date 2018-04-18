/*
 Opens or closes a device pill dispensing door.

 Code inspiration from Sweeper by BARRAGAN
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#ifndef PillDoor_h
#define PillDoor_h

#include <Servo.h>

class PillDoor {
  Servo servo;
  int pos;
  int increment = 1;
  int updateInterval;
  unsigned long lastUpdate;
  bool dispensing = false;

public:
  PillDoor(int duration); // door movement duration in millis

  void attach(int pin);
  void dispense();
  void update();
  bool isDispensing();
};

#endif
