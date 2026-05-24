# Animation Mode

**Mode ID:** `1`

## Available animations

| ID | Name               | Description                                              |
|----|--------------------|----------------------------------------------------------|
| 0  | Rainbow            | Full hue rainbow rotating around the strip               |
| 1  | Rainbow (glitter)  | Rainbow with random white sparkles                       |
| 2  | Confetti           | Random coloured speckles that fade smoothly              |
| 3  | Sinelon            | Single dot sweeping back and forth with a fading trail   |
| 4  | BPM                | Colour palette stripes pulsing at 62 BPM                 |
| 5  | Juggle             | 8 coloured dots weaving in and out of sync               |
| 6  | Twinkle Warm       | Warm white twinkles with soft fade out                   |
| 7  | Fireplace          | Fire-like heat diffusion in orange/red tones             |
| 8  | Ocean Wave         | Blue/cyan moving waves using a custom palette            |
| 9  | Meteor Rain        | Travelling meteor with a fading colourful tail           |
| 10 | Cylon Sweep        | Red scanner moving left/right with glow                  |
| 11 | Theater Chase      | Triple-step chase pattern with palette cycling           |
| 12 | Sparkle Burst      | Random sparkle bursts with short side flashes            |
| 13 | Larson Multi       | Three scanning points crossing at different speeds       |
| 14 | Plasma             | Organic plasma generated from layered sine waves         |
| 15 | Palette Flow       | Continuous rainbow-stripe palette flow                   |

## Switch animation

=== "Via API"

    ```json
    POST /animation
    { "id": 2 }
    ```

=== "Via buttons"

    - **Left click** — previous animation
    - **Right click** — next animation

## List all animations

```
GET /animations
```

Response:
```json
[
  { "id": 0, "name": "Rainbow" },
  { "id": 1, "name": "Rainbow (glitter)" },
  { "id": 2, "name": "Confetti" },
  { "id": 3, "name": "Sinelon" },
  { "id": 4, "name": "BPM" },
  { "id": 5, "name": "Juggle" },
  { "id": 6, "name": "Twinkle Warm" },
  { "id": 7, "name": "Fireplace" },
  { "id": 8, "name": "Ocean Wave" },
  { "id": 9, "name": "Meteor Rain" },
  { "id": 10, "name": "Cylon Sweep" },
  { "id": 11, "name": "Theater Chase" },
  { "id": 12, "name": "Sparkle Burst" },
  { "id": 13, "name": "Larson Multi" },
  { "id": 14, "name": "Plasma" },
  { "id": 15, "name": "Palette Flow" }
]
```

