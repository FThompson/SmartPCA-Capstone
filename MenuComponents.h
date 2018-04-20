/*
 * Menu icon and menu components.
 */

#ifndef MenuComponents_h
#define MenuComponents_h

#include "Component.h"
#include <Adafruit_GFX.h>

class MenuIcon : public Component {
public:
  MenuIcon();
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(TouchEvent event);
  void onClick(TouchEvent event);
};

#endif
