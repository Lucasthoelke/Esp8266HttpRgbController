/*
 * Parses payload JSON and runs the correct function.
 * payload example:
 * 
{
  "function": "setRGB",
  "arguments": [
    {
      "startLED": "0",
      "endLED": "20",
      "R": "255",
      "G": "255",
      "B": "255"
    }
  ]
}
 * Payload van be an array of payloads.
 * Payload function types:
 *  - setRGB > set RGB values
 *  - getRGB > respond with current RGB values
 *  - setBrightness > set brightness
 *  - getBrightness > respond with current brightness value
 *  - getNumLeds > respond with NUM_LEDS
 *  - executeBuffer > executes payload buffer
 *  - clearBuffer > clears buffer
 *  - delay > delay for X
 */

#ifndef PAYLOAD_H
#define PAYLOAD_H
#include <Arduino.h>
#include "LedManager.hpp"
#endif

class Payload 
{
    public:
        //String authkey;
        String function;
        String jsonArguments;

        //Vars are -1 by default to check if they are properly set and not 0 (NULL)
        int startLed = -1;
        int endLed = -1;

        int r = -1;
        int g = -1;
        int b = -1;
        int brightness = -1;

        void fillFromJson(String json);
};

class PayloadParser
{
    public:
        static void parsePayload(Payload payload);
        static void command_setRGB(Payload payload);
};