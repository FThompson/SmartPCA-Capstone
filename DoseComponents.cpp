/*
 * 
 */

// less regard for OOP and proper coding here due to time constraints
// TODO: return and fix the spaghetti

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
  long doseTime = prescription.getTimeUntilNextDose();
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

void drawTripleButton(Adafruit_GFX &g, int color) {
  g.fillRect(0, 191, 480, 129, color);
  g.fillRect(159, 191, 3, 129, WHITE);
  g.fillRect(319, 191, 3, 129, WHITE);
}

PainQuestion::PainQuestion() : Component(0, 0, 480, 320) {
  
}

bool PainQuestion::isValid(State state) {
  return state == State::PAIN_QUESTION;
}

void PainQuestion::onRepaint(Adafruit_GFX &g) {
  g.fillRect(0, 40, 480, 150, WHITE);
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  if (shouldAskDaily()) {
    g.setCursor(8, 93);
    g.println(F("How is your pain being"));
    g.setCursor(72, 153);
    g.println(F("managed overall?"));
  } else {
    g.setCursor(20, 93);
    g.println(F("How tolerable is your"));
    g.setCursor(82, 153);
    g.println(F("pain right now?"));
  }
  drawTripleButton(g, RIIT_LIGHT_GRAY);
  drawFace(g, 80, 255, 0);
  drawFace(g, 240, 255, 1);
  drawFace(g, 400, 255, 2);
}

void PainQuestion::onPress(int x, int y) {
}

void PainQuestion::onClick(int x, int y) {
  if (y >= 191) {
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

DoseQuestion::DoseQuestion() : Component(0, 0, 480, 320) {
  
}

bool DoseQuestion::isValid(State state) {
  return state == State::REQUEST_DOSE;
}

void DoseQuestion::onRepaint(Adafruit_GFX &g) {
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  g.setCursor(14, 93);
  g.print(F("How many "));
  g.print(getSelectedPrescription()->label);
  g.print(F(" do"));
  g.setCursor(34, 153);
  g.println(F("you need right now?"));
  drawTripleButton(g, RIIT_GRAY);
  g.setTextColor(WHITE);
  int maxDose = getSelectedPrescription()->maxDose; // hacky. TODO: auto size multiple buttons, no zero option
  g.setCursor(66, 268);
  g.print(maxDose - 2);
  g.setCursor(226, 268);
  g.print(maxDose - 1);
  g.setCursor(386, 268);
  g.print(maxDose);
}

void DoseQuestion::onPress(int x, int y) {
  
}

void DoseQuestion::onClick(int x, int y) {
  if (y >= 191) {
    Serial.print(F("Selected "));
    int doses = 0;
    int maxDose = getSelectedPrescription()->maxDose;
    if (x < 160) {
      doses = maxDose - 2;
      Serial.print(maxDose - 2);
    } else if (x < 320) {
      doses = maxDose - 1;
      Serial.print(maxDose - 1);
    } else {
      doses = maxDose;
      Serial.print(maxDose);
    }
    Serial.print(F(" doses of "));
    Serial.println(getSelectedPrescription()->label);
    if (doses > 0) {
      setDesiredDose(doses);
      if (getSelectedPrescription()->showOverride && doses > getSelectedPrescription()->getAvailableDoses()) {
        setState(State::OVERRIDE_DOSE);
      } else {
        setState(State::DISPENSING);
        getSelectedPrescription()->use(doses);
        dispense(getSelectedPrescription()->showOverride, doses); // so hacky
      }
    } else {
      setState(State::HOME);
    }
  }
}

OverrideQuestion::OverrideQuestion() : Component(0, 0, 480, 320) {
  
}

bool OverrideQuestion::isValid(State state) {
  return state == State::OVERRIDE_DOSE;
}

void OverrideQuestion::onRepaint(Adafruit_GFX &g) {
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  g.setCursor(18, 93);
  g.print(F("Oops, next dose is in"));
  g.setCursor(18, 153);
  long minutes = getSelectedPrescription()->getTimeUntilNextDose() / (60 * 1000L);
  if (minutes >= 60) {
    long hours = ceil(minutes / 60.0);
    g.print(hours);
    g.print(F(" hours"));
  } else {
    g.print(minutes);
    g.print(F(" minutes"));
  }
  g.print(F(". Would"));
  g.setCursor(18, 213);
  g.print(F("you like to override?"));
  g.fillRect(0, 236, 480, 84, RIIT_GRAY);
  g.fillRect(239, 236, 3, 84, WHITE);
  g.setTextColor(WHITE);
  g.setCursor(71, 292);
  g.print(F("YES"));
  g.setCursor(322, 292);
  g.print(F("NO"));
}

void OverrideQuestion::onPress(int x, int y) {
  
}

void OverrideQuestion::onClick(int x, int y) {
  if (y >= 229) {
    if (x < 240) {
      Serial.println("Chose to override");
      setState(State::OVERRIDE_REASON);
    } else {
      Serial.println("Chose not to override");
      setState(State::HOME);
    }
  }
}

OverrideOptions::OverrideOptions() : Component(0, 0, 480, 320) {
  
}

bool OverrideOptions::isValid(State state) {
  return state == State::OVERRIDE_REASON;
}

void OverrideOptions::onRepaint(Adafruit_GFX &g) {
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  g.setCursor(188, 73);
  g.print("Why?");
  drawOption(g, "I am in pain right now.", 75, 95);
  drawOption(g, "I lost or cannot take the pill.", 60, 165);
  drawOption(g, "Pill did not dispense.", 85, 235);
}

void OverrideOptions::onPress(int x, int y) {
  
}

void OverrideOptions::onClick(int x, int y) {
  if (x >= 39 && x <= 402) {
    bool clicked = false;
    if (y >= 95 && y <= 143) {
      Serial.println(F("Chose override: I am in pain right now."));
      clicked = true;
    } else if (y >= 165 && y <= 203) {
      Serial.println(F("Chose override: I lost or cannot take the pill."));
      clicked = true;
    } else if (y >= 235 && y <= 283) {
      Serial.println(F("Chose override: Pill did not dispense."));
      clicked = true;
    }
    if (clicked) {
      setState(State::DISPENSING);
      int doses = getDesiredDose();
      getSelectedPrescription()->use(doses);
      dispense(getSelectedPrescription()->showOverride, doses); // so hacky
    }
  }
}

void OverrideOptions::drawOption(Adafruit_GFX &g, char* option, int dx, int y) {
  g.fillRoundRect(39, y, 402, 48, 4, RIIT_GRAY);
  g.setTextSize(1);
  g.setTextColor(WHITE);
  g.setCursor(39 + dx, y + 30);
  g.print(option);
}

DispensingInfo::DispensingInfo() : Component(0, 0, 480, 320) {
  
}

bool DispensingInfo::isValid(State state) {
  return state == State::DISPENSING;
}

void DispensingInfo::onRepaint(Adafruit_GFX &g) {
  g.setTextSize(2);
  g.setTextColor(RIIT_GRAY);
  g.setCursor(18, 153);
  g.print(F("Dispensing "));
  g.print(getDesiredDose());
  g.print(F(" "));
  g.print(getSelectedPrescription()->label);
}

void DispensingInfo::onPress(int x, int y) {
  
}

void DispensingInfo::onClick(int x, int y) {
  
}

