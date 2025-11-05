# MilkyWay Spinner Wiring Table

This document provides a complete wiring reference for the MilkyWay Spinner setup, which includes an Arduino Uno, ULN2003 driver board, 28BYJ-48 stepper motor, push button, and LED.

| Breadboard                | Component                | Arduino Uno R3 | Stepper Driver Board (ULN2003)                | Stepper Motor | Notes                                     |
| ------------------------- | ------------------------ | -------------- | --------------------------------------------- | ------------- | ----------------------------------------- |
|                           |                          | D8             | IN1                                           |               |                                           |
|                           |                          | D9             | IN2                                           |               |                                           |
|                           |                          | D10            | IN3                                           |               |                                           |
|                           |                          | D11            | IN4                                           |               |                                           |
| A30                       |                          |                | VCC                                           |               |                                           |
| A29                       |                          |                | GND                                           |               |                                           |
|                           |                          |                | Stepper motor output connector (4-pin JST-XH) |   28BYJ-48    |                                           |
| I12                       |                          | D2             |                                               |               | D2 works as an input pullup               |
| C5                        |                          | D3             |                                               |               | D3 works as an output                     |
| Right-side GND Rail       |                          | GND            |                                               |               |                                           |
| J30 ↔ Right-side VCC Rail |                          |                |                                               |               | Jumper                                    |
| J29 ↔ Right-side GND Rail |                          |                |                                               |               | Jumper                                    |
| F30                       | C1 (Positive Lead)       |                |                                               |               | (100 µF electrolytic)                     |
| F29                       | C1 (Negative Lead)       |                |                                               |               | (100 µF electrolytic)                     |
| C29                       | C2                       |                |                                               |               | (0.1 µF ceramic)                          |
| C30                       | C2                       |                |                                               |               | (0.1 µF ceramic)                          |
| I14 ↔ Right-side GND Rail |                          |                |                                               |               | Jumper                                    |
| D14 ↔ G14                 | S1 (Push Button)         |                |                                               |               | INPUT_PULLUP; active-low                  |
| D12 ↔ G12                 | S1 (Push Button)         |                |                                               |               | To Right-side GND Rail                    |
| C6                        | R1 (220 Ω)               |                |                                               |               | 220 Ω                                     |
| Right-side GND Rail       | R1 (220 Ω)               |                |                                               |               | 220 Ω                                     |
| E5                        | LED1 (Anode, +)          |                |                                               |               | Green LED                                 |
| E6                        | LED1 (Cathode, -)        |                |                                               |               | Green LED                                 |
| Right-side VCC Rail       | Battery Pack (+)         |                |                                               |               | 4×AA (~4.8 V)                             |
| Right-side GND Rail       | Battery Pack (-)         |                |                                               |               | 4×AA (~4.8 V)                             |

---

**Notes:**

- The ULN2003 driver board acts as an interface between the Arduino and the stepper motor, providing the necessary current and voltage.
- **Push button:** one leg → **I12** (connected to **D2** via jumper), other leg → **I14** → **GND** (active-low using Arduino’s `INPUT_PULLUP`).
- The LED includes a current-limiting resistor (R1, typically 220Ω) in series with the cathode to prevent damage.
- Ensure all grounds (Arduino, ULN2003, button, LED, battery pack) are common to avoid erratic behavior.
- Decoupling capacitors C1 (100 µF electrolytic) located at F30 (positive lead) and F29 (negative lead) across ULN2003 VCC and GND nodes, and C2 (0.1 µF ceramic) located at C30/C29 across the same VCC–GND nodes.

---

### Power Source

- Arduino Uno is powered via USB Battery Pack (10,000 mAh / 37 Wh).
- Stepper motor/ULN2003 are powered by a dedicated **4×AA NiMH pack (~4.8 V nominal, 2400 mAh)** on the **right-side rails** (Positive → right VCC rail, Negative → right GND rail).
- All grounds are common: right-side GND rail is tied to Arduino GND (common reference).
- Decoupling: C1 (100 µF) and C2 (0.1 µF) placed across ULN2003 VCC–GND at A30/A29 and C30/C29 respectively.
