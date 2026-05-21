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

void LedController::process(uint8_t sensorTouched) {
  if (otaState != OTA_IDLE) {
    updateOtaLeds();
    FastLED.setBrightness(brightness);
    FastLED.show();
    return;
  }

  this->sensorTouched = sensorTouched;

  if (enabled) {
    if (ledMode == COLOR) {
      fill_solid(leds, NUM_LEDS, color);

    } else if (ledMode == ANIMATION) {
      hue++;
      callAnnimation();

    } else if (ledMode == GAME) {
      callGame();

    } else if (ledMode == SENSOR) {
      updateSensorLeds();

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

uint8_t LedController::getSensorTouched() const {
  return sensorTouched;
}

void LedController::otaStart() {
  otaPercent = 0;
  otaState = OTA_START;
}

void LedController::otaProgress(uint8_t percent) {
  if (percent > 100) {
    percent = 100;
  }
  otaPercent = percent;
  otaState = OTA_PROGRESS;
}

void LedController::otaEnd() {
  otaPercent = 100;
  otaState = OTA_END;
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

void LedController::updateSensorLeds() {
  constexpr uint8_t kBitCount = 8;
  constexpr uint8_t kFadeStep = 40;

  // Mapping physique: 56 LEDs (1,2,3), 19 LEDs (4), 56 LEDs (5,6,7), 19 LEDs (8).
  // Les gaps de 5 LEDs ne sont appliques que dans les zones 1-3 et 5-7.
  constexpr uint16_t kSensorStart[kBitCount] = {
    0,   // Sensor 1
    21,  // Sensor 2 (gap 16-20)
    41,  // Sensor 3 (gap 36-40)
    56,  // Sensor 4 (19 LEDs)
    75,  // Sensor 5
    96,  // Sensor 6 (gap 91-95)
    116, // Sensor 7 (gap 111-115)
    131  // Sensor 8 (19 LEDs)
  };

  constexpr uint8_t kSensorLength[kBitCount] = {
    16, 15, 15, 19, 16, 15, 15, 19
  };

  const CRGB bitColors[kBitCount] = {
    CRGB::Red,
    CRGB::Blue,
    CRGB::Green,
    CRGB::Purple,
    CRGB::Yellow,
    CRGB::Orange,
    CRGB::Aqua,
    CRGB::White
  };

  CRGB targetLeds[NUM_LEDS];
  fill_solid(targetLeds, NUM_LEDS, CRGB::Black);

  for (uint8_t bit = 0; bit < kBitCount; ++bit) {
    if ((sensorTouched & (1 << bit)) == 0) {
      continue;
    }

    const int start = kSensorStart[bit];
    int end = start + kSensorLength[bit];
    if (end > NUM_LEDS) {
      end = NUM_LEDS;
    }
    for (int led = start; led < end; ++led) {
      targetLeds[led] = bitColors[bit];
    }
  }

  // Interpole vers l'etat cible pour obtenir un fade in/out fluide.
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = blend(leds[i], targetLeds[i], kFadeStep);
  }
}

void LedController::updateOtaLeds() {
  if (otaState == OTA_START) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    this->setBrightness(128); // 50% to not blind people during OTA
    return;
  }

  if (otaState == OTA_PROGRESS) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    int greenCount = (NUM_LEDS * otaPercent) / 100;
    if (greenCount > NUM_LEDS) {
      greenCount = NUM_LEDS;
    }

    for (int i = 0; i < greenCount; ++i) {
      leds[i] = CRGB::White;
    }
    return;
  }

  if (otaState == OTA_END) {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
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

void LedController::checkIndex() {
  uint8_t max = 0;
  if (ledMode == ANIMATION) max = animationNames.size() - 1;
  else if (ledMode == GAME) max = 1;

  if (index < 0) index = 0;
  if (index > max) index = max;
}
