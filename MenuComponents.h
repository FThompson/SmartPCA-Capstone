/*
 * 
 */

#ifndef MenuComponents_h
#define MenuComponents_h

#include "Component.h"
#include <Adafruit_GFX.h>

class MenuIcon : public Component {
public:
  MenuIcon();
  bool isValid();
  void onRepaint(Adafruit_GFX &g);
  void onTouch(int x, int y);
};

#endif
