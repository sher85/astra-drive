# MilkyWay Spinner

This project controls a 28BYJ-48 stepper motor via an Arduino using a simple button interface. The motor is designed to perform ultra-slow rotations, intended for long-exposure astrophotography time-lapses (e.g., rotating a ballerina sculpture in front of the Milky Way over several hours).

## Features

- Button-controlled start/stop for the motor
- Adjustable rotation speed via RPM setting
- Designed for 28BYJ-48 stepper motor (2048 steps per revolution)
- Compatible with Arduino UNO
- Can be adapted for longer durations (e.g. 1 rotation per hour)

## Wiring

- **Stepper Motor Pins**: IN1 = 8, IN2 = 9, IN3 = 10, IN4 = 11
- **Button Pin**: 2 (uses internal pull-up resistor)
- **Power Supply**: Can run on 5V USB or battery bank

## File Overview

- `spinner_control.ino` – Main Arduino sketch

## Usage

1. Connect motor and button according to the wiring diagram.
2. Set the desired `motorSpeedRPM` in the sketch.
3. Upload the code to your Arduino.
4. Press the button to toggle the motor on or off.

## Future Improvements

- Add low-power rotation mode (e.g. 1 rev per 4 hours)
- Support for other stepper motors like NEMA 17
- Add OLED or LED indicator for status feedback
- Optional acceleration or delay controls

## License

MIT