# MilkyWay Spinner

This project controls a 28BYJ-48 stepper motor via an Arduino using a simple button interface. The motor is designed to perform ultra-slow rotations, intended for long-exposure astrophotography time-lapses (e.g., rotating a ballerina sculpture in front of the Milky Way over several hours).

## Features

- Button-controlled start/stop for the motor
- Adjustable rotation speed via RPM setting
- Designed for 28BYJ-48 stepper motor (2048 steps per revolution)
- Compatible with Arduino UNO
- Can be adapted for longer durations (e.g. 1 rotation per hour) or ultra low rpms (i.e. 1 revolution per 4 hours)

## Wiring

- `wiring-diagram.txt` - Main schematic file.

## File Overview

- `spinner_control.ino` – Main Arduino code

## Usage

1. Connect motor and button according to the wiring diagram.
2. Set the desired `motorSpeedRPM` in the sketch.
3. Upload the code to your Arduino.
4. Press the button to toggle the motor on or off.


## Suggested RPM Settings

| Use Case               | Rotation Rate               | RPM Value     |
|------------------------|-----------------------------|---------------|
| Testing (visible spin) | 1 rotation every 12 seconds | 5             |
| Real-time preview      | 1 rotation every 1 minute   | 1.0           |
| Field Use: 1 rev/hr    | 1 rotation every 1 hour     | 0.0167        |
| Field Use: 1 rev/2hr   | 1 rotation every 2 hours    | 0.0083        |
| Field Use: 1 rev/3hr   | 1 rotation every 3 hours    | 0.0056        |
| Field Use: 1 rev/4hr   | 1 rotation every 4 hours    | 0.0042        |
| Field Use: 1 rev/6hr   | 1 rotation every 6 hours    | 0.0028        |
| Field Use: 1 rev/8hr   | 1 rotation every 8 hours    | 0.0021        |

To change the rotation speed, update the `motorSpeedRPM` constant in `spinner_control.ino` with one of the values above.
