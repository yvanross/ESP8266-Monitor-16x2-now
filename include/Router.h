#ifndef Router_h
#define Router_h

#include "Arduino.h"
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <LittleFS.h> 
#include <DeviceController.h>

class Router {
  public: 
    Router();
    void loop();
    void startWifi();

  private:

  void enableRoutes();
  AsyncWebServer* server;
};

#endif
