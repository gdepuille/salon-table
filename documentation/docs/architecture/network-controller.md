# NetworkController

Manages Wi-Fi connection and OTA (Over-The-Air) firmware updates.

## Responsibilities

- Connect to a Wi-Fi network and set the mDNS hostname.
- Register ArduinoOTA callbacks and start the OTA service.
- Forward OTA lifecycle events to `LedController` for visual feedback.
- Call `ArduinoOTA.handle()` on every loop iteration.

## Public API

```cpp
// Constructor
NetworkController(const String& hostname);

// Connect to Wi-Fi (blocking until connected)
void connectWifi(const String& ssid, const String& password);

// Enable OTA with a password
void enableOTA(const String& otaPassword);

// Must be called in loop()
void process();

// Bind a LedController to drive OTA visual feedback
void setLedController(LedController* led);
```

## OTA event flow

```
ArduinoOTA.onStart     → led->otaStart()
                       → led->process(0)   ← called from the callback
ArduinoOTA.onProgress  → led->otaProgress(pct)
                       → led->process(0)
ArduinoOTA.onEnd       → led->otaEnd()
                       → led->process(0)
                       → board reboots
```

!!! tip
    `led->process()` must be called inside the OTA callbacks to drive `FastLED.show()` during the upload, otherwise the strip stays frozen.

