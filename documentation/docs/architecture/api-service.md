# APIService

Exposes a lightweight REST API over HTTP on port **80** using `ESPAsyncWebServer`.  
JSON payloads use `ArduinoJson` v7.

## Registered routes

| Method | Path         | Description                      |
|--------|-------------|----------------------------------|
| GET    | `/infos`    | Firmware version & git hash      |
| GET    | `/status`   | Current LED state                |
| GET    | `/animations` | List of available animations   |
| GET    | `/games`    | List of available games          |
| POST   | `/state`    | Enable/disable & set brightness  |
| POST   | `/sensor`   | Switch to Sensor mode            |
| POST   | `/color`    | Switch to Color mode & set colour|
| POST   | `/animation`| Switch to Animation mode & pick  |
| POST   | `/game`     | Switch to Game mode & pick       |

See [API Reference → Endpoints](../api/endpoints.md) for full request/response examples.

