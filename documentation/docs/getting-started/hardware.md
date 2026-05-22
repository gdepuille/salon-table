# Hardware

## Board

The firmware targets the **Arduino Nano ESP32** (ESP32-S3 based, 240 MHz, 320 KB RAM, 16 MB Flash).

## LED Strip

The LED strip is a NeoPixel (GRB order) connected to pin **D2**. It wraps around the table perimeter:

| Segment      | LEDs |
|-------------|------|
| Width (×2)  | 112  |
| Height (×2) | 38   |
| **Total**   | **150** |

```
#define LEDS_WIDHT  56
#define LEDS_HEIGHT 19
#define NUM_LEDS    ((LEDS_WIDHT * 2) + (LEDS_HEIGHT * 2))  // = 150
```

### Physical Sensor-to-LED Mapping

The strip is organised into 8 zones matching the 8 capacitive sensor channels:

| Sensor | Start LED | Length | Gap before |
|--------|-----------|--------|------------|
| 1      | 0         | 16     | —          |
| 2      | 21        | 15     | 5 LEDs     |
| 3      | 41        | 15     | 5 LEDs     |
| 4      | 56        | 19     | 0          |
| 5      | 75        | 16     | 0          |
| 6      | 96        | 15     | 5 LEDs     |
| 7      | 116       | 15     | 5 LEDs     |
| 8      | 131       | 19     | 0          |

## Capacitive Touch Sensor

The **Adafruit CAP1188** communicates over I²C. It provides 8 independent touch channels, one per table edge segment.

## Buttons

| Button | Pin | Function |
|--------|-----|----------|
| Left   | D4  | Single click: previous index / Double click: change mode / Long press: decrease brightness |
| Right  | D5  | Single click: next index / Long press: increase brightness |

## Built-in RGB LED

Used as a visual indicator during OTA updates:

| Pin         | Function      |
|-------------|---------------|
| LED_BUILTIN | Heartbeat blink (1 Hz) |
| LED_RED     | OTA progress < 40 %   |
| LED_GREEN   | OTA progress > 60 %   |
| LED_BLUE    | OTA progress > 90 %   |

