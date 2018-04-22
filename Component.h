/*
 * Common base class for graphical components.
 */

#ifndef Component_h
#define Component_h

#include <Adafruit_GFX.h>
#include "States.h"
#include "TouchEvent.h"

class Component {
  bool needsRepaint = true;

protected:
  int x;
  int y;
  int w;
  int h;
  
public:
  Component(int x, int y, int w, int h);
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  virtual bool isValid(State state);
  virtual void onRepaint(Adafruit_GFX &g);
  virtual void onPress(TouchEvent event);
  virtual void onClick(TouchEvent event);
  bool contains(int x, int y);
  int dx(int x); // translate x
  int dy(int y); // translate y
  void paint(Adafruit_GFX &g);
  void repaint();
  void clear(Adafruit_GFX &g);
};

#endif
