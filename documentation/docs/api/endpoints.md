# API Endpoints

Base URL: `http://table-salon.local` (or the device IP)

---

## GET /infos

Returns the firmware version information.

**Response**

```json
{
  "name": "Table Salon",
  "version": "2026.05.21",
  "hashGit": "a1b2c3d"
}
```

---

## GET /status

Returns the current LED controller state.

**Response — Sensor mode**

```json
{
  "state": { "enabled": true, "intensity": 70 },
  "mode": 3,
  "sensor": { "touched": 5 }
}
```

**Response — Color mode**

```json
{
  "state": { "enabled": true, "intensity": 70 },
  "mode": 0,
  "color": { "red": 255, "green": 100, "blue": 0, "pulse": false }
}
```

**Response — Animation mode**

```json
{
  "state": { "enabled": true, "intensity": 70 },
  "mode": 1,
  "animation": { "id": 0, "name": "Rainbow" }
}
```

**Response — Game mode**

```json
{
  "state": { "enabled": true, "intensity": 70 },
  "mode": 2,
  "game": { "id": 0, "name": "Random Choose" }
}
```

---

## GET /animations

Returns the list of available animations.

**Response**

```json
[
  { "id": 0, "name": "Rainbow" },
  { "id": 1, "name": "Rainbow (glitter)" },
  { "id": 2, "name": "Confetti" },
  { "id": 3, "name": "Sinelon" },
  { "id": 4, "name": "BPM" },
  { "id": 5, "name": "Juggle" }
]
```

---

## GET /games

Returns the list of available games.

**Response**

```json
[
  { "id": 0, "name": "Random Choose" }
]
```

---

## POST /state

Enable or disable the strip, and set brightness.

**Body**

```json
{
  "enabled": true,
  "intensity": 70
}
```

> `intensity` is a percentage (0–100). It is converted to 0–255 internally.

**Response**

```json
{ "ack": true }
```

---

## POST /sensor

Switch to **Sensor** mode.

**Body** *(empty JSON object accepted)*

```json
{}
```

**Response**

```json
{ "ack": true }
```

---

## POST /color

Switch to **Color** mode and set a solid colour.

**Body**

```json
{
  "red": 255,
  "green": 100,
  "blue": 0,
  "pulse": false
}
```

**Response**

```json
{ "ack": true }
```

---

## POST /animation

Switch to **Animation** mode and select an animation by id.

**Body**

```json
{ "id": 2 }
```

**Response**

```json
{ "ack": true }
```

---

## POST /game

Switch to **Game** mode and select a game by id.

**Body**

```json
{ "id": 0 }
```

**Response**

```json
{ "ack": true }
```

