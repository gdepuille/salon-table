# Game Mode

**Mode ID:** `2`

## Available games

| ID | Name           | Description |
|----|----------------|-------------|
| 0  | Random Choose  | Roulette-like runner that slows down and picks a random sensor zone |
| 1  | Sensor Race    | Touch race where players move by touch rising edges and can eat opponents |

## Switch game

=== "Via API"

	```json
	POST /game
	{ "id": 1 }
	```

=== "Via buttons"

	- **Left click** — previous game
	- **Right click** — next game

## Game details

### Random Choose

1. **Idle**: white breathing strip
2. **Run**: a colourful runner spins and slows down
3. **End**: selected zone blinks; right click returns to idle

### Sensor Race

1. **Preparation**:
   - coloured breathing alternation over sensor zones
   - touched zones stay fully lit using the same zone size as Sensor mode
2. **Start**:
   - right click starts with currently active sensors
   - at least 2 players are required
3. **Run**:
   - each player is a 3-LED marker in their sensor colour
   - a rising edge on a player sensor moves their marker by +3 LEDs
   - if a player overlaps another marker, the other player is eaten
4. **End**:
   - winner colour breathes on the full strip
   - right click restarts from preparation

## List all games

```
GET /games
```

Response:

```json
[
  { "id": 0, "name": "Random Choose" },
  { "id": 1, "name": "Sensor Race" }
]
```

