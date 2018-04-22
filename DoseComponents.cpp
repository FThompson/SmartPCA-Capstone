/*
 * 
 */

#include "DoseComponents.h"
#include "Component.h"
#include <Adafruit_GFX.h>
#include "Colors.h"
#include "States.h"
#include "Controls.h"
#include "Prescription.h"

#define DOSE_INFO_X 34
#define DOSE_INFO_Y 63
#define DOSE_INFO_WIDTH 190
#define DOSE_INFO_HEIGHT 231

int doseInfoXDiff = 0;

void drawArc(Adafruit_GFX &g, int x, int y, int r, float startRads, float angleRads, int color) {
  float step = angleRads / (r * 4); // appropriate amount of steps?
  for (float theta = startRads; theta < startRads + angleRads; theta += step) {
    int dx = (int) (r * cos(theta) + 0.5);
    int dy = (int) (r * sin(theta) + 0.5);
    g.drawPixel(x + dx, y + dy, color);
  }
}

DoseInfo::DoseInfo(int color, Prescription prescription) : color(color), prescription(prescription),
    Component(DOSE_INFO_X + doseInfoXDiff, DOSE_INFO_Y, DOSE_INFO_WIDTH, DOSE_INFO_HEIGHT) {
  doseInfoXDiff += 221;
}

bool DoseInfo::isValid(State state) {
  return state == State::HOME;
}

void DoseInfo::onRepaint(Adafruit_GFX &g) {
  g.drawRect(x, y, w, h, borderColor);
  g.drawRoundRect(dx(20), dy(12), 150, 150, 75, RIIT_GRAY);
  int doses = prescription.getAvailableDoses();
  if (doses == prescription.maxDose) {
    //g.fillRoundRect(dx(20), dy(12), 150, 150, 75, color);
    g.setCursor(dx(40), dy(100));
    g.setTextSize(2);
    g.setTextColor(RIIT_GRAY);
    g.print("Ready");
  } else {
    
  }
  drawArc(g, dx(95), dy(87), 75, PI / 2, PI, RIIT_BLUE);
  g.setCursor(dx(80), dy(100));
  g.setTextSize(2);
  if (doses == prescription.maxDose) {
    
  }
  g.setCursor(dx(80), dy(140));
  g.setTextSize(1);
}

void DoseInfo::drawProgressCircle(Adafruit_GFX &g) {
  int progress = prescription.getTimeUntilNextDose() / prescription.doseWindow * 100;
  float rads = 2 * PI / progress;
  float start = (5 * PI / 2) - rads;
  int x = dx(95);
  int y = dy(87);
  int r = 75;
  drawArc(g, x, y, r, start, rads, color);
  int dotX = (int) (r * cos(rads) + 0.5);
  int dotY = (int) (r * sin(rads) + 0.5);
  g.drawCircle(x + dotX, y + dotY, 5, color);
}

void DoseInfo::onPress(TouchEvent event) {
  
}

void DoseInfo::onClick(TouchEvent event) {
  setSelectedPrescription(prescription);
}

