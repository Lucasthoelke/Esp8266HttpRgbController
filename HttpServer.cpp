#include "HttpServer.hpp"

//Start web server on defined port.
//This is here and not in a function because otherwise it will throw linker errors. Fix this later.
ESP8266WebServer HttpServer::server(STAPRT);

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

  server.on("/setRGB", HttpServer::handleSetRGB);

  server.on("/setBrightness", HttpServer::handleSetBrightness);

  server.begin();
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
  LedManager::display();

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