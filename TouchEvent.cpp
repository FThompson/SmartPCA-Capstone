/*
 * A touchscreen event of a certain type that occurred at a point on the screen.
 */

#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include "TouchEvent.h">

TouchEvent::TouchEvent(Type type, TSPoint p) : type(type), x(p.x), y(p.y), z(p.z) {
  
}

TouchEvent::Type TouchEvent::getType() {
  return this->type;
}

int TouchEvent::getX() {
  return this->x;
}

int TouchEvent::getY() {
  return this->y;
}

int TouchEvent::getPressure() {
  return this->z;
}

