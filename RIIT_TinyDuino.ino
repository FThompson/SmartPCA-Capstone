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

// hardware components
Adafruit_HX8357 tft(TFT_CS, TFT_DC);
TouchScreen ts(XP, YP, XM, YM, TOUCH_RESISTANCE);
LED leftLED(LEFT_LED_PIN);
LED rightLED(RIGHT_LED_PIN);
LED backlight(BACKLIGHT_PIN);
PillDoor leftDoor(1000);
PillDoor rightDoor(2000);

// software components
State state = State::HOME;
Component *components[] = { &MenuIcon() };

void setup() {
  Serial.begin(9600);
  tft.begin(HX8357D);
  tft.fillScreen(HX8357_BLUE);
  if (!SD.begin(SD_CS)) {
    Serial.println("failed to initialize SD card");
  }
  backlight.turnOn();
  leftLED.turnOn();
  rightLED.turnOn();
  leftDoor.attach(LEFT_SERVO_PIN);
  rightDoor.attach(RIGHT_SERVO_PIN);
  //drawBMP(tft, "jumpers.bmp", 0, 0);
}

void loop() {
  leftDoor.update();
  rightDoor.update();
  // for each component, if component is valid for current state, paint
  // if touch point is in a valid component, handle onTouch
}

void handleTouch() {
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return;
  }
  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
}

