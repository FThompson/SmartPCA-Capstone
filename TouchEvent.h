/*
 * A touchscreen event of a certain type that occurred at a point on the screen.
 */

#ifndef TouchEvent_h
#define TouchEvent_h

#include "TouchScreen.h"

class TouchEvent {
public:
  enum class Type {
    PRESS,
    CLICK
    // TODO: implement component-tied release and mouseover/out
  };

private:
  Type type;
  int x;
  int y;
  int z;
  
public:
  TouchEvent(Type type, TSPoint point);
  Type getType();
  int getX();
  int getY();
  int getPressure();
};

#endif TouchEvent_h
