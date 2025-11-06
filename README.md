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
| Field Use: 1 rev/5min  | 1 rotation every 5 minutes  | 0.2           |
| Field Use: 1 rev/15min | 1 rotation every 15 minutes | 0.0667        |
| Field Use: 1 rev/30min | 1 rotation every 30 minutes | 0.0333        |
| Field Use: 1 rev/hr    | 1 rotation every 1 hour     | 0.0167        |
| Field Use: 1 rev/2hr   | 1 rotation every 2 hours    | 0.0083        |
| Field Use: 1 rev/3hr   | 1 rotation every 3 hours    | 0.0056        |
| Field Use: 1 rev/4hr   | 1 rotation every 4 hours    | 0.0042        |
| Field Use: 1 rev/6hr   | 1 rotation every 6 hours    | 0.0028        |
| Field Use: 1 rev/8hr   | 1 rotation every 8 hours    | 0.0021        |

To change the rotation speed, update the `motorSpeedRPM` constant in `spinner_control.ino` with one of the values above.

# MilkyWay Spinner

A modular, Arduino‑based motion controller designed for **ultra‑slow astrophotography rotations** — ideal for time‑lapses of sculptures, cameras, or turntables under the Milky Way. Built for **simplicity today** and **extensibility tomorrow**.

Originally created to rotate a small ballerina sculpture during long‑exposure night photography, the MilkyWay Spinner has evolved into a **refactor‑ready platform** supporting both the **ULN2003 + 28BYJ‑48 stepper motor** and future **STEP/DIR drivers** such as NEMA‑17 with A4988 or DRV8825.

---

## ✨ Features

- **Modular Driver Architecture** – switch between `ULN2003_28BYJ` or `STEP_DIR` via a single `#define`
- **AccelStepper‑based Control** – non‑blocking loop, smooth half‑stepping, minimal jitter
- **Predefined Speed Presets** – 4h, 2h, 1min, 10s per revolution for easy experimentation
- **Single‑Button Interface** – press to toggle rotation on/off
- **LED Status Indicator** – visual feedback for active/inactive state
- **Extensible Design** – structured for simple refactor to higher‑power motors or multi‑axis systems

---

## 🧠 Architecture Overview

```
[Button + LED]
       ↓
   Arduino Logic
       ↓
[Driver Interface Layer] ← selectable: ULN2003 (HALF4WIRE) or STEP/DIR (A4988/DRV8825)
       ↓
 [Stepper Motor]
```

`spinner_control.ino` separates the **user interface**, **speed presets**, and **motor driver abstraction**, making it easy to modify or extend the system for different motor types or control strategies.

---

## 🧰 Requirements

- **Arduino UNO** (or compatible)
- **AccelStepper library** (install from Arduino Library Manager)
- **28BYJ‑48 stepper motor + ULN2003 driver board** *(default)*
- Optional future: **NEMA‑17 + A4988/DRV8825**

---

## ⚙️ Setup & Wiring

Refer to [`wiring/wiring-table.md`](wiring/wiring-table.md) for pin connections.

| Component | Arduino Pin | Notes |
|------------|-------------|-------|
| IN1 | D8 | Stepper driver IN1 |
| IN2 | D10 | Stepper driver IN3 (note order) |
| IN3 | D9 | Stepper driver IN2 |
| IN4 | D11 | Stepper driver IN4 |
| Button | D2 | Active‑low, uses `INPUT_PULLUP` |
| LED | D3 | Mirrors motor state |
| Power | 4×AA pack (~4.8 V) | Feeds ULN2003/motor |

All grounds (Arduino + ULN2003 + power pack) must share a **common reference**.

---

## 🚀 Usage

1. Open `spinner_control.ino` in Arduino IDE.
2. In the **Driver Selection** section, confirm your hardware:
   ```cpp
   #define DRIVER_ULN2003_28BYJ   // for 28BYJ‑48 + ULN2003
   // #define DRIVER_STEP_DIR      // for NEMA‑17 + A4988/DRV8825
   ```
3. Choose your desired preset:
   ```cpp
   SpeedPreset PRESET = REV_4H;  // REV_2H, REV_1MIN, or REV_10S
   ```
4. Upload the sketch and open the **Serial Monitor** to confirm configuration.
5. Press the button to toggle motion. The LED indicates the active state.

---

## 🧮 Technical Details

### Step Calculations
- **28BYJ‑48 (half‑step)** ≈ 4096 steps per output‑shaft revolution (32 × 64 gearbox × 2 half‑steps)
- **NEMA‑17 (1.8°)** = 200 full steps/rev × microstep setting (e.g., ×16 = 3200 steps/rev)

The controller computes `stepsPerSecond = RPM × stepsPerRevolution / 60`, ensuring constant, non‑blocking motion via `runSpeed()`.

### Speed Presets
| Preset | One Revolution Every | RPM |
|--------|----------------------|-----|
| REV_4H | 4 hours | 0.00417 |
| REV_2H | 2 hours | 0.00833 |
| REV_1MIN | 1 minute | 1.0 |
| REV_10S | 10 seconds | 6.0 |

---

## 🧩 Engineering Notes

- **Non‑blocking architecture:** AccelStepper’s `runSpeed()` allows other tasks (e.g., sensors or future multi‑motor sync) to execute concurrently.
- **Safe speed caps:** RPM automatically limited to prevent damage or runaway values.
- **Modular constants:** `STEPS_PER_REV` and `MICROSTEP` isolate hardware parameters from control logic.
- **Power efficiency:** `stepper.disableOutputs()` cuts coil current when idle.

---

## 🔮 Future Development

- Add runtime serial menu to change presets without recompiling
- Integrate camera trigger synchronization
- Upgrade path to NEMA‑17 with DRV8825/A4988
- Expand to multi‑axis astrophotography mounts

---

### Created by Mauricio Castro
*Designed for long‑exposure astrophotography enthusiasts and engineers alike.*