# Radium

A Geometry Dash mod menu with a built-in replay tool, built on [Geode](https://geode-sdk.org).

## Features

- TPS bypass (60 to 10000)
- Speedhack (0.01x to 10x)
- Noclip
- Unlock all icons and colors
- Main level bypass
- Radium Replay: record and play back inputs, saved as `.radium1` files (format v1)

## Platforms

Windows, macOS, iOS and Android 64-bit. Android 32-bit is not supported.

## Opening the menu

- Desktop: press Tab
- Android and iOS: tap the floating Ra button

## Radium Replay

Open a level, go to the Replay tab and press Record. The level restarts, your inputs are recorded from the first frame, and Stop (or finishing the level) saves a `.radium1` file. Playing a replay sets the TPS it was recorded at and restarts the level.

Replays are saved in the mod's save folder under `replays`.

Current limits:

- A recording is a single attempt. Dying restarts it from frame 0.
- Practice mode and checkpoints are not supported yet.

## Project layout

```
src/
  Hooks/         hooks into the game
  Physics/       TPS bypass
  Assist/        tools that help with making macros
  RadiumReplay/  the replay format, recorder and player
  Utility/       the menu and everything unrelated to making macros
```

## Building Locally

Set the `GEODE_SDK` environment variable, then:

```
geode build
```

GitHub Actions builds every platform on each push.

## Font

The menu uses Open Sans Regular, licensed under the SIL Open Font License. See `resources/OFL.txt`.
