// calibration and usage sourced from Adafruit HX8357 library breakouttouchpaint example

#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include <Servo.h>
#include "LED.h"
#include "PillDoor.h"
#include <SD.h>
#include "DrawBMP.h"
#include "States.h"
#include "MenuComponents.h"
#include "Component.h"
#include "Controls.h"
#include "FadeableLED.h"

// non-SPI tft pins
#define TFT_DC 2
#define TFT_CS 3
#define SD_CS 4

// touchscreen analog pins
#define YP A2
#define XM A3
#define YM 7
#define XP 8

// touch calibration data
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TOUCH_RESISTANCE 300 // resistance across X plate

// component pinouts
#define AUDIO_PIN 5
#define BACKLIGHT_PIN 6
#define LEFT_SERVO_PIN 9
#define RIGHT_SERVO_PIN 10
#define LEFT_LED_PIN A0
#define RIGHT_LED_PIN A1

// device settings
#define SCREEN_HZ 10;
#define DISPENSE_TURN_DURATION 2000

// hardware components
Adafruit_HX8357 tft(TFT_CS, TFT_DC);
TouchScreen ts(XP, YP, XM, YM, TOUCH_RESISTANCE);
LED leftLED(LEFT_LED_PIN);
LED rightLED(RIGHT_LED_PIN);
FadeableLED backlight(BACKLIGHT_PIN);
PillDoor leftDoor(LEFT_SERVO_PIN, DISPENSE_TURN_DURATION);
PillDoor rightDoor(RIGHT_SERVO_PIN, DISPENSE_TURN_DURATION);

// software components
int componentCount = 1;
MenuIcon menuIcon;
Component *components[] = { 
  &menuIcon
};

// state components
State state = State::HOME;
bool pressed = false;
TSPoint lastPoint;
int screenUpdateInterval = 1000 / SCREEN_HZ;
unsigned long lastScreenUpdateTime;

void setup() {
  Serial.begin(9600);
  tft.begin(HX8357D);
  tft.setRotation(1); // rotate to landscape mode
  tft.fillScreen(HX8357_BLUE);
  if (!SD.begin(SD_CS)) {
    Serial.println("failed to initialize SD card");
  }
  backlight.turn(true);
  leftLED.turn(true);
  rightLED.turn(true);
//  leftDoor.attach(LEFT_SERVO_PIN);
//  rightDoor.attach(RIGHT_SERVO_PIN);
  leftDoor.dispense();
  rightDoor.dispense();
  //drawBMP(tft, "jumpers.bmp", 0, 0);
}

void loop() {
  leftDoor.update();
  rightDoor.update();
  updateComponents();
}

// paints components and passes touch events
// TODO: fix issue where currently does not account for touch drag
// i.e. if pressing on a component and releasing elsewhere, click still registered
void updateComponents() {
  unsigned long ms = millis();
  if ((ms - lastScreenUpdateTime) < screenUpdateInterval) {
    return; // only update screen components at 10hz
  }
  lastScreenUpdateTime = ms;
  bool clicked = false;
  TSPoint p = ts.getPoint();
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    if (!pressed) {
      pressed = true;
      translateTouchPoint(p);
      lastPoint = p;
    }
  } else { // no current touch
    if (pressed) {
      clicked = true;
      pressed = false;
    }
  }
  // paint valid components, pass touch event if applicable
  for (int i = 0; i < componentCount; i++) {
    Component *curr = components[i];
    if (components[i]->isValid(state)) {
      components[i]->paint(tft);
      if (clicked && components[i]->contains(lastPoint.x, lastPoint.y)) {
        TouchEvent event(TouchEvent::Type::CLICK, lastPoint);
        components[i]->onClick(event);
      } else if (pressed && components[i]->contains(p.x, p.y)) {
        TouchEvent event(TouchEvent::Type::PRESS, p);
        components[i]->onPress(event);
      }
    }
  }
}

// calibrates a point to the screen and screen orientation
void translateTouchPoint(TSPoint &p) {
  // scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  // map rotation to screen rotation
  int oldX = p.x;
  p.x = map(p.y, 0, tft.height(), 0, tft.width());
  p.y = map(oldX, 0, tft.width(), tft.height(), 0);
}

void dispense(bool left) {
  if (left) {
    leftDoor.dispense();
  } else {
    rightDoor.dispense();
  }
}

void setLED(bool left, bool on) {
  if (left) {
    leftLED.turn(on);
  } else {
    rightLED.turn(on);
  }
}

void setBrightness(int brightness) {
  backlight.setBrightness(brightness);
}

void setBrightness(int brightness, int duration) {
  backlight.fade(brightness, duration);
}

void playTone() {
  
}

void setState(State newState) {
  state = newState;
}

