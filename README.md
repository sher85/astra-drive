# Astra Drive

Arduino stepper controller for ultra-slow, controlled rotation in lightweight display and photography setups.

Astra Drive drives a small rotating display, including a lightweight ballerina figure, at very slow and repeatable speeds. The current build is centered on an Arduino Uno, a 28BYJ-48 stepper motor, and a ULN2003 driver board, with the sketch already structured to support future STEP/DIR drivers such as an A4988 or DRV8825.

## What This Repo Contains

- [`code/spinner_control.ino`](code/spinner_control.ino): main Arduino sketch
- [`wiring/wiring-table.md`](wiring/wiring-table.md): detailed breadboard and pin reference
- [`wiring/wiring-diagram.png`](wiring/wiring-diagram.png): primary wiring diagram for the current hardware setup
- [`wiring/wiring-diagram.pdf`](wiring/wiring-diagram.pdf): PDF export of the wiring diagram

## How It Works

The sketch uses the `AccelStepper` library in a non-blocking loop. A push button toggles the motor on and off, an LED mirrors the motor state, and the selected preset determines how long one full revolution should take.

Field-friendly controls are built in:

- Short press: toggle the motor on or off
- Long press while stopped: cycle to the next speed preset
- LED blink count: confirms the selected preset (`1` blink = `REV_10S`, `2` = `REV_1MIN`, `3` = `REV_2H`, `4` = `REV_4H`)
- EEPROM storage: remembers the last selected preset after power is removed

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

`REV_10S` is a bring-up and testing default so motion is easy to verify right after upload. For the actual slow-rotation display use case, switch to one of the slower presets before running the finished assembly.

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

For the full layout, use [`wiring/wiring-table.md`](wiring/wiring-table.md), [`wiring/wiring-diagram.png`](wiring/wiring-diagram.png), and [`wiring/wiring-diagram.pdf`](wiring/wiring-diagram.pdf).

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
4. Optionally change the default preset in code, or use the built-in long-press field control after upload.
5. Upload the sketch to the Arduino.
6. Open Serial Monitor at `9600` baud to confirm the active configuration.
7. Short press the button to toggle the motor on and off.
8. Long press the button while the motor is stopped to cycle presets.

## Serial Diagnostics

When using the default ULN2003 build, you can send `t` in Serial Monitor to run a coil self-test. This energizes each ULN2003 output in sequence so you can verify the board LEDs and coil order.

## Field Controls

The single-button interface is designed so you do not need a computer in the field:

- Short press toggles the motor state
- Long press while stopped advances to the next preset
- Presets cycle in this order: `REV_10S` → `REV_1MIN` → `REV_2H` → `REV_4H`
- The selected preset is shown with LED blink count and saved to EEPROM
- On startup, the sketch reloads the saved preset and blinks it once for confirmation

## Technical Notes

- The default 28BYJ-48 configuration assumes about `4096` half-steps per output shaft revolution
- Speed is converted into steps per second and applied with `runSpeed()`
- `stepper.disableOutputs()` is used when idle to reduce heat and power draw
- Debounce logic is built into the button handler
- The sketch caps very large step rates to avoid unrealistic or unsafe values

## Suitability For The Ballerina Spinner

Electrically, the current Uno + ULN2003 + 28BYJ-48 wiring is sound for the documented build and is a good proof-of-concept for a lightweight rotating figure.

The main limitation is motion quality at very slow presets. With `4096` half-steps per revolution, the current presets work out to approximately:

- `REV_4H`: one half-step every `3.52` seconds
- `REV_2H`: one half-step every `1.76` seconds
- `REV_1MIN`: about `68.3` steps/second
- `REV_10S`: about `409.6` steps/second

That means the `4H` and `2H` modes are controlled and repeatable, but they are not truly continuous-looking. For a ballerina display, the current setup is best suited to a **very light, well-balanced subject** where slight stepwise motion is acceptable.

If the finished piece needs smoother-looking ultra-slow motion, higher torque, or less visible stepping, the next hardware upgrade should be:

- a bearing-supported turntable so the motor does not carry the full vertical load directly
- a belt or gear reduction stage between the motor and the platform
- a microstepped STEP/DIR driver build such as a NEMA-17 with `A4988` or `DRV8825`

## Project Structure

This repo is small and purpose-built:

- The sketch is the main product
- The wiring docs support assembly and troubleshooting
- The code is written to stay simple for the current sculpture/turntable use case while leaving room for a stronger motor and driver later

## Versioning And Releases

This repo uses `release-please` for automated versioning and GitHub Releases.

- The release workflow runs from [`.github/workflows/release-please.yml`](.github/workflows/release-please.yml)
- Manifest configuration lives in [`release-please-config.json`](release-please-config.json) and [`.release-please-manifest.json`](.release-please-manifest.json)
- The current tracked release version is `0.1.1`, reflected in [`version.txt`](version.txt), [`CHANGELOG.md`](CHANGELOG.md), and the existing `v0.1.1` git tag
- Future release PRs are driven by Conventional Commit messages such as `feat:` and `fix:`

This repository is already bootstrapped for `release-please`. Push releasable commits to `main`, let `release-please` open or update the release PR, and merge that PR to cut the next release.

## License

This project is licensed under the `MIT` License. See [`LICENSE`](LICENSE).

## Good Next Improvements

- Add a runtime menu for switching presets without editing the sketch
- Add direction control or reversible motion
- Add camera trigger or intervalometer sync
- Add tested STEP/DIR wiring and defaults for a NEMA-17 upgrade

## Who This Is For

This project is a good fit for:

- Slow-rotation display platforms
- Kinetic art and rotating figure displays
- Astrophotography and timelapse experiments
- Arduino users who want a practical stepper project with a clear upgrade path
