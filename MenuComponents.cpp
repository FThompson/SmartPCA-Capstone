/*
 * Menu icon and menu components.
 */

#include "MenuComponents.h"
#include "Component.h"
#include <Adafruit_GFX.h>
#include "Colors.h"
#include "States.h"
#include "Controls.h"

#define BACK_BUTTON_X 0
#define BACK_BUTTON_Y 0
#define BACK_BUTTON_WIDTH 40
#define BACK_BUTTON_HEIGHT 40

#define MENU_ICON_X 440
#define MENU_ICON_Y 0
#define MENU_ICON_WIDTH 40
#define MENU_ICON_HEIGHT 40

#define CONTENT_X 39
#define CONTENT_Y 63
#define CONTENT_WIDTH 402
#define CONTENT_HEIGHT 247

#define MENU_OPTION_HEIGHT 56
#define MENU_OPTION_Y_DIFF 71;

int menuOptionYDiff = 0;

State getBackState() {
  if (getState() == State::MENU) {
    return State::HOME;
  } else {
    return State::MENU;
  }
  // no other possible back states
}

BackButton::BackButton() : Component(BACK_BUTTON_X, BACK_BUTTON_Y, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT) {
  
}

bool BackButton::isValid(State state) {
  switch (state) {
    case State::MENU:
    case State::SETTINGS:
    case State::PRESCRIPTION:
    case State::CONTACT:
      return true;
  }
  return false;
}

void BackButton::onRepaint(Adafruit_GFX &g) {
  g.drawLine(dx(20), dy(5), dx(5), dy(20), color);
  g.drawLine(dx(5), dy(20), dx(20), dy(35), color);
  g.drawFastHLine(dx(5), dy(20), 30, color);
}

void BackButton::onPress(TouchEvent event) {
  color = BLACK;
  repaint();
}

void BackButton::onClick(TouchEvent event) {
  color = RIIT_GRAY;
  setState(getBackState());
}

MenuIcon::MenuIcon() : Component(MENU_ICON_X, MENU_ICON_Y, MENU_ICON_WIDTH, MENU_ICON_HEIGHT) {
  
}

// only show menu icon when menu is not already open
bool MenuIcon::isValid(State state) {
  switch (state) {
    case State::MENU:
    case State::SETTINGS:
    case State::PRESCRIPTION:
    case State::CONTACT:
      return false;
  }
  return true;
}

// draw hamburger menu
void MenuIcon::onRepaint(Adafruit_GFX &g) {
  g.fillRoundRect(dx(5), dy(7), w - 10, 6, 3, color);
  g.fillRoundRect(dx(5), dy(17), w - 10, 6, 3, color);
  g.fillRoundRect(dx(5), dy(27), w - 10, 6, 3, color);
}

void MenuIcon::onPress(TouchEvent event) {
  color = BLACK;
  repaint();
}

void MenuIcon::onClick(TouchEvent event) {
  color = RIIT_GRAY;
  setState(State::MENU);
}

MenuOption::MenuOption(int iconX, int iconY, char *text, State toState)
    : iconX(iconX), iconY(iconY), text(text), toState(toState),
    Component(CONTENT_X, CONTENT_Y + menuOptionYDiff, CONTENT_WIDTH, MENU_OPTION_HEIGHT) {
  menuOptionYDiff += MENU_OPTION_Y_DIFF;
}

bool MenuOption::isValid(State state) {
  return state == State::MENU;
}

void MenuOption::onRepaint(Adafruit_GFX &g) {
  g.drawRoundRect(dx(0), dy(0), w, h, 10, backgroundColor);
  g.setCursor(dx(70), dy(30));
  g.setTextColor(RIIT_GRAY);
  g.print(text);
}

void MenuOption::onPress(TouchEvent event) {
  backgroundColor = RIIT_LIGHT_GRAY;
  repaint();
}

void MenuOption::onClick(TouchEvent event) {
  backgroundColor = WHITE;
  setState(toState);
}

PrescriptionInfo::PrescriptionInfo(char* info, char* date, char* number, char* frequency)
    : name(name), date(date), number(number), frequency(frequency),
    Component(CONTENT_X, CONTENT_Y, CONTENT_WIDTH, CONTENT_HEIGHT) {
  
}

bool PrescriptionInfo::isValid(State state) {
  return state == State::PRESCRIPTION;
}

void PrescriptionInfo::onRepaint(Adafruit_GFX &g) {
  g.setTextColor(RIIT_GRAY);
  g.setCursor(dx(0), dy(30));
  g.print(F("Name: "));
  g.print(name);
  g.setCursor(dx(0), dy(60));
  g.print(F("Date: "));
  g.print(date);
  g.setCursor(dx(0), dy(90));
  g.print(F("Prescription Number: "));
  g.print(number);
  g.setCursor(dx(0), dy(120));
  g.print(F("Frequency: "));
  g.print(frequency);
}

