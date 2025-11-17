//
// Created by Gregory DEPUILLE on 12/11/2025.
//

#ifndef SALON_TABLE_APISERVICE_H
#define SALON_TABLE_APISERVICE_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include <controllers/LedController.h>


class APIService {

public:
  explicit APIService(LedController *ledController) {
    this->server = new AsyncWebServer(80);
    this->ledController = ledController;
  }

  ~APIService() {
    delete this->server;
    delete this->ledController;
  }

  void setup();
private:
  AsyncWebServer *server;
  LedController *ledController;

  static void handleGetInfos(AsyncWebServerRequest *request);
  static void handleGetStatus(AsyncWebServerRequest *request);

  void handlePostModeSimple(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostModeAnimation(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostModeGame(AsyncWebServerRequest *request, const JsonVariant &json) const;
};


#endif //SALON_TABLE_APISERVICE_H