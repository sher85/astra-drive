# MilkyWay Spinner

Arduino stepper controller for ultra-slow rotation in long-exposure and astrophotography setups.

MilkyWay Spinner drives a small turntable or sculpture at very slow, repeatable speeds so a subject can rotate smoothly during a night shoot. The current build is centered on an Arduino Uno, a 28BYJ-48 stepper motor, and a ULN2003 driver board, with the sketch already structured to support future STEP/DIR drivers such as an A4988 or DRV8825.

## What This Repo Contains

- [`code/spinner_control.ino`](code/spinner_control.ino): main Arduino sketch
- [`wiring/wiring-table.md`](wiring/wiring-table.md): detailed breadboard and pin reference
- [`wiring/wiring-diagram.pdf`](wiring/wiring-diagram.pdf): wiring diagram for the current hardware setup

## How It Works

The sketch uses the `AccelStepper` library in a non-blocking loop. A push button toggles the motor on and off, an LED mirrors the motor state, and the selected preset determines how long one full revolution should take.

Current supported modes:

- `DRIVER_ULN2003_28BYJ`: default build for `28BYJ-48 + ULN2003`
- `DRIVER_STEP_DIR`: future-ready mode for STEP/DIR drivers like `A4988` or `DRV8825`

Current speed presets:

- `REV_4H`: one revolution every 4 hours
- `REV_2H`: one revolution every 2 hours
- `REV_1MIN`: one revolution every 1 minute
- `REV_10S`: one revolution every 10 seconds

The sketch currently defaults to:

```cpp
#define DRIVER_ULN2003_28BYJ
SpeedPreset PRESET = REV_10S;
```

## Hardware

Required for the current default build:

- Arduino Uno or compatible board
- 28BYJ-48 stepper motor
- ULN2003 driver board
- Momentary push button
- LED and 220-330 ohm resistor
- External battery pack for the motor/driver
- Common ground shared between Arduino and motor supply

Software dependency:

- `AccelStepper` library installed through Arduino Library Manager

## Wiring Summary

For the full layout, use [`wiring/wiring-table.md`](wiring/wiring-table.md) and [`wiring/wiring-diagram.pdf`](wiring/wiring-diagram.pdf).

Core Arduino connections for the current sketch:

| Component | Arduino Pin | Notes |
| --- | --- | --- |
| ULN2003 `IN1` | `D8` | Physical wiring |
| ULN2003 `IN2` | `D9` | Physical wiring |
| ULN2003 `IN3` | `D10` | Physical wiring |
| ULN2003 `IN4` | `D11` | Physical wiring |
| Button | `D2` | Active-low with `INPUT_PULLUP` |
| LED | `D3` | Mirrors motor on/off state |

Important:

- Keep the ULN2003 board wired physically in order: `IN1 -> IN2 -> IN3 -> IN4`
- The sketch may use a different internal software stepping sequence for smooth motion
- All grounds must be tied together

## Setup

1. Install the `AccelStepper` library in Arduino IDE.
2. Open [`code/spinner_control.ino`](code/spinner_control.ino).
3. Confirm the driver selection at the top of the file.
4. Set the speed preset you want.
5. Upload the sketch to the Arduino.
6. Open Serial Monitor at `9600` baud to confirm the active configuration.
7. Press the button to toggle the motor on and off.

## Serial Diagnostics

When using the default ULN2003 build, you can send `t` in Serial Monitor to run a coil self-test. This energizes each ULN2003 output in sequence so you can verify the board LEDs and coil order.

## Technical Notes

- The default 28BYJ-48 configuration assumes about `4096` half-steps per output shaft revolution
- Speed is converted into steps per second and applied with `runSpeed()`
- `stepper.disableOutputs()` is used when idle to reduce heat and power draw
- Debounce logic is built into the button handler
- The sketch caps very large step rates to avoid unrealistic or unsafe values

## Project Structure

This repo is small and purpose-built:

- The sketch is the main product
- The wiring docs support assembly and troubleshooting
- The code is written to stay simple for the current sculpture/turntable use case while leaving room for a stronger motor and driver later

## Versioning And Releases

This repo uses `release-please` for automated versioning and GitHub Releases.

- Release metadata lives in `version.txt` and `CHANGELOG.md`
- The release workflow runs from `.github/workflows/release-please.yml`
- Future release PRs are driven by Conventional Commit messages such as `feat:` and `fix:`
- The repository is bootstrapped at `0.1.0` as the first documented baseline

If you want the current state published as an actual GitHub release, create a one-time `v0.1.0` tag/release from the current `main` branch. After that, `release-please` can manage subsequent releases automatically.

## Good Next Improvements

- Add a runtime menu for switching presets without editing the sketch
- Persist the selected preset in EEPROM
- Add direction control or reversible motion
- Add camera trigger or intervalometer sync
- Add tested STEP/DIR wiring and defaults for a NEMA-17 upgrade

## Who This Is For

This project is a good fit for:

- Astrophotography and timelapse experiments
- Slow-rotation display platforms
- Arduino users who want a practical stepper project with a clear upgrade path

### Created by Mauricio Castro
