/*
 * 
 */

#include "FadeableLED.h"
#include "Arduino.h"

FadeableLED::FadeableLED(int pin) : LED(pin) {
  
}

// brightness 0-256
void FadeableLED::setBrightness(int brightness) {
  this->brightness = brightness;
  targetBrightness = brightness;
  analogWrite(pin, brightness);
}

void FadeableLED::fade(int brightness, int duration) {
  targetBrightness = brightness;
  int change = targetBrightness - this->brightness;
  updateInterval = duration / abs(change);
  if (updateInterval < 1) {
    updateInterval = 1; // is this necessary? ideally, would also increase increment as necessary
  }
  if (change > 0) {
    if (change > duration) {
      increment = change / duration;
    } else {
      increment = 1;
    }
  } else if (change < 0) {
    if (abs(change) > duration) {
      increment = change / duration;
    } else {
      increment = -1;
    }
  }
}

// to be called every cycle regardless of currently fading or not
void FadeableLED::update() {
  if (brightness != targetBrightness) {
    unsigned long ms = millis();
    if ((ms - lastUpdate) > updateInterval) {
      lastUpdate = ms;
      brightness += increment;
      analogWrite(pin, brightness);
    }
  }
}

int FadeableLED::getBrightness() {
  return brightness;
}

bool FadeableLED::isFading() {
  return brightness != targetBrightness;
}

