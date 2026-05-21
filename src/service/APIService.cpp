//
// Created by Gregory DEPUILLE on 12/11/2025.
//

#include <service/APIService.h>

#include <ArduinoLog.h>
#include <AsyncJson.h>

void APIService::setup() const {
  Log.infoln("Register API :");

  // Register simple API handler
  Log.infoln("GET /infos");
  server->on("/infos", HTTP_GET, [](AsyncWebServerRequest *request) { handleGetInfos(request); });

  Log.infoln("GET /status");
  server->on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){ handleGetStatus(request); });

  Log.infoln("GET /animations");
  server->on("/animations", HTTP_GET, [this](AsyncWebServerRequest *request){ handleGetAnimations(request); });

  Log.infoln("GET /games");
  server->on("/games", HTTP_GET, [this](AsyncWebServerRequest *request){ handleGetGames(request); });

  // Register JSON handlers
  Log.infoln("POST /state");
  auto *postJsonState = new AsyncCallbackJsonWebHandler("/state", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostState(request, json); });
  postJsonState->setMethod(HTTP_POST);
  server->addHandler(postJsonState);

  Log.infoln("POST /sensor");
  auto *postJsonSensor = new AsyncCallbackJsonWebHandler("/sensor", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostSensor(request, json); });
  postJsonSensor->setMethod(HTTP_POST);
  server->addHandler(postJsonSensor);

  Log.infoln("POST /color");
  auto *postJsonColor = new AsyncCallbackJsonWebHandler("/color", [this](AsyncWebServerRequest *request, const JsonVariant &json) { handlePostColor(request, json); });
  postJsonColor->setMethod(HTTP_POST);
  server->addHandler(postJsonColor);

  Log.infoln("POST /animation");
  auto *postJsonAnimation = new AsyncCallbackJsonWebHandler("/animation", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostAnimation(request, json); });
  postJsonAnimation->setMethod(HTTP_POST);
  server->addHandler(postJsonAnimation);

  Log.infoln("POST /game");
  auto *postJsonGame = new AsyncCallbackJsonWebHandler("/game", [this](AsyncWebServerRequest *request, JsonVariant &json) { handlePostGame(request, json); });
  postJsonGame->setMethod(HTTP_POST);
  server->addHandler(postJsonGame);

  // Start server
  server->begin();
}

void APIService::handleGetInfos(AsyncWebServerRequest *request) {
  Log.infoln("GET /infos");
  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["name"] = "Table Salon";
  root["version"] = TIMESTAMP;
  root["hashGit"] = COMMIT_HASH;
  response->setLength();
  request->send(response);
}

void APIService::handleGetStatus(AsyncWebServerRequest *request) const {
  Log.infoln("GET /status");
  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();

  const auto state = root["state"].to<JsonObject>();
  state["enabled"] = ledController->isEnabled();
  state["intensity"] = static_cast<uint8_t>(100 * ledController->getBrightness() / 255);

  const LedMode mode = ledController->getMode();
  root["mode"] = mode;

  if (mode == SENSOR) {
    const auto data = root["sensor"].to<JsonObject>();
    data["touched"] = ledController->getSensorTouched();

  } else if (mode == COLOR) {
    const auto data = root["color"].to<JsonObject>();
    const auto color = ledController->getColor();
    data["red"] = color.red;
    data["green"] = color.green;
    data["blue"] = color.blue;
    data["pulse"] = ledController->isPulse();

  } else if (mode == ANIMATION) {
    const auto data = root["animation"].to<JsonObject>();
    data["id"] = ledController->getIndex();
    data["name"] = ledController->animationName();

  } else {
    const auto data = root["game"].to<JsonObject>();
    data["id"] = ledController->getIndex();
    data["name"] = "TO BE DEFINED";

  }

  response->setLength();
  request->send(response);
}

void APIService::handleGetAnimations(AsyncWebServerRequest *request) const {
  Log.infoln("GET /animations");
  auto *response = new AsyncJsonResponse();
  const JsonArray array = response->getRoot().to<JsonArray>();

  for (int i = 0; i < ledController->animationNames.size(); i++) {
    const JsonObject object = array[i].to<JsonObject>();
    object["id"] = i;
    object["name"] = ledController->animationNames[i];
  }

  response->setLength();
  request->send(response);
}

void APIService::handleGetGames(AsyncWebServerRequest *request) const {
  Log.infoln("GET /games");
  auto *response = new AsyncJsonResponse();
  const JsonArray array = response->getRoot().to<JsonArray>();

  for (int i = 0; i < ledController->gameNames.size(); i++) {
    const JsonObject object = array[i].to<JsonObject>();
    object["id"] = i;
    object["name"] = ledController->gameNames[i];
  }

  response->setLength();
  request->send(response);
}

void APIService::handlePostState(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /state : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setEnabled(jsonObj["enabled"]);
  uint8_t brightness = jsonObj["intensity"];
  ledController->setBrightness(brightness * 255 / 100);

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = true;
  response->setLength();
  request->send(response);
}

void APIService::handlePostSensor(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /sensor : %s", jsonString.c_str());

  ledController->setMode(SENSOR);

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = true;
  response->setLength();
  request->send(response);
}

void APIService::handlePostColor(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /color : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(COLOR);
  ledController->setPulse(jsonObj["pulse"]);

  const uint8_t red = jsonObj["red"];
  const uint8_t green = jsonObj["green"];
  const uint8_t blue = jsonObj["blue"];
  ledController->setColor(CRGB(red, green, blue));

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = true;
  response->setLength();
  request->send(response);
}

void APIService::handlePostAnimation(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /animation : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(ANIMATION);
  ledController->setIndex(jsonObj["id"]);
  Log.infoln(" - Animation name : %s", ledController->animationName().c_str());

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = true;
  response->setLength();
  request->send(response);
}

void APIService::handlePostGame(AsyncWebServerRequest *request, const JsonVariant &json) const {
  String jsonString = json.as<String>();
  serializeJson(json, jsonString);
  Log.infoln("POST /game : %s", jsonString.c_str());

  auto jsonObj = json.as<JsonObject>();
  ledController->setMode(GAME);
  ledController->setIndex(jsonObj["id"]);
  Log.infoln(" - Game name : %s", "TO BE DEFINED");

  auto *response = new AsyncJsonResponse();
  const JsonObject root = response->getRoot().to<JsonObject>();
  root["ack"] = true;
  response->setLength();
  request->send(response);
}
