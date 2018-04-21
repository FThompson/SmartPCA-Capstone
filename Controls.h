/*
 * 
 */

#ifndef Controls_h
#define Controls_h

void dispense(bool left);
void setLED(bool left, bool on);
void setBrightness(int brightness);
void setBrightness(int brightness, int duration);
void playTone();
void setState(State newState);

#endif
