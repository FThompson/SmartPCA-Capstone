/*
 * Menu icon and menu components.
 */

#include "MenuComponents.h"
#include "Component.h"
#include <Adafruit_GFX.h>
#include "Colors.h"
#include "States.h"

#define MENU_ICON_X 300
#define MENU_ICON_Y 300
#define MENU_ICON_WIDTH 30
#define MENU_ICON_HEIGHT 30

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
  g.fillRoundRect(dx(0), dy(0), w, 8, 4, BLACK);
  g.fillRoundRect(dx(0), dy(11), w, 8, 4, BLACK);
  g.fillRoundRect(dx(0), dy(22), w, 8, 4, BLACK);
}

void MenuIcon::onTouch(int x, int y) {
  
}

