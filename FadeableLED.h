/*
 * 
 */

#ifndef FadeableLED_h
#define FadeableLED_h

#include "LED.h"

class FadeableLED : public LED {
  int brightness;
  int increment = 1;
  int updateInterval;
  unsigned long lastUpdate;
  int targetBrightness;

public:
  FadeableLED(int pin);
  void turn(bool on);
  void setBrightness(int brightness);
  void fade(int brightness, int duration);
  void update();
  int getBrightness();
  bool isFading();
};

#endif
