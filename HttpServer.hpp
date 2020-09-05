#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include "Payload.hpp"
#endif

#ifndef STASSID
#define STASSID "Thoelke"
#define STAPSK  "Ganesha_2018"
#define STAPRT  80 //Not implemented
#define STAIP   127.0.0.1 //Not implemented
#define MDNSID  "esp8266"
#define AUTHKEY "1234567"
#endif

class HttpServer
{
  public:
    static void onLoop();
    static void connectWifi();
    static void setupWebServer();
    static Payload generatePayload();
  private:
    static constexpr char* ssid = STASSID;
    static constexpr char* password = STAPSK;
    static ESP8266WebServer server;
    static Payload payloadBuffer[255];
    static unsigned short payloadBufferPos;
    static void handleNotFound();
    static void handleRoot();
    static void handleCommand();
    static void handleJsonPayload();
    static void handleSetRGB();
    static void handleSetBrightness();
    static boolean isAuthenticated();
};
