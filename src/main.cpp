#include <Adafruit_CAP1188.h>
#include <Arduino.h>
#include <ArduinoLog.h>
#include <I2CScanner.h>
#include <OneButton.h>

#include <service/APIService.h>
#include <controllers/NetworkController.h>
#include <controllers/LedController.h>

#include "common.secrets.h"

auto i2cScanner = I2CScanner();
auto network = NetworkController(HOSTNAME);
auto led = LedController();
auto api = APIService(&led);

// Capacitive sensor
auto cap = Adafruit_CAP1188();
auto cap_present = false;

// Buttons
auto btnLeft = OneButton(D4, true, true);
auto btnLeftClicked = false;
auto btnRight = OneButton(D5, true, true);
auto btnRightClicked = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
#if defined(DEBUG)
  Log.begin(LOG_LEVEL_DEBUG, &Serial, true, false);
#else
  Log.begin(LOG_LEVEL_INFO, &Serial, true, false);
#endif

  Log.infoln("Start table salon !");
  Log.infoln("Version: %s-%s", TIMESTAMP, COMMIT_HASH);

  i2cScanner.Init();
  int nbDevices = i2cScanner.Scan();
  Log.infoln("Scanned %d devices", nbDevices);
  if (nbDevices != 1) {
    Log.errorln("Only one device is allowed");
  }

  network.connectWifi(WIFI_SSID, WIFI_PASS);
  led.setup();
  network.setLedController(&led);
  network.enableOTA(OTA_PASSWORD);
  api.setup();

  Log.infoln("IO Configuration");
  pinMode(LEDS_DATA_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Init buttons
  btnLeft.attachClick([]() { btnLeftClicked = !btnLeftClicked; });
  btnRight.attachClick([]() { btnRightClicked = !btnRightClicked; });

  Log.infoln("Capacitive sensor configuration");
  if (!cap.begin()) {
    Log.warningln("CAP1188 not found");

  } else {
    cap_present = true;
    Log.infoln("CAP1188 found");
  }
}

void loop() {
  network.process();
  btnLeft.tick();
  btnRight.tick();

  EVERY_N_MILLIS(1000) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  EVERY_N_MILLIS(20) {
    auto touched = cap.touched();
    if (touched == 0) {
      if (btnLeftClicked) {
        touched |= 1 << 0;
      }
      if (btnRightClicked) {
        touched |= 1 << 1;
      }
    }
    led.process(touched);
  }
}