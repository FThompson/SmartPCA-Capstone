/*
 * 
 */

#ifndef Prescription_h
#define Prescription_h

class Prescription {

public:
  char* label;
  bool showOverride;
  int maxDose;
  long doseWindow; // milliseconds
  unsigned long lastDoses[5];

  Prescription(char* label, int maxDose, long doseWindow, bool showOverride);
  long getTimeUntilNextDose();
  int getAvailableDoses();
  void use(int count);
};

#endif
