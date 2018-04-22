/*
 * 
 */

#ifndef DoseComponents_h
#define DoseComponents_h

#include "Component.h"
#include "Prescription.h"
#include "Colors.h"

class DoseInfo : public Component {
  int borderColor = RIIT_GRAY;
  int color;
  Prescription prescription;

public:
  DoseInfo(int color, Prescription prescription);
  bool isValid(State state);
  void onRepaint(Adafruit_GFX &g);
  void onPress(TouchEvent event);
  void onClick(TouchEvent event);
  void drawProgressCircle(Adafruit_GFX &g);
};

#endif
