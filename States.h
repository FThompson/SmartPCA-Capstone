/*
 * Device action states.
 */

#ifndef States_h
#define States_h

enum class State {
  HOME,
  PAIN_QUESTION,
  REQUEST_DOSE,
  DISPENSING,
  OVERRIDE_DOSE,
  OVERRIDE_REASON,
  MENU,
  SETTINGS,
  PRESCRIPTION,
  CONTACT
};

#endif
