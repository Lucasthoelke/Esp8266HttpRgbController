#include "Payload.hpp"

void Payload::fillFromJson(String json)
{
    
}

void PayloadParser::parsePayload(Payload payload)
{
    //Check if payload is authenticated
    //if (payload.authkey != AUTHKEY) return;

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
                PayloadParser::command_setRGB(payload);
            }
    }
}

void PayloadParser::command_setRGB(Payload payload)
{
    //Make sure startLed cannot be higher than endLed
    if (payload.startLed > payload.endLed) payload.startLed = payload.endLed;

    for (int i = payload.startLed; i < payload.endLed; i++)
    {
        LedManager::setBrightness(payload.brightness);
        LedManager::setRGB(payload.r, payload.g, payload.b, i);
    }

    LedManager::display();
}