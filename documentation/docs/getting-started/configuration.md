# Configuration

All secrets are stored in `include/common.secrets.h` (excluded from version control).

## Secret file

Create it from the template:

```bash
cp include/common.secrets.h.example include/common.secrets.h
```

Then fill in the values:

```cpp
// include/common.secrets.h

#define HOSTNAME     "table-salon"
#define WIFI_SSID    "YourNetwork"
#define WIFI_PASS    "YourPassword"
#define OTA_PASSWORD "your-ota-password"
```

## Build flags

Two macros are injected automatically at build time:

| Macro         | Source                          | Example         |
|---------------|---------------------------------|-----------------|
| `TIMESTAMP`   | `date +%Y.%m.%d`                | `2026.05.21`    |
| `COMMIT_HASH` | `git log -1 --format=%h`        | `a1b2c3d`       |

They are exposed by the `GET /infos` endpoint.

## Default values

| Setting     | Default | Location                      |
|-------------|---------|-------------------------------|
| Brightness  | 180/255 | `LedController.h`             |
| LED mode    | SENSOR  | `LedController.h`             |
| Hostname    | set in `common.secrets.h`      | —   |

