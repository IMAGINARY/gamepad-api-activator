/**
 * This sketch pulls a signal pin to an active state for a short time,
 * goes to sleep for about 1s and repeats. The onboard (or external,
 * if configured) LED flashes accordingly to indicate the activity.
 */

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef PIN_LED
#define PIN_LED LED_BUILTIN
#endif

#ifndef PIN_BUTTON
#define PIN_BUTTON PIN_LED
#endif

#ifndef BUTTON_ACTIVE_DURATION
#define BUTTON_ACTIVE_DURATION 50
#endif

#ifdef INVERT
#define BUTTON_ACTIVE_STATE LOW
#define BUTTON_INACTIVE_STATE HIGH
#else
#define BUTTON_ACTIVE_STATE HIGH
#define BUTTON_INACTIVE_STATE LOW
#endif

/**
 * watchdog interrupt
 */
ISR(WDT_vect) {
  wdt_disable(); // disable watchdog
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUTTON, BUTTON_INACTIVE_STATE);
}

void loop() {
  // briefly flash the LED and activate signal pin
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUTTON, BUTTON_ACTIVE_STATE);
  delay(BUTTON_ACTIVE_DURATION);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUTTON, BUTTON_INACTIVE_STATE);

  // go to sleep for 1s
  #ifdef ACTIVE_SLEEP
  // active sleep (not recommended but supports porting to other platforms)
  delay(1000);
  #else
  // power down during sleep to reduce power consumption
  {
    // disable ADC
    ADCSRA = 0;

    // clear various "reset" flags
    MCUSR = 0;

    wdt_enable(WDTO_1S);
    wdt_reset(); // pat the watchdog

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    noInterrupts(); // timed sequence follows
    sleep_enable();

    // turn off brown-out enable in software
    MCUCR = bit(BODS) | bit(BODSE);
    MCUCR = bit(BODS);
    interrupts(); // guarantees next instruction executed
    sleep_cpu();

    // cancel sleep as a precaution
    sleep_disable();
  }
  #endif
}
