/*
 * Device action states.
 */

#ifndef States_h
#define States_h

enum class State {
  HOME,
  PAIN_OVERALL,
  PAIN_NOW,
  REQUEST_OPIOID,
  REQUEST_ADJUVANT,
  DISPENSING,
  OVERRIDE_DOSE,
  OVERRIDE_REASON,
  MENU,
  SETTINGS,
  PRESCRIPTION,
  CONTACT
};

#endif
