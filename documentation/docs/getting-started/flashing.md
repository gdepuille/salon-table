# Flashing

## Prerequisites

- [PlatformIO](https://platformio.org/) (CLI or VSCode extension)
- USB-C cable connected to the Arduino Nano ESP32

## First Flash (USB)

```bash
# Clone the repository
git clone https://github.com/gdepuille/salon-table.git
cd salon-table

# Copy and fill in secrets
cp include/common.secrets.h.example include/common.secrets.h
# Edit common.secrets.h with your Wi-Fi credentials and OTA password

# Build and upload
platformio run -e nanoesp32 --target upload
```

## OTA Flash (Wi-Fi)

Once the device is on network:

```bash
platformio run -e nanoesp32-ota --target upload
```

The `nanoesp32-ota` environment is preconfigured with:

```ini
upload_protocol = espota
upload_port     = table-salon.local
upload_flags    = --auth=password
```

!!! note
    Change `--auth=password` to match the `OTA_PASSWORD` defined in `common.secrets.h`.

## Environments

| Environment        | Purpose                           |
|--------------------|-----------------------------------|
| `nanoesp32`        | Standard USB upload               |
| `nanoesp32-ota`    | OTA upload via mDNS               |
| `nanoesp32-debug`  | Debug build with `LOG_LEVEL_DEBUG`|

## Serial Monitor

```bash
platformio device monitor --baud 115200
```

