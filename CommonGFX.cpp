/*
 * 
 */

#include "CommonGFX.h"
#include "Colors.h"

void drawArc(Adafruit_GFX &g, int x, int y, int r, float startRads, float angleRads, int color) {
  float step = angleRads / (r * 4); // appropriate amount of steps?
  for (float theta = startRads; theta < startRads + angleRads; theta += step) {
    int dx = (int) (r * cos(theta));
    int dy = (int) (r * -sin(theta));
    g.drawPixel(x + dx, y + dy, color);
  }
}

void drawButton(Adafruit_GFX &g, int x, int y, int w, int h, int dx, char* label, bool largeText) {
  g.fillRoundRect(x, y, w, h, 4, RIIT_GRAY);
  g.setCursor(x + dx, y);
  g.setTextSize(largeText ? 2 : 1);
  g.setTextColor(WHITE);
  g.print(label);
}

