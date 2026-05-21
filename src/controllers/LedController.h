//
// Created by Gregory DEPUILLE on 15/11/2025.
//

#ifndef SALON_TABLE_LEDCONTROLLER_H
#define SALON_TABLE_LEDCONTROLLER_H

#include <vector>

#include <FastLED.h>

// Leds
#define LEDS_DATA_PIN D2
#define LEDS_WIDHT 56
#define LEDS_HEIGHT 19
#define NUM_LEDS ((LEDS_WIDHT * 2) + (LEDS_HEIGHT * 2))

enum LedMode {
  COLOR = 0,
  ANIMATION = 1,
  GAME = 2,
  SENSOR = 3,
};

enum LedAnimation {
  RAINBOW = 0,
  RAINBOW_GLITTER = 1,
  JUGGLE = 2,
  SINELON = 3,
  CONFETTI = 4,
  BPM = 5
};

class LedController {
public:
  LedController() = default;

  void setup();
  void process(uint8_t sensorTouched);

  String animationName();
  String gameName();

  std::vector<String> animationNames = {
    "Rainbow", "Rainbow (glitter)", "Confetti",
    "Sinelon", "BPM","Juggle"
  };

  std::vector<String> gameNames = {
    "Led Runner", "Random Choose"
  };

  LedController* setEnabled(bool value);
  bool isEnabled() const;

  LedController* setPulse(bool value);
  bool isPulse() const;

  LedController* setBrightness(uint8_t value);
  uint8_t getBrightness() const;

  LedController* setMode(LedMode value);
  LedMode getMode() const;

  LedController* setColor(CRGB value);
  CRGB getColor() const;

  LedController* setIndex(uint8_t value);
  uint8_t getIndex() const;

  uint8_t getSensorTouched() const;

  void otaStart();
  void otaProgress(uint8_t percent);
  void otaEnd();

private:
  CRGB leds[NUM_LEDS];

  bool enabled = true;
  bool pulse = false;
  LedMode ledMode = SENSOR;
  uint8_t index = 0;

  uint8_t hue = 0;
  uint8_t brightness = 180;
  CRGB color = CRGB::White;

  uint8_t sensorTouched = 0;

  enum OtaState : uint8_t {
    OTA_IDLE = 0,
    OTA_START = 1,
    OTA_PROGRESS = 2,
    OTA_END = 3
  };

  OtaState otaState = OTA_IDLE;
  uint8_t otaPercent = 0;

  void callAnnimation();
  void callGame();
  void updateSensorLeds();
  void updateOtaLeds();
  void checkIndex();

  void animateRainbow();
  void animateRainbowWithGlitter();
  void animateJuggle();
  void animateSinelon();
  void animateConfetti();
  void animateBpm();

  void gameLedRunner();
  void gameRandomChoose();

  void addGlitter(fract8 chanceOfGlitter);
};


#endif //SALON_TABLE_LEDCONTROLLER_H