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

  void setup() const;
private:
  AsyncWebServer *server;
  LedController *ledController;

  static void handleGetInfos(AsyncWebServerRequest *request);
  void handleGetStatus(AsyncWebServerRequest *request) const;

  void handleGetAnimations(AsyncWebServerRequest *request) const;
  static void handleGetGames(AsyncWebServerRequest *request);

  void handlePostState(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostSensor(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostColor(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostAnimation(AsyncWebServerRequest *request, const JsonVariant &json) const;
  void handlePostGame(AsyncWebServerRequest *request, const JsonVariant &json) const;
};


#endif //SALON_TABLE_APISERVICE_H