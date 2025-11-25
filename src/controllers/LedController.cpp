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

  if (enabled) {
    if (ledMode == COLOR) {
      fill_solid(leds, NUM_LEDS, color);

    } else if (ledMode == ANIMATION) {
      hue++;
      callAnnimation();

    } else if (ledMode == GAME) {
      callGame();

    }

  } else {
    fadeToBlackBy(leds, NUM_LEDS, 25);
  }

  // TODO Pulse
  FastLED.setBrightness(brightness);
  FastLED.show();
}

LedController* LedController::setEnabled(const bool value) {
  if (value == enabled)
    return this;

  Log.infoln("Change enabled : %d -> %d", enabled, value);
  enabled = value;
  return this;
}

bool LedController::isEnabled() const {
  return this->enabled;
}

LedController* LedController::setPulse(const bool value) {
  if (value == pulse)
    return this;

  Log.infoln("Change pulse : %d -> %d", pulse, value);
  pulse = value;
  return this;
}

bool LedController::isPulse() const {
  return this->pulse;
}

LedController* LedController::setMode(const LedMode value) {
  if (value == ledMode)
    return this;

  Log.infoln("Change mode : %d -> %d", ledMode, value);
  ledMode = value;
  return this;
}

LedMode LedController::getMode() const {
  return ledMode;
}

LedController* LedController::setColor(const CRGB value) {
  if (value == color)
    return this;

  Log.infoln("Change color : %s -> %s", color.toString().c_str(), value.toString().c_str());
  color = value;
  return this;
}

CRGB LedController::getColor() const {
  return color;
}

LedController* LedController::setBrightness(const uint8_t value) {
  if (value == brightness)
    return this;

  Log.infoln("Change brightness : %d -> %d", brightness, value);
  brightness = value;
  return this;
}

uint8_t LedController::getBrightness() const {
  return brightness;
}

LedController* LedController::setIndex(const uint8_t value) {
  if (value == index)
    return this;

  Log.infoln("Change index : %d -> %d", this->index, value);
  this->index = value;
  return this;
}

uint8_t LedController::getIndex() const {
  return index;
}

void LedController::callAnnimation() {
  checkIndex();
  switch (index) { // Appel dynamique
    case 0: rainbow(); break;
    case 1: rainbowWithGlitter(); break;
    case 2: confetti(); break;
    case 3: sinelon(); break;
    case 4: bpm(); break;
    case 5:
    default: juggle();
  }
}

void LedController::callGame() {
  checkIndex();
  // NOPE
}

String LedController::animationName() {
  checkIndex();
  return animationNames[index];
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

void LedController::checkIndex() {
  uint8_t max = 0;
  if (ledMode == ANIMATION) max = animationNames.size() - 1;
  else if (ledMode == GAME) max = 1;

  if (index < 0) index = 0;
  if (index > max) index = max;
}
