//
// Created by Gregory DEPUILLE on 11/11/2025.
//

#include <controllers/NetworkController.h>

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

  ArduinoOTA.onStart([]() -> void {
    Log.infoln("Start OTA");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  });

  ArduinoOTA.onEnd([]() -> void {
    Log.infoln("End OTA");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
  });
  ArduinoOTA.onProgress([](int current, int total) -> void {
    const int pct = (current / (total / 100));
    Log.infoln("Upgrade %d %%", pct);

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

void NetworkController::process() {
  if (this->otaEnabled) {
    ArduinoOTA.handle();
  }
}
