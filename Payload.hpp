/*
 * Parses payload JSON and runs the correct function.
 * payload example:
 * 
 * {
 *  "authkey": "123456abcdef",
 *  "function": "setRGB",
 *  "startLED": "0",
 *  "endLED": "20",
 *  "r": "255",
 *  "g": "150",
 *  "b": "255
 * }
 * 
 * Payload van be an array of payloads.
 * Payload function types:
 *  - setRGB > set RGB values
 *  - getRGB > respond with current RGB values
 */

#ifndef PAYLOAD_H
#define PAYLOAD_H
#define AUTHKEY "1234567"
#include <Arduino.h>
#endif

struct Payload 
{
    String authkey;
    String function;

    //Vars are -1 by default to check if they are properly set and not 0 (NULL)
    int startLed = -1;
    int endLed = -1;

    int r = -1;
    int g = -1;
    int b = -1;
    int brightness = -1;
};

class PayloadParser
{
    public:
        static void parsePayload(Payload payload);
        static void setRGB(Payload payload);
};