//
// Created by Gregory DEPUILLE on 15/11/2025.
//

#ifndef SALON_TABLE_LEDCONTROLLER_H
#define SALON_TABLE_LEDCONTROLLER_H

#include <FastLED.h>

// Leds
#define LEDS_DATA_PIN D6
#define LEDS_WIDHT 56
#define LEDS_HEIGHT 19
#define NUM_LEDS ((LEDS_WIDHT * 2) + (LEDS_HEIGHT * 2))

enum LedMode {
  SIMPLE = 0,
  ANIMATION = 1,
  GAME = 2,
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
  void process();

  void setEnabled(bool value);
  void setMode(LedMode value);
  void setColor(CRGB value);
  void setBrightness(uint8_t value);
  void setIndex(uint8_t value);

private:
  CRGB leds[NUM_LEDS];

  bool ledEnabled = true;
  LedMode ledMode = ANIMATION;
  uint8_t index = 0;

  uint8_t hue = 0;
  uint8_t brightness = 20;
  CRGB color = CRGB::White;

  void callAnnimation();

  void rainbow();
  void rainbowWithGlitter();
  void juggle();
  void sinelon();
  void confetti();
  void bpm();

  void addGlitter(fract8 chanceOfGlitter);
};


#endif //SALON_TABLE_LEDCONTROLLER_H