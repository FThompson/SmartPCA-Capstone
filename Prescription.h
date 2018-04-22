/*
 * 
 */

#ifndef Prescription_h
#define Prescription_h

class Prescription {
  unsigned long lastDoses[];

public:
  char* label;
  int maxDose;
  int doseWindow;

  Prescription(char* label, int maxDose, int doseWindow);
  int getTimeUntilNextDose();
  int getAvailableDoses();
};

#endif
