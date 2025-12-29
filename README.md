# Table Salon firmware

## API

### Infos

Request :

GET /infos

Response :

```json
{
  "name": "table-salon",
  "version": "YYYY.MM.DD",
  "git_hash": "ebef14bc",
}
```

### Status

Request :

GET /status

Response :

```json
{
  "mode": "simple|animation|game",
  "simple": {
    "color": "#FFBBAA",
    "brightness": 80
  },
  "animation": {
    "name": "sparkling"
  },
  "game": {
    "name": "speed"
  }
}
```