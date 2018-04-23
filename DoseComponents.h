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
  unsigned long lastDailyTime;

public:
  PainQuestion();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
  bool shouldAskDaily();
};

class DoseQuestion : public Component {

public:
  DoseQuestion();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

class OverrideQuestion : public Component {

public:
  OverrideQuestion();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

class OverrideOptions : public Component {
  
public:
  OverrideOptions();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
  void drawOption(Adafruit_GFX &g, char* option, int dx, int y);
};

class DispensingInfo : public Component {

public:
  DispensingInfo();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

#endif
