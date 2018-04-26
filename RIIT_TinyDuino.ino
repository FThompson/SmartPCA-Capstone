// calibration and usage sourced from Adafruit HX8357 library breakouttouchpaint example

#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include <Fonts/FreeSans12pt7b.h>
#include "TouchScreen.h"
#include <Servo.h>
#include "LED.h"
#include "PillDoor.h"
//#include <SD.h>
#include "States.h"
#include "MenuComponents.h"
#include "Component.h"
#include "Controls.h"
#include "FadeableLED.h"
#include "Colors.h"
#include "Prescription.h"
#include "DoseComponents.h"

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
#define DISPENSE_TURN_DURATION 1500

// state components
State state = State::HOME;
Prescription leftPrescription("Opioids", 3, 1 * 2 * 60 * 1000L, true);
Prescription rightPrescription("Tylenol", 2, 4 * 60 * 60 * 1000L, false);
Prescription* selectedPrescription;
bool pressed = false;
bool stateChanged = false;
bool leftDispense = false;
TSPoint lastPoint;
uint8_t desiredDose;
uint8_t dispenseCount;
uint8_t screenUpdateInterval = 1000 / SCREEN_HZ;
unsigned long lastScreenUpdateTime;
unsigned long lastTouchTime;
unsigned long lastGlanceUpdateTime;

// hardware components
Adafruit_HX8357 tft(TFT_CS, TFT_DC);
TouchScreen ts(XP, YP, XM, YM, TOUCH_RESISTANCE);
LED leftLED(LEFT_LED_PIN);
LED rightLED(RIGHT_LED_PIN);
FadeableLED backlight(BACKLIGHT_PIN);
PillDoor leftDoor(LEFT_SERVO_PIN, DISPENSE_TURN_DURATION, 0, 120);
PillDoor rightDoor(RIGHT_SERVO_PIN, DISPENSE_TURN_DURATION, 180, 60);

// software components
BackButton backButton;
MenuIcon menuIcon;
DoseInfo leftDoseInfo(RIIT_BLUE, leftPrescription);
DoseInfo rightDoseInfo(RIIT_PURPLE, rightPrescription);
HelloLabel helloLabel;
MenuOption viewRX(14, 14, "View Prescription", State::PRESCRIPTION);
MenuOption settings(14, 14, "Settings", State::SETTINGS);
MenuOption contact(13, 17, "Contact Doctor", State::CONTACT);
PrescriptionInfo prescriptionInfo("John Doe", "05/05/18", "1234567", "Take 3 pills every 3 hours");
PainQuestion painQuestion;
DoseQuestion doseQuestion;
OverrideQuestion overrideQuestion;
OverrideOptions overrideOptions;
DispensingInfo dispensingInfo;
Component *components[] = {
  &backButton,
  &menuIcon,
  &viewRX,
  &settings,
  &contact,
  &prescriptionInfo,
  &helloLabel,
  &leftDoseInfo,
  &rightDoseInfo,
  &painQuestion,
  &doseQuestion,
  &overrideQuestion,
  &overrideOptions,
  &dispensingInfo
};
uint8_t componentCount = 14;

void setup() {
  Serial.begin(9600);
//  Serial.println(leftPrescription.label);
//  Serial.println(leftPrescription.maxDose);
//  Serial.println(leftPrescription.doseWindow);
//  Serial.println("last dose times: ");
//  for (int i = 0; i < leftPrescription.maxDose; i++) {
//    Serial.println(leftPrescription.lastDoses[i]);
//  }
//  Serial.println("available: ");
//  Serial.println(leftPrescription.getAvailableDoses());
//  Serial.println("time until next dose: ");
//  Serial.println(leftPrescription.getTimeUntilNextDose());
//  leftPrescription.use(1);
//  Serial.println("last dose times: ");
//  for (int i = 0; i < leftPrescription.maxDose; i++) {
//    Serial.println(leftPrescription.lastDoses[i]);
//  }
//  Serial.println("available: ");
//  Serial.println(leftPrescription.getAvailableDoses());
//  Serial.println("time until next dose: ");
//  Serial.println(leftPrescription.getTimeUntilNextDose());
  tft.begin(HX8357D);
  tft.setRotation(1); // rotate to landscape mode
  tft.setFont(&FreeSans12pt7b);
  tft.fillScreen(WHITE);
//  if (!SD.begin(SD_CS)) {
//    Serial.println(F("failed to initialize SD card"));
//  }
  backlight.turn(true);
  leftLED.turn(true);
  rightLED.turn(true);
  leftDoor.attach();
  rightDoor.attach();
}

void loop() {
  leftDoor.update();
  rightDoor.update();
  backlight.update();
  // hacky multi dispense handling, TODO: fix w/ OOP
  if (leftDispense) {
    updateDispense(leftDoor);
  } else {
    updateDispense(rightDoor);
  }
  updateGlance();
  updateComponents();
  delay(1);
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
      lastTouchTime = ms;
    }
  } else { // no current touch
    if (pressed) {
      clicked = true;
      pressed = false;
    }
  }
  // paint valid components, pass touch event if applicable
  for (int i = 0; i < componentCount && !stateChanged; i++) {
    Component *curr = components[i];
    if (curr->isValid(state)) {
      curr->paint(tft);
      if (clicked && curr->contains(lastPoint.x, lastPoint.y)) {
        curr->onClick(lastPoint.x, lastPoint.y);
      } else if (pressed && curr->contains(p.x, p.y)) {
        curr->onPress(p.x, p.y);
      }
    }
  }
  stateChanged = false; // detects if a another component changes the state
}

// hacky non-oop solution to multi dispense
// call double for resetting
void updateDispense(PillDoor &pillDoor) {
  if (dispenseCount > 0) {
    if (!pillDoor.isDispensing()) {
      pillDoor.dispense();
      if (dispenseCount % 2 == 0) { // actual dispense, not servo reset
        // tone
      }
      dispenseCount--;
      if (dispenseCount == 0) {
        setState(State::HOME);
      }
    }
  }
}

void updateGlance() {
  unsigned long ms = millis();
  if (state == State::HOME && (ms - lastGlanceUpdateTime) > 30000) {
    leftDoseInfo.requestClear();
    leftDoseInfo.repaint();
    rightDoseInfo.requestClear();
    rightDoseInfo.repaint();
    lastGlanceUpdateTime = ms;
  }
}

// calibrates a point to the screen and screen orientation
void translateTouchPoint(TSPoint &p) {
  // scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  // map rotation to `````````````````````````````````````````````````````````````````````````````````````````````````````````````screen rotation
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

void dispense(bool left, int count) {
  leftDispense = left;
  dispenseCount = count;
}

void setDesiredDose(int count) {
  desiredDose = count;
}

int getDesiredDose() {
  return desiredDose;
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
  clearComponents();
  state = newState;
  stateChanged = true;
}

void clearComponents() {
  for (int i = 0; i < componentCount; i++) {
    Component *curr = components[i];
    if (curr->isValid(state)) {
      curr->clear(tft);
      curr->repaint(); // set component to needsRepaint mode for next use
    }
  }
}

State getState() {
  return state;
}

void setSelectedPrescription(Prescription &prescription) {
  selectedPrescription = &prescription;
}

Prescription* getSelectedPrescription() {
  return selectedPrescription;
}

