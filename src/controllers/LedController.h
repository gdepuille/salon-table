//
// Created by Gregory DEPUILLE on 15/11/2025.
//

#ifndef SALON_TABLE_LEDCONTROLLER_H
#define SALON_TABLE_LEDCONTROLLER_H

#include <vector>

#include <FastLED.h>

// Leds
#define LEDS_DATA_PIN D6
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
  void process(uint8_t senorTouched);

  String animationName();

  std::vector<String> animationNames = {
    "Rainbow", "Rainbow (glitter)", "Confetti",
    "Sinelon", "BPM","Juggle"
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

private:
  CRGB leds[NUM_LEDS];

  bool enabled = true;
  bool pulse = false;
  LedMode ledMode = SENSOR;
  uint8_t index = 0;

  uint8_t hue = 0;
  uint8_t brightness = 180;
  CRGB color = CRGB::White;


  void callAnnimation();
  void callGame();
  void updateSensorLeds(uint8_t sensorTouched);
  void checkIndex();

  void rainbow();
  void rainbowWithGlitter();
  void juggle();
  void sinelon();
  void confetti();
  void bpm();

  void addGlitter(fract8 chanceOfGlitter);
};


#endif //SALON_TABLE_LEDCONTROLLER_H