# Salon Table Firmware

**Salon Table** is an ESP32-based firmware for a custom LED coffee table. It controls a NeoPixel LED strip mounted around the edges of the table and exposes a Wi-Fi REST API for real-time control.

---

## Features

- 🌈 **4 LED modes** — Sensor, Color, Animation, Game
- 🎮 **2 physical buttons** — mode switching and index control
- 📡 **REST API** — full control over Wi-Fi
- 🔄 **OTA updates** — upload firmware wirelessly
- 💡 **Google Home-style startup animation**
- 🖐 **Capacitive touch sensor** (CAP1188, 8 zones)

---

## Hardware

| Component        | Reference                |
|-----------------|--------------------------|
| Microcontroller | Arduino Nano ESP32       |
| LED strip       | NeoPixel (150 LEDs total)|
| Touch sensor    | Adafruit CAP1188 (I²C)   |
| Buttons         | 2× tactile on D4 / D5    |

---

## Quick links

- [Getting Started → Hardware](getting-started/hardware.md)
- [API Reference](api/endpoints.md)
- [LED Modes](modes/sensor.md)
- [OTA Updates](ota.md)

