# Sensor Mode

**Mode ID:** `3`  
**Default mode** at startup.

## Behaviour

Each of the **8 capacitive zones** maps to a segment of the LED strip. When a zone is touched its LEDs fade **in** with its assigned colour; when released they fade **out**.

The transition uses `blend()` with a fixed step (`kFadeStep = 40`) called every **20 ms**, resulting in a smooth ≈ 150 ms fade.

## Zone colours

| Sensor | LEDs       | Colour  |
|--------|-----------|---------|
| 1      | 0–15      | Red     |
| 2      | 21–35     | Blue    |
| 3      | 41–55     | Green   |
| 4      | 56–74     | Purple  |
| 5      | 75–90     | Yellow  |
| 6      | 96–110    | Orange  |
| 7      | 116–130   | Aqua    |
| 8      | 131–149   | White   |

!!! note "Gaps"
    Sensors 2, 3, 6 and 7 are preceded by a 5-LED dark gap (physical spacing on the table frame).

## Switch to Sensor mode

Via API:
```json
POST /sensor
{}
```

Via buttons: double-click left until `mode == 3`.

## Via POST /sensor

See [API Reference → POST /sensor](../api/endpoints.md#post-sensor).

