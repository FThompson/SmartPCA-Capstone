/*
 * 
 */

#ifndef Controls_h
#define Controls_h

#include "Prescription.h"

void dispense(bool left);
void dispense(bool left, int count);
void setDesiredDose(int count);
int getDesiredDose();
void setLED(bool left, bool on);
void setBrightness(int brightness);
void setBrightness(int brightness, int duration);
void playTone();
void setState(State newState);
State getState();
void setSelectedPrescription(Prescription &prescription);
Prescription* getSelectedPrescription();

#endif
