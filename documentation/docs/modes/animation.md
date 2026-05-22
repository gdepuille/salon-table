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
  { "id": 5, "name": "Juggle" }
]
```

