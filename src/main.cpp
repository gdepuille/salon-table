#include <Arduino.h>

#include <I2CScanner.h>
#include <FastLED.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Adafruit_CAP1188.h>

#if defined(DEBUG)
#define F_CPU 16000000L
#endif

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
#if defined(ARDUINO_SAMD_NANO_33_IOT)
#include <WiFiNINA.h>
#else
#include <WiFi.h>
#endif
#include <ArduinoOTA.h>
#endif
I2CScanner i2cScan;

// OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Capacitive sensor

auto cap = Adafruit_CAP1188();

// Leds
#define LEDS_DATA_PIN 6
#define LEDS_WIDHT 56
#define LEDS_HEIGHT 19
#define NUM_LEDS 3 // ((LEDS_WIDHT * 2) + (LEDS_HEIGHT * 2))
CRGB leds[NUM_LEDS];

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
#define WIFI_SSID "picotteland-ng"
#define WIFI_PASS "#z3uxoFf2sh&$QT?nQZ#joZiyTONeHyKRHe8BHVg"
#define HOSTNAME "table-salon"
#endif

// IO pins
#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_ENTER 4

void setup() {
#if defined(DEBUG)
  // Configure Serial monitoring
  delay(1000);
  Serial.begin(115200);
  Serial.println("Start table salon !");
#endif

#if defined(DEBUG)
  Serial.println("I2C Configuration");
#endif
  i2cScan.Init();
#if defined(DEBUG)
  i2cScan.Scan();
#endif

#if defined(DEBUG)
  Serial.println("Display Configuration");
#endif

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font

  u8g2.clearBuffer();					// clear the internal memory
  u8g2.drawStr(0, 10, "Table salon !");	// write something to the internal memory
  u8g2.drawStr(0, 20, COMMIT_HASH);	// write something to the internal memory
  u8g2.drawStr(0, 30, TIMESTAMP);	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
#if defined(DEBUG)
  Serial.println("Connecting to WiFi");
#endif
  WiFiClass::setHostname(HOSTNAME);
  while(WiFiClass::status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
#if defined(DEBUG)
    Serial.print(".");
#endif
    delay(10000);
  }
#if defined(DEBUG)
  Serial.println();
  Serial.println("Wifi OK");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  const IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  const int8_t rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
#endif


#if defined(DEBUG)
  Serial.println("Enable OTA flash upgrade");
#endif
  ArduinoOTA.setHostname(WiFiClass::getHostname());
  ArduinoOTA.onStart([]() -> void {
    Serial.println("Start OTA");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  });
  ArduinoOTA.onEnd([]() -> void {
    Serial.println("End OTA");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
  });
  ArduinoOTA.onProgress([](int current, int total) -> void {
    const int pct = (current / (total / 100));
    Serial.print("Upgrade ");
    Serial.print(pct);
    Serial.println("%");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    if (pct > 40) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    if (pct > 60) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    if (pct > 90) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  });
#if defined(ARDUINO_NANO_ESP32)
  ArduinoOTA.begin();
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
  ArduinoOTA.begin(WiFi.localIP(), HOSTNAME, "password", InternalStorage);
#endif
#endif

#if defined(DEBUG)
  Serial.println("IO Configuration");
#endif
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_ENTER, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
#if defined(ARDUINO_NANO_ESP32)
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
#endif


#if defined(DEBUG)
  Serial.println("Led strip configuration");
#endif
  CFastLED::addLeds<NEOPIXEL, LEDS_DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(20);
}

uint8_t i = 0;

void loop() {
#if defined(ARDUINO_NANO_ESP32)
  ArduinoOTA.handle();
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
  ArduinoOTA.poll();
#endif

  EVERY_N_MILLIS(200) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  EVERY_N_MILLIS(100) {
    FastLED.clear();
    leds[i] = CRGB::Blue;
    i++;
    if (i >= NUM_LEDS) {
      i = 0;
    }
    FastLED.show();
  }
}