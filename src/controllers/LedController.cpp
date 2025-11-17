//
// Created by Gregory DEPUILLE on 15/11/2025.
//

#include <ArduinoLog.h>

#include <controllers/LedController.h>

void LedController::setup() {
  Log.infoln("Led strip configuration");
  CFastLED::addLeds<NEOPIXEL, LEDS_DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(brightness);
}

void LedController::process() {

  if (ledEnabled) {
    if (ledMode == SIMPLE) {
      fill_solid(leds, NUM_LEDS, color);

    } else if (ledMode == ANIMATION) {
      hue++;
      callAnnimation();

    } else if (ledMode == GAME) {
      //gamePattern[gameIndex]();

    }

  } else {
    fadeToBlackBy(leds, NUM_LEDS, 25);
  }

  FastLED.setBrightness(brightness);
  FastLED.show();
}

void LedController::setEnabled(const bool value) {
  if (value == ledEnabled)
    return;

  Log.infoln("Change enabled : %d -> %d", ledEnabled, value);
  ledEnabled = value;
}

void LedController::setMode(const LedMode value) {
  if (value == ledMode)
    return;

  Log.infoln("Change mode : %d -> %d", ledMode, value);
  ledMode = value;
}

void LedController::setColor(const CRGB value) {
  if (value == color)
    return;

  Log.infoln("Change color : %s -> %s", color.toString().c_str(), value.toString().c_str());
  color = value;
}

void LedController::setBrightness(const uint8_t value) {
  if (value == brightness)
    return;

  Log.infoln("Change brightness : %d -> %d", brightness, value);
  brightness = value;
}

void LedController::setIndex(const uint8_t value) {
  if (value == index)
    return;

  Log.infoln("Change index : %d -> %d", this->index, value);
  this->index = value;
}

void LedController::callAnnimation() {

  if (index < 0) index = 0;
  if (index > 5) index = 5;

  // Appel dynamique
  switch (index) {
    case 0: rainbow(); break;
    case 1: rainbowWithGlitter(); break;
    case 2: confetti(); break;
    case 3: sinelon(); break;
    case 4: bpm(); break;
    case 5:
    default: juggle(); break;
  }
}


// --------- //
// ANIMATION //
// --------- //

void LedController::rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, hue, 7);
}

void LedController::rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void LedController::confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void LedController::sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(hue, 255, 192);
}

void LedController::bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
}

void LedController::juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// ---- //
// Game //
// ---- //


// ----- //
// Utils //
// ----- //

void LedController::addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}