/*
 * 
 */

#include "Prescription.h"
#include "Arduino.h"

Prescription::Prescription(char* label, int maxDose, int doseWindow) : label(label), maxDose(maxDose), doseWindow(doseWindow) {
  lastDoses[maxDose] = {};
}

int Prescription::getTimeUntilNextDose() {
  return max(0, millis() - lastDoses[0]);
}

int Prescription::getAvailableDoses() {
  unsigned long lastDose = 0;
  int available = 0;
  for (int i = 0; i < maxDose; i++) {
    int curWindow = doseWindow * (i + 1);
    if (lastDoses[i] > curWindow) {
      available++;
    }
  }
  return available;
}

