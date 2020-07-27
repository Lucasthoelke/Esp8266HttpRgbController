#include "Payload.hpp"

void PayloadParser::parsePayload(Payload payload)
{
    if (payload.authkey != AUTHKEY) return;

    //Switch does not work String (?) so if-else it is....
    if (payload.function == "setRGB")
    {
        if (
            payload.startLed > -1 && 
            payload.endLed > -1 &&
            payload.r > -1 &&
            payload.g > -1 &&
            payload.b > -1 &&
            payload.brightness > -1) 
            {
                Serial.println("SET RGB");
                PayloadParser::setRGB(payload);
            }
    }
}

void PayloadParser::setRGB(Payload payload)
{
    ;
}