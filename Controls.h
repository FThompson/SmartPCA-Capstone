/*
 * 
 */

#ifndef Controls_h
#define Controls_h

#include "Prescription.h"

void dispense(bool left);
void setLED(bool left, bool on);
void setBrightness(int brightness);
void setBrightness(int brightness, int duration);
void playTone();
void setState(State newState);
State getState();
void setSelectedPrescription(Prescription &prescription);
Prescription* getSelectedPrescription();
void drawBMP(const char *filename, uint8_t x, uint16_t y);

#endif
