/*
 * 
 */

#include "Prescription.h"
#include "Arduino.h"

Prescription::Prescription(char* label, int maxDose, long doseWindow, bool showOverride)
    : label(label), maxDose(maxDose), doseWindow(doseWindow), showOverride(showOverride) {
  //lastDoses = new unsigned long[maxDose];
}

long Prescription::getTimeUntilNextDose() {
  if (lastDoses[0] == 0) {
    return 0;
  } else {
    long timeSinceLastDose = millis() - lastDoses[0];
    long time = doseWindow - timeSinceLastDose;
    return max(0, time);
  }
}

int Prescription::getAvailableDoses() {
  unsigned long lastDose = 0;
  int available = 0;
  unsigned long ms = millis();
  for (int i = 0; i < maxDose; i++) {
    if (lastDoses[i] == 0) {
      available++;
    } else {
      long curWindow = doseWindow * (i + 1);
      if ((ms - lastDoses[i]) > curWindow) {
        available++;
      }
    }
  }
  return available;
}

void Prescription::use(int count) {
  for (int i = maxDose - 1; i > 0; i--) {
    lastDoses[i] = lastDoses[i - 1]; // push back previous dose times
  }
  unsigned long ms = millis();
  for (int i = 0; i < count; i++) {
    lastDoses[i] = ms; // record current dose times
  }
}

