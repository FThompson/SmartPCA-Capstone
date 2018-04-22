/*
 * 
 */

#include <Adafruit_GFX.h>
#include "Component.h"
#include "Colors.h"

Component::Component(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
}

int Component::getX() {
  return this->x;
}

int Component::getY() {
  return this->y;
}

int Component::getWidth() {
  return this->w;
}

int Component::getHeight() {
  return this->h;
}

bool Component::contains(int x, int y) {
  return (x >= this->x) && (x <= this->x + this->w) && (y >= this->y) && (y <= this->y + this->h);
}

int Component::dx(int x) {
  return x + this->x;
}

int Component::dy(int y) {
  return y + this->y;
}

void Component::paint(Adafruit_GFX &g) {
  if (needsRepaint) {
    onRepaint(g);
    needsRepaint = false;
  }
}

void Component::repaint() {
  needsRepaint = true;
}

void Component::clear(Adafruit_GFX &g) {
  Serial.print("clearing ");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(w);
  Serial.print(",");
  Serial.println(h);
  g.fillRect(x, y, w, h, WHITE);
}

