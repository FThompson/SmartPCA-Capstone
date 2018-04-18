/*
 * 
 */

#ifndef Component_h
#define Component_h

#include <Adafruit_GFX.h>

class Component {
public:
  int x;
  int y;
  int w;
  int h;
  
  Component(int x, int y, int w, int h);
  virtual bool isValid();
  virtual void onRepaint(Adafruit_GFX &g);
  virtual void onTouch(int x, int y);
  bool contains(int x, int y);
  int dx(int x); // translate x
  int dy(int y); // translate y
  void paint(Adafruit_GFX &g);
  void repaint();
  
private:
  bool needsRepaint = true;
};

#endif
