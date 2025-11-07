# MilkyWay Spinner

> ⚙️ Compatible with both the original fixed-RPM implementation and the new modular AccelStepper version.  
> For current builds, use `spinner_control.ino` with the preset system described below.

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