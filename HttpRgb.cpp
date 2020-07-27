#include <Arduino.h>
#include "BuiltInLedEffects.hpp"
#include "HttpServer.hpp"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

//BuiltInLedEffects effects;

void setup() {
  //Wait one second before starting, makes debugging easier.
  delay(1000); 

  //Start serial with baud rate of 9600
  Serial.begin(9600);

  //Wait for serial
  while (!Serial) { ; }

  //Set LED_BUILTIN pin to OUTPUT.
  pinMode(LED_BUILTIN, OUTPUT);

  //Connect to WiFI
  HttpServer::connectWifi();

  //Set up webserver
  HttpServer::setupWebServer();

  Serial.println("Setup Complete.");
}

void loop() {
  HttpServer::onLoop();
}
