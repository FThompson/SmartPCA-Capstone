/*
 * Menu icon and menu components.
 */

#ifndef MenuComponents_h
#define MenuComponents_h

#include "Component.h"
#include <Adafruit_GFX.h>
#include "Colors.h"
#include "States.h"

class BackButton : public Component {
  int color = RIIT_GRAY;
  
public:
  BackButton();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

class MenuIcon : public Component {
  int color = RIIT_GRAY;
  
public:
  MenuIcon();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

class MenuOption : public Component {
  int iconX;
  int iconY;
  char *text;
  State toState;
  int backgroundColor = WHITE;

public:
  MenuOption(int iconX, int iconY, char *text, State toState);
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

class PrescriptionInfo : public Component {
  char *name;
  char *date;
  char *number;
  char *frequency;

public:
  PrescriptionInfo(char *name, char *date, char *number, char *frequency);
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y) {};
  void onClick(int x, int y) {};
};

class ContactPage : public Component {
public:
  ContactPage();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(int x, int y);
  void onClick(int x, int y);
};

#endif
