//
// Created by Gregory DEPUILLE on 12/11/2025.
//

#include <service/APIService.h>

#include <ArduinoLog.h>
#include <AsyncJson.h>

void APIService::setup() {
  // Register simple API handler
  server->on("/infos", HTTP_GET, [](AsyncWebServerRequest *request) { handleGetInfos(request); });
  server->on("/status", HTTP_GET, [](AsyncWebServerRequest *request){ handleGetStatus(request); });

  // Register JSON handlers
  auto *postJsonSimple = new AsyncCallbackJsonWebHandler("/mode/simple", [this](AsyncWebServerRequest *request, const JsonVariant &json) { handlePostModeSimple(request, json); });
  postJsonSimple->setMethod(HTTP_POST);
  server->addHandler(postJsonSimple);

  auto *postJsonAnimation = new AsyncCallbackJsonWebHandler("/mode/animation", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostModeAnimation(request, json); });
  postJsonAnimation->setMethod(HTTP_POST);
  server->addHandler(postJsonAnimation);

  auto *postJsonGame = new AsyncCallbackJsonWebHandler("/mode/game", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostModeGame(request, json); });
  postJsonGame->setMethod(HTTP_POST);
  server->addHandler(postJsonGame);

  // Start server
  server->begin();
}

void APIService::handleGetInfos(AsyncWebServerRequest *request) {
  Log.infoln("GET /infos");
  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["name"] = "table-salon";
  root["version"] = TIMESTAMP;
  root["git_hash"] = COMMIT_HASH;
  response->setLength();
  request->send(response);
}

void APIService::handleGetStatus(AsyncWebServerRequest *request) {
  Log.infoln("GET /status");
  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["enabled"] = true;
  root["mode"] = "SIMPLE";
  auto color = JsonObject();
  color["red"] = 0;
  color["green"] = 0;
  color["blue"] = 0;
  root["color"] = color;
  root["intensity"] = static_cast<uint8_t>(100 * 51 / 255);
  response->setLength();
  request->send(response);
}

void APIService::handlePostModeSimple(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /mode/simple : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(SIMPLE);
  ledController->setEnabled(jsonObj["enabled"]);

  const uint8_t red = jsonObj["red"];
  const uint8_t green = jsonObj["green"];
  const uint8_t blue = jsonObj["blue"];
  ledController->setColor(CRGB(red, green, blue));

  uint8_t brightness = jsonObj["intensity"];
  ledController->setBrightness(brightness * 255 / 100);

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = "OK";
  response->setLength();
  request->send(response);
}

void APIService::handlePostModeAnimation(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /mode/animation : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(ANIMATION);
  ledController->setEnabled(jsonObj["enabled"]);
  ledController->setIndex(jsonObj["animIdx"]);

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = "OK";
  response->setLength();
  request->send(response);
}

void APIService::handlePostModeGame(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /mode/game : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(GAME);
  ledController->setEnabled(jsonObj["enabled"]);


  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = "OK";
  response->setLength();
  request->send(response);
}
