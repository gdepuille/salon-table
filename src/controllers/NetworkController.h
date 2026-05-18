//
// Created by Gregory DEPUILLE on 11/11/2025.
//

#ifndef SALON_TABLE_OTACONTROLLER_H
#define SALON_TABLE_OTACONTROLLER_H

#include <WString.h>
#include <utility>

#include "LedController.h"

class NetworkController {

public:
  explicit NetworkController(String hostname) {
    this->hostname = std::move(hostname);
  }

  void connectWifi(const String& ssid, const String& password);
  void enableOTA(const String& otaPassword);
  void process();

  void setLedController(LedController* value);

private:
  LedController* ledController = nullptr;

  String hostname;

  bool otaEnabled = false;
};


#endif //SALON_TABLE_OTACONTROLLER_H