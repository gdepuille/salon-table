//
// Created by Gregory DEPUILLE on 11/11/2025.
//

#include <controllers/NetworkController.h>
#include <controllers/LedController.h>

#include <ArduinoLog.h>
#include <ArduinoOTA.h>

void NetworkController::connectWifi(const String& ssid, const String& password) {
  Log.infoln("Connecting to WiFi");
  WiFiClass::setHostname(this->hostname.c_str());
  WiFi.begin(ssid, password);
  while(WiFiClass::status() != WL_CONNECTED) {
    Log.info(".");
    delay(1000);
  }
  Log.infoln(".");

  Log.infoln("Wifi OK");
  Log.infoln("SSID: %s", WiFi.SSID().c_str());
  Log.infoln("IP Address: %s", WiFi.localIP().toString().c_str());
  Log.infoln("Signal strength (RSSI): %d dBm", WiFi.RSSI());
}

void NetworkController::enableOTA(const String& otaPassword) {
  ArduinoOTA.setHostname(WiFiClass::getHostname());
  ArduinoOTA.setPassword(otaPassword.c_str());

  ArduinoOTA.onStart([this]() -> void {
    Log.infoln("Start OTA");
    if (this->ledController != nullptr) {
      this->ledController->otaStart();
      this->ledController->process(0);
    }
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  });

  ArduinoOTA.onEnd([this]() -> void {
    Log.infoln("End OTA");
    if (this->ledController != nullptr) {
      this->ledController->otaEnd();
      this->ledController->process(0);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
  });

  ArduinoOTA.onProgress([this](int current, int total) -> void {
    int pct = 0;
    if (total > 0) {
      pct = (current * 100) / total;
    }
    if (pct > 100) {
      pct = 100;
    }
    Log.infoln("Upgrade %d %%", pct);

    if (this->ledController != nullptr) {
      this->ledController->otaProgress(static_cast<uint8_t>(pct));
      this->ledController->process(0);
    }

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    if (pct > 40) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    if (pct > 60) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    if (pct > 90) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  });

  ArduinoOTA.begin();
  this->otaEnabled = true;

  Log.infoln("OTA is enabled");
}

void NetworkController::setLedController(LedController* value) {
  this->ledController = value;
}

void NetworkController::process() {
  if (this->otaEnabled) {
    ArduinoOTA.handle();
  }
}
