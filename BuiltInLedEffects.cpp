#include "BuiltInLedEffects.hpp"

void BuiltInLedEffects::SOS()
{
  BuiltInLedEffects::dot(); BuiltInLedEffects::dot(); BuiltInLedEffects::dot();
  BuiltInLedEffects::dash(); BuiltInLedEffects::dash(); BuiltInLedEffects::dash();
  BuiltInLedEffects::dot(); BuiltInLedEffects::dot(); BuiltInLedEffects::BuiltInLedEffects::dot();
}

void BuiltInLedEffects::dot()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

void BuiltInLedEffects::dash()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}
