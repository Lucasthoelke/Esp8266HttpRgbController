/*
 * Effects for the built in LED on the arduino/esp.
 */

#ifndef BUILTINLEFFECTS_H
#include <Arduino.h>
#define BUILTINLEFFECTS_H
#endif


 class BuiltInLedEffects
 {
  private:
    static void dot();
    static void dash();
  public:
    static void SOS();
 };
