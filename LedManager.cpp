#include "LedManager.hpp"

CRGB LedManager::leds[LED_NUM_LEDS];

void LedManager::setupLeds()
{
    FastLED.addLeds<LED_CHIPSET, LED_PIN, LED_COLOR_ORDER>(leds, LED_NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    LedManager::debugDance();
}

void LedManager::setRGB(int R, int G, int B, unsigned int ledNum)
{
    //If ledNum is higher than maximum set maximum
    if (ledNum > LED_NUM_LEDS) ledNum = LED_NUM_LEDS;

    CRGB rgb;

    rgb.r = R;
    rgb.g = G;
    rgb.b = B;

    LedManager::leds[ledNum] = rgb;
}

void LedManager::setBrightness(int brightness)
{
    FastLED.setBrightness(brightness);
}

void LedManager::display()
{
    FastLED.show();
}

void LedManager::debugDance()
{
    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        LedManager::leds[i] = CRGB::White;

        FastLED.show();

        delay(2000 / LED_NUM_LEDS);
    }

    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        LedManager::leds[i] = CRGB::Black;

        FastLED.show();

        delay(2000 / LED_NUM_LEDS);
    }

}