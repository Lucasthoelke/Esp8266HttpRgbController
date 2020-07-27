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

  server.on("/command", HttpServer::handleCommand);

  server.begin();
}

Payload HttpServer::generatePayload()
{
  Payload payload = Payload();

  if (server.hasArg("authkey"))
    payload.authkey = server.arg("authkey");

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

void HttpServer::handleCommand()
{
  String message = "__TODO__ Add useful info here";

  Payload payload = generatePayload();

  PayloadParser::parsePayload(payload);

  server.send(200, "text/plain", message);
}