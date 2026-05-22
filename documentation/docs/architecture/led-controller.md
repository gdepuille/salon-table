# LedController

The core component driving the NeoPixel strip. It is called every **20 ms** from `loop()` via `process()`.

## State machine

```
        double-click left
SENSOR ──────────────────► ANIMATION ──► GAME ──► SENSOR
  ◄──────────────────────────────────────────────────┘
```

| Mode        | Value | Description                               |
|-------------|-------|-------------------------------------------|
| `SENSOR`    | 3     | Each zone lights up when touched (default)|
| `COLOR`     | 0     | Solid user-defined colour                 |
| `ANIMATION` | 1     | Built-in FastLED animations               |
| `GAME`      | 2     | Mini-games (Random Choose, …)             |

## Public API

```cpp
// Lifecycle
void setup();
void process(uint8_t sensorTouched);

// Enable / brightness
LedController* setEnabled(bool value);
LedController* setBrightness(uint8_t value);   // 0–255

// Mode
LedController* setMode(LedMode value);

// Color (MODE == COLOR)
LedController* setColor(CRGB value);

// Animation / Game index (MODE == ANIMATION or GAME)
LedController* setIndex(uint8_t value);

// Button callbacks (from main.cpp)
void btnLeftPressed();
void btnLeftDoublePressed();   // rotate mode
void btnRightPressed();

// OTA hooks
void otaStart();
void otaProgress(uint8_t percent);
void otaEnd();

// Boot animation
void startupGoogleHome();
```

## Button behaviour

| Button | Action       | Effect                                      |
|--------|-------------|---------------------------------------------|
| Left   | Single click | prev index in current mode                  |
| Left   | Double click | rotate `LedMode` circularly                 |
| Left   | Long press   | brightness − 5 (min 0)                      |
| Right  | Single click | next index in current mode                  |
| Right  | Long press   | brightness + 5 (max 255)                    |

## OTA visual feedback

When an OTA upload is in progress, `LedController` intercepts `process()` and drives the strip independently:

| State          | Strip                          |
|----------------|-------------------------------|
| `OTA_START`    | Black (brightness forced 50%) |
| `OTA_PROGRESS` | White bar growing left→right  |
| `OTA_END`      | Full green                    |

