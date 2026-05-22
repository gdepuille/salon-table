# Color Mode

**Mode ID:** `0`

## Behaviour

Fills the entire strip with a single solid colour. An optional **pulse** effect (in development) will breathe the brightness rhythmically.

## Switch via API

```json
POST /color
{
  "red": 255,
  "green": 100,
  "blue": 0,
  "pulse": false
}
```

## Switch via buttons

Double-click left until `mode == 0`, then use right-click / long-press to adjust brightness.

