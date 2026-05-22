# Architecture Overview

## Source tree

```
src/
├── main.cpp                 # Entry point: setup() / loop()
├── controllers/
│   ├── LedController.h/.cpp # FastLED state machine
│   └── NetworkController.h/.cpp # Wi-Fi + OTA
└── service/
    ├── APIService.h/.cpp    # ESPAsyncWebServer REST API
```

## Component diagram

```mermaid
flowchart TD
    subgraph main["main.cpp"]
        setup["setup()\nconnectWifi → led.setup → api.setup\n→ startupGoogleHome"]
        loop["loop()\nnetwork.process()\nbtnLeft.tick() / btnRight.tick()\nled.process(cap.touched())  every 20 ms"]
    end

    subgraph net["NetworkController"]
        connectWifi["connectWifi()"]
        enableOTA["enableOTA()"]
        netProcess["process()"]
    end

    subgraph led["LedController"]
        modes["SENSOR / COLOR\nANIMATION / GAME"]
        updateSensor["updateSensorLeds()"]
        updateOta["updateOtaLeds()"]
        gameRandom["gameRandomChoose()"]
    end

    subgraph api["APIService (HTTP:80)"]
        routes["GET /infos /status /animations /games\nPOST /state /color /animation /game /sensor"]
    end

    setup --> net
    setup --> led
    setup --> api
    loop --> net
    loop --> led
    enableOTA -- "OTA callbacks" --> led
    led --> api
```

## Main loop timing

| Task                         | Interval  |
|------------------------------|-----------|
| OTA handle + button tick     | Every iteration |
| LED strip update             | Every 20 ms (~50 FPS) |
| Heartbeat LED_BUILTIN toggle | Every 1 s  |

