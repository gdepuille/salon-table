# Salon Table

> An **ESP32-based firmware** for a custom LED coffee table with Wi-Fi control, OTA updates, and interactive games.

[![GitHub release](https://img.shields.io/github/release/gdepuille/salon-table?style=flat-square)](https://github.com/gdepuille/salon-table/releases)
[![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)](LICENSE)
[![Documentation](https://img.shields.io/badge/docs-GitHub%20Pages-blue?style=flat-square)](https://gdepuille.github.io/salon-table/)

---

## Overview

**Salon Table** is a firmware for an Arduino Nano ESP32 that drives a NeoPixel LED strip wrapped around a coffee table. It provides:

- 🌈 **4 LED modes** — Sensor touch, solid colors, animations, and interactive games
- 📡 **REST API** — full control over Wi-Fi (HTTP on port 80)
- 🎮 **Physical controls** — 2 buttons + 8-zone capacitive touch sensor
- 🔄 **OTA firmware updates** — upload wirelessly without USB
- ✨ **Google Home-style startup animation**

---

## Quick Start

### Prerequisites

- [PlatformIO](https://platformio.org/) CLI or VSCode extension
- Arduino Nano ESP32 microcontroller
- NeoPixel LED strip (150 LEDs recommended)
- USB-C cable

### Flash firmware

```bash
# 1. Clone the repository
git clone https://github.com/gdepuille/salon-table.git
cd salon-table

# 2. Copy and configure secrets
cp include/common.secrets.h.example include/common.secrets.h
# Edit with your Wi-Fi credentials and OTA password

# 3. Flash via USB
platformio run -e nanoesp32 --target upload
```

### Update via OTA

Once connected to Wi-Fi:

```bash
platformio run -e nanoesp32-ota --target upload
```

---

## Features

| Feature          | Description                                           |
|-----------------|-------------------------------------------------------|
| **Sensor Mode** | 8 capacitive zones light up when touched              |
| **Color Mode**  | Solid user-defined RGB colour across the strip        |
| **Animation**   | 6 built-in FastLED animations (Rainbow, Confetti...) |
| **Game Mode**   | "Random Choose" — a roulette-style mini-game         |
| **REST API**    | Control everything from a mobile app or web UI        |
| **OTA Updates** | Update firmware wirelessly with visual feedback       |

---

## Documentation

📖 **Full documentation** is available on **[GitHub Pages](https://gdepuille.github.io/salon-table/)**

### Key topics

- [Getting Started](https://gdepuille.github.io/salon-table/getting-started/hardware/) — Hardware setup & flashing
- [Architecture](https://gdepuille.github.io/salon-table/architecture/overview/) — System design & components
- [API Reference](https://gdepuille.github.io/salon-table/api/endpoints/) — All HTTP endpoints with examples
- [LED Modes](https://gdepuille.github.io/salon-table/modes/sensor/) — How each mode works
- [OTA Updates](https://gdepuille.github.io/salon-table/ota/) — Wireless firmware updates

---

## Project Structure

```
salon-table/
├── src/
│   ├── main.cpp                 # Entry point
│   ├── controllers/
│   │   ├── LedController.cpp    # FastLED state machine
│   │   └── NetworkController.cpp # Wi-Fi + OTA
│   └── service/
│       └── APIService.cpp       # REST API
├── documentation/               # MkDocs documentation
│   ├── mkdocs.yml
│   └── docs/
├── platformio.ini              # PlatformIO config
└── README.md                   # This file
```

---

## Building locally

```bash
# Build firmware
platformio run -e nanoesp32

# Build documentation (MkDocs)
pip install mkdocs-material
cd documentation && mkdocs serve
```

---

## Technologies

- **Microcontroller** — Arduino Nano ESP32 (ESP32-S3)
- **LED library** — [FastLED](http://fastled.io/) v3.10.3
- **Web framework** — [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- **JSON** — [ArduinoJson](https://arduinojson.org/) v7.4.2
- **Logging** — [ArduinoLog](https://github.com/thijse/Arduino-Log)
- **Touch detection** — Adafruit CAP1188 (I²C)
- **Documentation** — [MkDocs](https://www.mkdocs.org/) with [Material theme](https://squidfunk.github.io/mkdocs-material/)

---

## License

This project is licensed under the **[MIT License](LICENSE)** — feel free to use, modify, and distribute.

---

## Contributing

Found a bug or want to improve something?

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Commit your changes (`git commit -m "Add my feature"`)
4. Push to the branch (`git push origin feature/my-feature`)
5. Open a Pull Request

---

## Author

**Gregory DEPUILLE** — [GitHub](https://github.com/gdepuille)

---

## Support

For questions or issues:

- **Documentation** → https://gdepuille.github.io/salon-table/
- **GitHub Issues** → [Open an issue](https://github.com/gdepuille/salon-table/issues)
- **Serial logs** → `platformio device monitor --baud 115200`
