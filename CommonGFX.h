/*
 * 
 */

#ifndef CommonGFX_h
#define CommonGFX_h

#include <Adafruit_GFX.h>

void drawArc(Adafruit_GFX &g, int x, int y, int r, float startRads, float angleRads, int color);
void drawButton(Adafruit_GFX &g, int x, int y, int dx, char* label, bool largeText);

#endif
