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
#include "CommonGFX.h"

#define DOSE_INFO_X 34
#define DOSE_INFO_Y 63
#define DOSE_INFO_WIDTH 190
#define DOSE_INFO_HEIGHT 231 

int doseInfoXDiff = 0;

HelloLabel::HelloLabel() : Component(210, 25, 60, 25) {
  
}

bool HelloLabel::isValid(State state) {
  return state == State::HOME;
}

void HelloLabel::onRepaint(Adafruit_GFX &g) {
  g.setCursor(dx(0), dy(20));
  g.setTextSize(1);
  g.setTextColor(RIIT_GRAY);
  g.print(F("Hello."));
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
  int doses = prescription.getAvailableDoses();
  if (doses == prescription.maxDose) {
    g.fillRoundRect(dx(20), dy(12), 150, 150, 75, color);
    g.setCursor(dx(28), dy(100));
    g.setTextSize(2);
    g.setTextColor(WHITE);
    g.print(F("Ready"));
  } else {
    drawProgressCircle(g);
  }
  for (int i = 0; i < prescription.maxDose; i++) {
    int dotDX = i * 19;
    if (i < doses) {
      g.fillCircle(dx(76) + dotDX, dy(182), 5, color);
    } else {
      g.drawCircle(dx(76) + dotDX, dy(182), 5, color);
    }
  }
  g.setCursor(dx(52), dy(220));
  g.setTextSize(1);
  g.setTextColor(RIIT_GRAY);
  g.print(prescription.label);
}

void DoseInfo::drawProgressCircle(Adafruit_GFX &g) {
  g.drawRoundRect(dx(20), dy(12), 150, 150, 75, RIIT_GRAY); // full circle
  long doseTime = 60 * 60 * 1000L;//prescription.getTimeUntilNextDose();
  float progress = 1.0 - (doseTime / (float) prescription.doseWindow);
  float rads = 2 * PI * progress;
  float start = (5 * PI / 2) - rads;
  int x = dx(95);
  int y = dy(87);
  int r = 75;
  drawArc(g, x, y, r, start, rads, color);
  int dotX = (int) (r * cos(start) + 0.5);
  int dotY = (int) (r * -sin(start) + 0.5);
  g.fillCircle(x + dotX, y + dotY, 5, color);
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  long minutes = doseTime / (60 * 1000L);
  if (minutes >= 60) {
    long hours = ceil(minutes / 60.0);
    g.setCursor(dx(82), dy(90));
    g.print(hours);
    g.setTextSize(1);
    g.setCursor(dx(65), dy(120));
    g.print(F("hours"));
  } else {
    g.setCursor(dx(minutes < 10 ? 82 : 65), dy(90));
    g.print(minutes);
    g.setTextSize(1);
    g.setCursor(dx(50), dy(120));
    g.print(F("minutes"));
  }
}

void DoseInfo::onPress(int x, int y) {
  
}

void DoseInfo::onClick(int x, int y) {
  setSelectedPrescription(prescription);
  setState(State::PAIN_QUESTION);
}

void drawFace(Adafruit_GFX &g, int x, int y, int face) {
  g.drawCircle(x, y, 34, BLACK);
  g.fillCircle(x - 10, y - 10, 3, BLACK);
  g.fillCircle(x + 10, y - 10, 3, BLACK);
  if (face == 0) { // happy
    drawArc(g, x, y, 20, PI + PI / 12, PI - PI / 6, BLACK);
  } else if (face == 1) { // flat
    g.drawFastHLine(x - 17, y + 12, 34, BLACK);
  } else if (face == 2) { // sad
    drawArc(g, x, y + 24, 20, PI / 6, PI - PI / 3, BLACK);
  }
}

PainQuestion::PainQuestion() : Component(0, 0, 480, 320) {
  
}

bool PainQuestion::isValid(State state) {
  return state == State::PAIN_QUESTION;
}

void PainQuestion::onRepaint(Adafruit_GFX &g) {
  g.fillRect(10, 40, 460, 150, WHITE);
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  g.setCursor(12, 93);
  if (shouldAskDaily()) {
    g.println(F("How is your pain being"));
    g.setCursor(82, 153);
    g.println(F("managed overall?"));
  } else {
    g.println(F("How tolerable is your"));
    g.setCursor(82, 153);
    g.println(F("pain right now?"));
  }
  g.fillRect(0, 191, 480, 129, color);
  drawFace(g, 80, 255, 0);
  g.fillRect(159, 191, 3, 129, WHITE);
  drawFace(g, 240, 255, 1);
  g.fillRect(319, 191, 3, 129, WHITE);
  drawFace(g, 400, 255, 2);
}

void PainQuestion::onPress(int x, int y) {
}

void PainQuestion::onClick(int x, int y) {
  if (y >= 191) {
    color = RIIT_LIGHT_GRAY;
    if (shouldAskDaily()) {
      lastDailyTime = millis();
      repaint();
      Serial.print(F("Daily pain: "));
    } else {
      setState(State::REQUEST_DOSE);
      Serial.print(F("Current pain: "));
    }
    if (x < 160) {
      Serial.println("positive");
    } else if (x < 320) {
      Serial.println("neutral");
    } else {
      Serial.println("negative");
    }
  }
}

bool PainQuestion::shouldAskDaily() {
  return lastDailyTime == 0 || ((millis() - lastDailyTime) > 24 * 60 * 60 * 1000L);
}

