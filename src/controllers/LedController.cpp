//
// Created by Gregory DEPUILLE on 15/11/2025.
//

#include <ArduinoLog.h>

#include <controllers/LedController.h>

namespace {
constexpr uint8_t kSensorCount = 8;
constexpr uint16_t kSensorStart[kSensorCount] = {
  0,   // Sensor 1
  21,  // Sensor 2 (gap 16-20)
  41,  // Sensor 3 (gap 36-40)
  56,  // Sensor 4 (19 LEDs)
  75,  // Sensor 5
  96,  // Sensor 6 (gap 91-95)
  116, // Sensor 7 (gap 111-115)
  131  // Sensor 8 (19 LEDs)
};

constexpr uint8_t kSensorLength[kSensorCount] = {
  16, 15, 15, 19, 16, 15, 15, 19
};

constexpr CRGB kSensorColors[kSensorCount] = {
  CRGB::Red,
  CRGB::Blue,
  CRGB::Green,
  CRGB::Purple,
  CRGB::Yellow,
  CRGB::Orange,
  CRGB::Aqua,
  CRGB::White
};

uint16_t wrapLedIndex(int32_t value) {
  while (value < 0) {
    value += NUM_LEDS;
  }
  return static_cast<uint16_t>(value % NUM_LEDS);
}

uint16_t sensorCenter(const uint8_t sensorIndex) {
  return kSensorStart[sensorIndex] + (kSensorLength[sensorIndex] / 2);
}
}

void LedController::setup() {
  Log.infoln("Led strip configuration");
  CFastLED::addLeds<NEOPIXEL, LEDS_DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(brightness);
}

void LedController::process(const uint8_t value) {
  if (otaState != OTA_IDLE) {
    updateOtaLeds();
    FastLED.setBrightness(brightness);
    FastLED.show();
    return;
  }

  this->sensorTouched = value;

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

  // Reset btnPressed
  btnLeft = false;
  btnRight = false;
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
  if (ledMode == GAME) {
    gameState = GAME_IDLE;
    resetSensorRace();
  }
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
  if (ledMode == GAME) {
    gameState = GAME_IDLE;
    resetSensorRace();
  }
  return this;
}

uint8_t LedController::getIndex() const {
  return index;
}

uint8_t LedController::getSensorTouched() const {
  return sensorTouched;
}

void LedController::btnLeftPressed() {
  Log.infoln("Button left pressed");
  btnLeft = true;
}

void LedController::btnLeftDoublePressed() {
  switch (getMode()) {
    case SENSOR: setMode(ANIMATION); break;
    case ANIMATION: setMode(GAME); break;
    case GAME:
    case COLOR:
    default: setMode(SENSOR); break;
  }
}

void LedController::btnRightPressed() {
  Log.infoln("Button right pressed");
  btnRight = true;
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

void LedController::startupGoogleHome() {
  const CRGB googleColors[4] = {
    CRGB(66, 133, 244),  // Blue
    CRGB(234, 67, 53),   // Red
    CRGB(251, 188, 5),   // Yellow
    CRGB(52, 168, 83)    // Green
  };

  Log.infoln("Startup animation: Google Home style");

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.setBrightness(brightness);
  FastLED.show();
  delay(80);

  const uint16_t spacing = NUM_LEDS / 4;
  for (int step = 0; step < (NUM_LEDS * 2); ++step) {
    fadeToBlackBy(leds, NUM_LEDS, 85);
    for (uint8_t i = 0; i < 4; ++i) {
      const uint16_t pos = (step + (i * spacing)) % NUM_LEDS;
      leds[pos] = googleColors[i];
    }
    FastLED.show();
    delay(14);
  }

  for (uint8_t fade = 0; fade < 8; ++fade) {
    fadeToBlackBy(leds, NUM_LEDS, 48);
    FastLED.show();
    delay(22);
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void LedController::callAnnimation() {
  if (btnLeft) {
    index--;
  }
  if (btnRight) {
    index++;
  }

  checkIndex();
  switch (index) { // Appel dynamique
    case 0: animateRainbow(); break;
    case 1: animateRainbowWithGlitter(); break;
    case 2: animateConfetti(); break;
    case 3: animateSinelon(); break;
    case 4: animateBpm(); break;
    case 5: animateJuggle(); break;
    case 6: animateTwinkleWarm(); break;
    case 7: animateFireplace(); break;
    case 8: animateOceanWave(); break;
    case 9: animateMeteorRain(); break;
    case 10: animateCylonSweep(); break;
    case 11: animateTheaterChase(); break;
    case 12: animateSparkleBurst(); break;
    case 13: animateLarsonMulti(); break;
    case 14: animatePlasma(); break;
    case 15:
    default: animatePaletteFlow();
  }
}

void LedController::callGame() {
  if (btnLeft) {
    index--;
  }
  if (btnRight) {
    index++;
  }

  checkIndex();
  switch (index) {
    case 0: gameRandomChoose(); break;
    case 1: gameSensorRace(); break;
    default: break;
  }
}

String LedController::animationName() {
  checkIndex();
  return animationNames[index];
}

String LedController::gameName() {
  checkIndex();
  return gameNames[index];
}

void LedController::updateSensorLeds() {
  constexpr uint8_t kFadeStep = 40;

  CRGB targetLeds[NUM_LEDS];
  fill_solid(targetLeds, NUM_LEDS, CRGB::Black);

  for (uint8_t bit = 0; bit < kSensorCount; ++bit) {
    if ((sensorTouched & (1 << bit)) == 0) {
      continue;
    }

    const int start = kSensorStart[bit];
    int end = start + kSensorLength[bit];
    if (end > NUM_LEDS) {
      end = NUM_LEDS;
    }
    for (int led = start; led < end; ++led) {
      targetLeds[led] = kSensorColors[bit];
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

void LedController::animateRainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, hue, 7);
}

void LedController::animateRainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  animateRainbow();
  addGlitter(80);
}

void LedController::animateConfetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  const int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void LedController::animateSinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  const int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(hue, 255, 192);
}

void LedController::animateBpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
}

void LedController::animateJuggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void LedController::animateTwinkleWarm() {
  fadeToBlackBy(leds, NUM_LEDS, 18);
  if (random8() < 90) {
    leds[random16(NUM_LEDS)] += CHSV(20 + random8(22), 170, 220 + random8(35));
  }
}

void LedController::animateFireplace() {
  static uint16_t heat[NUM_LEDS];

  for (int i = 0; i < NUM_LEDS; ++i) {
    heat[i] = qsub8(heat[i], random8(0, 24));
  }

  for (int k = NUM_LEDS - 1; k >= 2; --k) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  if (random8() < 120) {
    const uint8_t y = random8(7);
    heat[y] = qadd8(heat[y], random8(120, 255));
  }

  for (int j = 0; j < NUM_LEDS; ++j) {
    const uint8_t colorIndex = scale8(static_cast<uint8_t>(heat[j]), 240);
    leds[j] = ColorFromPalette(HeatColors_p, colorIndex);
  }
}

void LedController::animateOceanWave() {
  const CRGBPalette16 oceanPalette = CRGBPalette16(
    CRGB(0, 6, 20), CRGB(0, 30, 80), CRGB(0, 90, 170), CRGB(20, 160, 220)
  );
  const uint8_t waveA = beatsin8(9, 0, 255);
  const uint8_t waveB = beatsin8(13, 0, 255);

  for (int i = 0; i < NUM_LEDS; ++i) {
    const uint8_t idx = hue + (i * 3) + waveA + scale8(waveB, i);
    leds[i] = ColorFromPalette(oceanPalette, idx, 180);
  }
}

void LedController::animateMeteorRain() {
  static int16_t head = 0;
  static int8_t direction = 1;
  constexpr uint8_t kTail = 20;

  fadeToBlackBy(leds, NUM_LEDS, 40);

  for (uint8_t i = 0; i < kTail; ++i) {
    const uint16_t pos = wrapLedIndex(head - (direction * i));
    const uint8_t value = 255 - (i * (255 / kTail));
    leds[pos] += CHSV(hue + i * 4, 220, value);
  }

  head += direction;
  if (head >= NUM_LEDS + static_cast<int16_t>(kTail)) {
    direction = -1;
    head = NUM_LEDS - 1;
  }
  if (head < -static_cast<int16_t>(kTail)) {
    direction = 1;
    head = 0;
  }
}

void LedController::animateCylonSweep() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  const int pos = beatsin16(18, 0, NUM_LEDS - 1);
  leds[pos] = CHSV(0, 255, 255);
  leds[wrapLedIndex(pos - 1)] += CHSV(0, 255, 128);
  leds[wrapLedIndex(pos + 1)] += CHSV(0, 255, 128);
}

void LedController::animateTheaterChase() {
  static uint8_t step = 0;
  fadeToBlackBy(leds, NUM_LEDS, 80);
  for (int i = step; i < NUM_LEDS; i += 3) {
    leds[i] = CHSV(hue + i * 2, 220, 255);
  }
  step = (step + 1) % 3;
}

void LedController::animateSparkleBurst() {
  fadeToBlackBy(leds, NUM_LEDS, 30);
  if (random8() < 40) {
    const uint16_t center = random16(NUM_LEDS);
    leds[center] = CHSV(hue + random8(64), 150, 255);
    leds[wrapLedIndex(center - 1)] += CHSV(hue + 20, 180, 220);
    leds[wrapLedIndex(center + 1)] += CHSV(hue + 20, 180, 220);
  }
}

void LedController::animateLarsonMulti() {
  fadeToBlackBy(leds, NUM_LEDS, 45);
  const int p1 = beatsin16(11, 0, NUM_LEDS - 1);
  const int p2 = beatsin16(15, 0, NUM_LEDS - 1);
  const int p3 = beatsin16(19, 0, NUM_LEDS - 1);
  leds[p1] += CHSV(0, 255, 255);
  leds[p2] += CHSV(96, 255, 255);
  leds[p3] += CHSV(160, 255, 255);
}

void LedController::animatePlasma() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    const uint8_t v = sin8((i * 10) + hue) + sin8((i * 7) + hue * 2) / 2;
    leds[i] = CHSV(v, 220, 255);
  }
}

void LedController::animatePaletteFlow() {
  static uint8_t startIndex = 0;
  startIndex += 2;
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(RainbowStripeColors_p, startIndex + (i * 4), 255);
  }
}



// ---- //
// Game //
// ---- //

void LedController::gameRandomChoose() {
  if (gameState == GAME_IDLE) {
    const uint8_t breath = beatsin8(10, 16, 96);
    fill_solid(leds, NUM_LEDS, CRGB(breath, breath, breath));

    if (btnRight) {
      positionIndex = random8(kSensorCount);
      gameRunnerHead = random16(NUM_LEDS);
      gameRunStep = 0;
      gameRunTotalSteps = ((2 + random8(2)) * NUM_LEDS)
        + ((sensorCenter(positionIndex) + NUM_LEDS - gameRunnerHead) % NUM_LEDS);
      if (gameRunTotalSteps == 0) {
        gameRunTotalSteps = NUM_LEDS;
      }
      gameLastStepAt = millis();
      gameState = GAME_RUN;
      Log.infoln("Game random choose: target position %d", positionIndex);
    }
    return;
  }

  if (gameState == GAME_RUN) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    constexpr uint8_t kRunnerSize = 16;
    constexpr uint8_t kRunnerMinValue = 48;
    for (uint8_t offset = 0; offset < kRunnerSize; ++offset) {
      const uint16_t ledIndex = wrapLedIndex(static_cast<int32_t>(gameRunnerHead) - (kRunnerSize / 2) + offset);
      leds[ledIndex] = CHSV(hue + (offset * (255 / kRunnerSize)), 255, kRunnerMinValue + (offset * 12));
    }

    leds[sensorCenter(positionIndex)] += CRGB(24, 24, 24);

    const uint16_t currentDelay = map(gameRunStep, 0, gameRunTotalSteps, 12, 85);
    if (millis() - gameLastStepAt >= currentDelay) {
      gameLastStepAt = millis();

      if (gameRunStep < gameRunTotalSteps) {
        gameRunnerHead = (gameRunnerHead + 1) % NUM_LEDS;
        gameRunStep++;
        hue += 4;
      }

      if (gameRunStep >= gameRunTotalSteps) {
        gameRunnerHead = sensorCenter(positionIndex);
        gameState = GAME_END;
      }
    }
    return;
  }

  if (gameState == GAME_END) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    const bool blinkOn = ((millis() / 250) % 2) == 0;
    const CRGB blinkColor = blinkOn ? CRGB(CHSV(hue, 255, 255)) : CRGB::Black;
    const int start = kSensorStart[positionIndex];
    const int end = start + kSensorLength[positionIndex];
    for (int led = start; led < end && led < NUM_LEDS; ++led) {
      leds[led] = blinkColor;
    }

    if (blinkOn) {
      leds[sensorCenter(positionIndex)] = kSensorColors[positionIndex];
    }

    hue += 2;
    if (btnRight) {
      gameState = GAME_IDLE;
      gameRunStep = 0;
      gameRunTotalSteps = 0;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    return;
  }
}

void LedController::gameSensorRace() {
  if (gameState == GAME_IDLE) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    const uint8_t breath = beatsin8(10, 18, 90);
    const bool phaseToggle = ((millis() / 700) % 2) == 0;
    for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
      if (((sensor + static_cast<uint8_t>(phaseToggle)) % 2) != 0) {
        continue;
      }

      CRGB color = kSensorColors[sensor];
      color.nscale8_video(breath);
      const int start = kSensorStart[sensor];
      const int end = start + kSensorLength[sensor];
      for (int led = start; led < end && led < NUM_LEDS; ++led) {
        leds[led] = color;
      }
    }

    for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
      if ((sensorTouched & (1 << sensor)) == 0) {
        continue;
      }

      const int start = kSensorStart[sensor];
      const int end = start + kSensorLength[sensor];
      for (int led = start; led < end && led < NUM_LEDS; ++led) {
        leds[led] = kSensorColors[sensor];
      }
    }

    if (btnRight) {
      const uint8_t selectedMask = sensorTouched;
      const uint8_t playerCount = countActivePlayers(selectedMask);
      if (playerCount < 2) {
        Log.warningln("Sensor race: need at least 2 active sensors to start");
        return;
      }

      raceActiveMask = selectedMask;
      raceAliveMask = selectedMask;
      raceWinner = 0;
      racePrevTouched = sensorTouched;
      for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
        racePosition[sensor] = sensorCenter(sensor);
      }

      gameState = GAME_RUN;
      Log.infoln("Sensor race: start with %d players", playerCount);
    }
    return;
  }

  if (gameState == GAME_RUN) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    const uint8_t risingEdges = sensorTouched & ~racePrevTouched;
    for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
      const uint8_t bit = (1 << sensor);
      if ((raceAliveMask & bit) == 0 || (risingEdges & bit) == 0) {
        continue;
      }

      racePosition[sensor] = wrapLedIndex(static_cast<int32_t>(racePosition[sensor]) + 3);

      for (uint8_t other = 0; other < kSensorCount; ++other) {
        if (other == sensor) {
          continue;
        }

        const uint8_t otherBit = (1 << other);
        if ((raceAliveMask & otherBit) == 0) {
          continue;
        }

        bool overlap = false;
        for (int8_t ownOffset = -1; ownOffset <= 1 && !overlap; ++ownOffset) {
          const uint16_t ownLed = wrapLedIndex(static_cast<int32_t>(racePosition[sensor]) + ownOffset);
          for (int8_t otherOffset = -1; otherOffset <= 1; ++otherOffset) {
            const uint16_t otherLed = wrapLedIndex(static_cast<int32_t>(racePosition[other]) + otherOffset);
            if (ownLed == otherLed) {
              overlap = true;
              break;
            }
          }
        }

        if (overlap) {
          raceAliveMask &= ~otherBit;
          Log.infoln("Sensor race: player %d ate player %d", sensor + 1, other + 1);
        }
      }
    }

    for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
      if ((raceAliveMask & (1 << sensor)) == 0) {
        continue;
      }

      for (int8_t offset = -1; offset <= 1; ++offset) {
        const uint16_t ledIndex = wrapLedIndex(static_cast<int32_t>(racePosition[sensor]) + offset);
        leds[ledIndex] = kSensorColors[sensor];
      }
    }

    const uint8_t aliveCount = countActivePlayers(raceAliveMask);
    if (aliveCount <= 1) {
      gameState = GAME_END;
      raceWinner = 0;
      for (uint8_t sensor = 0; sensor < kSensorCount; ++sensor) {
        if ((raceAliveMask & (1 << sensor)) != 0) {
          raceWinner = sensor;
          break;
        }
      }
      Log.infoln("Sensor race: winner is player %d", raceWinner + 1);
    }

    racePrevTouched = sensorTouched;
    return;
  }

  if (gameState == GAME_END) {
    const uint8_t breath = beatsin8(10, 24, 255);
    CRGB winnerColor = kSensorColors[raceWinner];
    winnerColor.nscale8_video(breath);
    fill_solid(leds, NUM_LEDS, winnerColor);

    if (btnRight) {
      gameState = GAME_IDLE;
      resetSensorRace();
    }
  }
}

void LedController::resetSensorRace() {
  raceActiveMask = 0;
  raceAliveMask = 0;
  raceWinner = 0;
  racePrevTouched = sensorTouched;
  for (uint8_t i = 0; i < kSensorCount; ++i) {
    racePosition[i] = sensorCenter(i);
  }
}

uint8_t LedController::countActivePlayers(const uint8_t mask) const {
  uint8_t count = 0;
  for (uint8_t i = 0; i < kSensorCount; ++i) {
    if ((mask & (1 << i)) != 0) {
      count++;
    }
  }
  return count;
}

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
  else if (ledMode == GAME) max = gameNames.size() - 1;

  if (index < 0) index = 0;
  if (index > max) index = max;
}
