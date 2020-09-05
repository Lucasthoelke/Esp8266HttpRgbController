#ifndef LEDMANAGER_H
#define LEDMANAGER_H
#include <Arduino.h>
#include <FastLED.h>
#endif

#define LED_NUM_LEDS 26
#define LED_PIN 5 //Pin where led strip is connected
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER GRB

#define LED_BRIGHTNESS  128

class LedManager
{
    public:
        static void onLoop();
        static void setupLeds();
        static void setRGB(int R, int G, int B, unsigned int ledNum);
        static void setBrightness(int brightness);
        static void display();
        static void debugDance();
    protected:
        static CRGB leds[LED_NUM_LEDS];
};