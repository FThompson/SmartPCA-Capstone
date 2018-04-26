/*
 * 
 */

#include "Prescription.h"
#include "Arduino.h"

Prescription::Prescription(char* label, int maxDose, long doseWindow, bool showOverride)
    : label(label), maxDose(maxDose), doseWindow(doseWindow), showOverride(showOverride) {

}

long Prescription::getTimeUntilNextDose() {
  if (lastDoses[0] > 0) {
    for (int i = 0; i < maxDose; i++) {
      long timeSinceLastDose = millis() - lastDoses[i];
      long time = doseWindow * (i + 1) - timeSinceLastDose;
      if (time >= 0) {
        return time;
      }
    }
  }
  return 0;
}

int Prescription::getAvailableDoses() {
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

