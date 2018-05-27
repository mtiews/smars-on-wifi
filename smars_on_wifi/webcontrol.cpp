#include <functional>
#include <Arduino.h>
#include "Webcontrol.h"

/* c'tor */
Webcontrol::Webcontrol(
  int port,
  void (*moveForwardHandler)(void), 
  void (*moveLeftHandler)(void), 
  void (*moveRightHandler)(void), 
  void (*moveBackwardHandler)(void), 
  void (*moveStopHandler)(void),
  void (*enableAutomodeHandler)(void))
: _moveForwardHandler(moveForwardHandler)
, _moveLeftHandler(moveLeftHandler)
, _moveRightHandler(moveRightHandler)
, _moveBackwardHandler(moveBackwardHandler)  
, _moveStopHandler(moveStopHandler)  
, _enableAutomodeHandler(enableAutomodeHandler)  
{
  _webServer = new ESP8266WebServer(port);
}

void Webcontrol::begin() {
  _webServer->on("/", std::bind (&Webcontrol::_handleRoot, this));
  _webServer->onNotFound(std::bind (&Webcontrol::_handleRoot, this));
  
  _webServer->on("/command/forward", std::bind (&Webcontrol::_moveForward, this));
  _webServer->on("/command/left", std::bind (&Webcontrol::_moveLeft, this));
  _webServer->on("/command/stop", std::bind (&Webcontrol::_moveStop, this));
  _webServer->on("/command/right", std::bind (&Webcontrol::_moveRight, this));
  _webServer->on("/command/backward", std::bind (&Webcontrol::_moveBackward, this));
  _webServer->on("/command/auto", std::bind (&Webcontrol::_enableAutomode, this));
  
  _webServer->begin();
  Serial.println("Webcontrol: HTTP server with Webcontrol started");
}

void Webcontrol::handleClientCommands() {
  _webServer->handleClient();
}

void Webcontrol::_handleRoot() {
  String page = FPSTR(WEBCONTROL_PAGE);
  _webServer->send(200, "text/html", page);
}

void Webcontrol::_handleNotFound() {
  _webServer->send(404, "text/plain", "NOT FOUND");
}

void Webcontrol::_sendStatus(String status) {
  _webServer->send(200, "text/plain", status);
}

void Webcontrol::_moveForward() { 
  Serial.println("Webcontrol: Forward");
  _moveForwardHandler();
  _sendStatus("Forward"); 
}

void Webcontrol::_moveLeft() { 
  Serial.println("Webcontrol: Left"); 
  _moveLeftHandler();
  _sendStatus("Left"); 
}
  
void Webcontrol::_moveStop() { 
  Serial.println("Webcontrol: Stop"); 
  _moveStopHandler();
  _sendStatus("Stop"); 
}

void Webcontrol::_moveRight() { 
  Serial.println("Webcontrol: Right"); 
  _moveRightHandler();
  _sendStatus("Right"); 
}

void Webcontrol::_moveBackward() { 
  Serial.println("Webcontrol: Backward"); 
  _moveBackwardHandler();
  _sendStatus("Backward"); 
}
  
void Webcontrol::_enableAutomode() { 
  Serial.println("Webcontrol: AutoMode"); 
  _enableAutomodeHandler();
  _sendStatus("AutoMode"); 
}
