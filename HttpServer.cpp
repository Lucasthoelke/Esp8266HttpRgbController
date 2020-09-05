#include "HttpServer.hpp"

//Start web server on defined port.
//This is here and not in a function because otherwise it will throw linker errors. Fix this later.
ESP8266WebServer HttpServer::server(STAPRT);

//Payload buffer for advanced frames. Max 255 payloads
Payload HttpServer::payloadBuffer[255];
unsigned short HttpServer::payloadBufferPos;

/*
* This is run on every loop() arduino function
*/
void HttpServer::onLoop()
{
  server.handleClient();
  MDNS.update();
}

void HttpServer::connectWifi()
{
  //Set WiFi mode and start
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting WiFi (" + String(ssid) + ")");

  //Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to '" + String(ssid) + "' as '");
  Serial.print(WiFi.localIP());
  Serial.print("'.\n");

  //Start MDNS responder
  if (MDNS.begin(MDNSID)) 
  {
    Serial.println("MDNS responder started. (" + String(MDNSID) + ")");
  }
  else
  {
    Serial.println("Could not start MDNS responder.");
  }
}

/*
* Start server and setup handlers
*/
void HttpServer::setupWebServer()
{
  server.onNotFound(HttpServer::handleNotFound);

  server.on("/", HttpServer::handleRoot);

  server.on("/command", HttpServer::handleCommand);

  server.on("/setRGB", HttpServer::handleSetRGB);

  server.begin();
}

Payload HttpServer::generatePayload()
{
  Payload payload = Payload();

  //if (server.hasArg("authkey"))
    //payload.authkey = server.arg("authkey");

  if (server.hasArg("function"))
    payload.function = server.arg("function");

  if (server.hasArg("startLed"))
    payload.startLed = server.arg("startLed").toInt();

  if (server.hasArg("endLed"))
    payload.endLed = server.arg("endLed").toInt();

  if (server.hasArg("r"))
    payload.r = server.arg("r").toInt();

  if (server.hasArg("g"))
    payload.g = server.arg("g").toInt();

  if (server.hasArg("b"))
    payload.b = server.arg("b").toInt();

  if (server.hasArg("brightness"))
    payload.brightness = server.arg("brightness").toInt();
  
  return payload;
}

/*
* This is called when 404
*/
void HttpServer::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void HttpServer::handleRoot()
{
  String message = "POST:";
  message += server.args();
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(200, "text/plain", message);
}

void HttpServer::handleJsonPayload()
{

  if (!HttpServer::isAuthenticated()) {
    server.send(401, "text/plain", "Not authenticated.");
    return;
  }

  String outputHtml = "";

  String inputJson;

  const int jsonCapacity = JSON_OBJECT_SIZE(10);
  DynamicJsonDocument jsonDocument(jsonCapacity); //TODO: Calculate capacity

  /* Try to parse json */
  DeserializationError jsonError = deserializeJson(jsonDocument, inputJson);

  if (jsonError) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(jsonError.c_str());
    server.send(500, "text/plain", "Json error. See Serial for details.");
  }


}

void HttpServer::handleSetBrightness() {
  if (!HttpServer::isAuthenticated()) {
    server.send(401, "text/plain", "Not authenticated.");
    return;
  }

  int brightness = 0;
  if (server.hasArg("brightness")) {
    brightness = server.arg("brightness").toInt();
  }

  LedManager::setBrightness(brightness);

  server.send(200, "text/plain", "Brightness set.");
}

void HttpServer::handleSetRGB()
{
  if (!HttpServer::isAuthenticated()) {
    server.send(401, "text/plain", "Not authenticated.");
    return;
  }

  int R = 0;
  if (server.hasArg("R")) {
    R = server.arg("R").toInt();
  }

  int G = 0;
  if (server.hasArg("G")) {
    G = server.arg("G").toInt();
  }

  int B = 0;
  if (server.hasArg("B")) {
    B = server.arg("B").toInt();
  }

  int startLED = 0;
  if (server.hasArg("startLED")) {
    startLED = server.arg("startLED").toInt();
  }

  int endLED = 0;
  if (server.hasArg("endLED")) {
    endLED = server.arg("endLED").toInt();
  }

  Serial.print(startLED);
  Serial.print(endLED);

  if (endLED > LED_NUM_LEDS) endLED = LED_NUM_LEDS;

  for (int i = startLED; i < endLED; i++) {
    LedManager::setRGB(R, G, B, i);
    Serial.print("SET ");
    Serial.println(i);
  }

  LedManager::display();

  server.send(200, "text/plain", "RGB set.");
}

void HttpServer::handleCommand()
{
  String intention = "displayPayload"; //default is displayPayload
  String message = "__TODO__ Add useful info here";

  if (!HttpServer::isAuthenticated())
  {
    server.send(401, "text/plain", "AUTH ERROR");
    return;
  }

  if (server.hasArg("intention"))
  {
    intention = server.arg("intention");
  }

  Payload payload = HttpServer::generatePayload();

  if (intention == "displayPayload")
  {
    PayloadParser::parsePayload(payload);
  }
  else if (intention == "resetPayloadBuffer")
  {
    memset(HttpServer::payloadBuffer, 0, sizeof(HttpServer::payloadBuffer));
    HttpServer::payloadBufferPos = 0;
  }
  else if (intention == "addPayloadBuffer")
  {
    HttpServer::payloadBuffer[HttpServer::payloadBufferPos] = payload;
    HttpServer::payloadBufferPos++;
  }
  else if (intention == "displayPayloadBuffer")
  {
    for (int i = 0; i < sizeof(HttpServer::payloadBufferPos); i++)
    {
      PayloadParser::parsePayload(HttpServer::payloadBuffer[i]);
    }
  }

  server.send(200, "text/plain", message);
}

boolean HttpServer::isAuthenticated()
{
  if (server.hasArg("authkey"))
  {
    if (server.arg("authkey") == AUTHKEY)
    {
      return true;
    }
  }

  return false;
}