#include <Arduino.h>
#include <I2CScanner.h>
#include <FastLED.h>
#include <Adafruit_CAP1188.h>
#include <Adafruit_SSD1306.h>

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
#include <ArduinoOTA.h>
#endif
I2CScanner i2cScan;

// Leds
#define LEDS_WIDHT 56
#define LEDS_HEIGHT 19
#define NUM_LEDS ((LEDS_WIDHT * 2) + (LEDS_HEIGHT * 2))
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

// Screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
auto display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Capacitive touch
#define SENSITIVITY_REGISTER 0x1F
#define SENSITIVITY_DELTA_SENSE 0x1
#define SENSITIVITY_BASE_SHIFT 0xF
auto cap = Adafruit_CAP1188();

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
#define WIFI_SSID "picotteland-ng"
#define WIFI_PASS "#z3uxoFf2sh&$QT?nQZ#joZiyTONeHyKRHe8BHVg"
#define HOSTNAME "table-salon"
#endif


void setup() {
#if defined(DEBUG)
  // Configure Serial monitoring
  Serial.begin(115200);
  Serial.println("Table test!");
#endif

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();
  display.display();
  delay(1000);

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)

#if defined(DEBUG)
  Serial.println("Connecting to WiFi");
#endif

  uint8_t idxDot = 0;
  while(ArduinoOTA.status() != WL_CONNECTED) {
#if defined(DEBUG)
    Serial.print(".");
#endif
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("WiFi connecting");
    for (uint8_t i = 0; i < idxDot; i++) {
      display.print('.');
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);
    }
    display.display();

    idxDot++;
    if (idxDot > 3) {
      idxDot = 0;
    }
    delay(1000);
  }
#endif
  display.clearDisplay();
  display.display();

#if defined(DEBUG)
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

#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
  ArduinoOTA.begin();
#endif

  pinMode(LED_BUILTIN, OUTPUT);
#if defined(ARDUINO_NANO_ESP32)
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
#endif


  i2cScan.Init();
  i2cScan.Scan();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
#if defined(DEBUG)
    Serial.println(F("SSD1306 allocation failed"));
#endif

    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
#if defined(ARDUINO_NANO_ESP32)
      analogWrite(LEDR, digitalRead(LED_BUILTIN)? 0 : 255);
#endif
      delay(500);
    }
  }
#if defined(DEBUG)
  Serial.println(F("SSD1306 allocation success"));
#endif

  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
#if defined(DEBUG)
    Serial.println("CAP1188 not found");
#endif
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
#if defined(ARDUINO_NANO_ESP32)
      analogWrite(LEDR, digitalRead(LED_BUILTIN)? 0 : 255);
#endif
      delay(100);
    }
  }
#if defined(DEBUG)
  Serial.println("CAP1188 found!");
#endif

  cap.writeRegister(SENSITIVITY_REGISTER, (SENSITIVITY_DELTA_SENSE << 4) + SENSITIVITY_BASE_SHIFT);
#if defined(DEBUG)
  Serial.println("CAP1188 sensitivity changed!");
#endif

  CFastLED::addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(20);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
}

uint8_t i = 0;

void loop() {
#if defined(ARDUINO_NANO_ESP32) or defined(ARDUINO_SAMD_NANO_33_IOT)
  ArduinoOTA.handle();
#endif

  uint8_t touched = cap.touched();
  display.clearDisplay();

  EVERY_N_MILLIS(100) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Led : ");
    display.println(i);
    display.display();

    FastLED.clear();
    leds[i] = CRGB::Blue;
    i++;
    if (i >= NUM_LEDS) {
      i = 0;
    }
    FastLED.show();
  }
}