/*
 * 
 */

#ifndef DoseComponents_h
#define DoseComponents_h

#include "Component.h"
#include "Prescription.h"
#include "Colors.h"

class HelloLabel : public Component {
public:
  HelloLabel();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y) {};
  void onClick(int x, int y) {};
};

class DoseInfo : public Component {
  int borderColor = RIIT_GRAY;
  int color;
  Prescription prescription;

public:
  DoseInfo(int color, Prescription prescription);
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
  void drawProgressCircle(Adafruit_GFX &g);
};

class PainQuestion : public Component {
  int color = RIIT_LIGHT_GRAY;
  unsigned long lastDailyTime;

public:
  PainQuestion();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
  bool shouldAskDaily();
};

#endif
