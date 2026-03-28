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
|                           |                          |                | Stepper motor output connector (5-pin JST-XH-style) | 28BYJ-48 |                                           |
| C5                        |                          | D3             |                                               |               | Jumper from D3 to the LED drive node       |
| I12                       |                          | D2             |                                               |               | Upper-right button leg lands here; button signal node on the right half |
| Right-side GND Rail       |                          | GND            |                                               |               |                                           |
| J30 ↔ Right-side VCC Rail |                          |                |                                               |               | Jumper                                    |
| J29 ↔ Right-side GND Rail |                          |                |                                               |               | Jumper                                    |
| H30                       | C1 (Positive Lead)       |                |                                               |               | (100 µF electrolytic)                     |
| H29                       | C1 (Negative Lead)       |                |                                               |               | (100 µF electrolytic)                     |
| C29                       | C2                       |                |                                               |               | (0.1 µF ceramic)                          |
| C30                       | C2                       |                |                                               |               | (0.1 µF ceramic)                          |
| E30 ↔ F30                 |                          |                |                                               |               | Bridges row 30 across the center gap for the C1/VCC node |
| E29 ↔ F29                 |                          |                |                                               |               | Bridges row 29 across the center gap for the C1/GND node |
| I14 ↔ Right-side GND Rail | GND jumper               |                |                                               |               | Lower-right button leg lands here; jumper ties the ground-side button node to the right-side GND rail |
| C6 ↔ Right-side GND Rail  | R1 (220 Ω)               |                |                                               |               | Series resistor from the LED cathode return node to ground |
| E5                        | LED1 (Anode, +)          |                |                                               |               | Long leg; connects to the D3-driven node   |
| E6                        | LED1 (Cathode, -)        |                |                                               |               | Short leg; returns through R1 to ground    |
| Right-side VCC Rail       | Battery Pack (+)         |                |                                               |               | 4×AA (~4.8 V)                             |
| Right-side GND Rail       | Battery Pack (-)         |                |                                               |               | 4×AA (~4.8 V)                             |

---

**Notes:**

- The ULN2003 driver board acts as an interface between the Arduino and the stepper motor, providing the necessary current and voltage.
- The 28BYJ-48 plugs into the ULN2003 board's 5-pin motor connector; the Arduino only connects to the ULN2003 control inputs.
- **Control inputs:** the diagram matches the sketch's expected physical order: **D8 → IN1**, **D9 → IN2**, **D10 → IN3**, **D11 → IN4**.
- **Pushbutton placement:** `S1` straddles the breadboard center gap across rows **12** and **14**.
- **Push button:** the switch straddles the breadboard center gap across rows **12** and **14**. The Arduino input connects at **I12**, and the grounded side connects at **I14**, both on the **right half** of the breadboard. Do not move either connection to the left half unless you add a jumper across the center gap.
- **Pushbutton orientation caution:** this layout only works if the pushbutton is oriented exactly as shown in the diagram. If you rotate the button 90 degrees, the internal contacts may not behave the way this table assumes. Check the switch continuity with a multimeter if there is any doubt.
- The LED includes a current-limiting resistor (R1, 220–330Ω recommended) in series with the cathode to prevent damage.
- Ensure all grounds (Arduino, ULN2003, button, LED, battery pack) are common to avoid erratic behavior.
- Wiring to the ULN2003 board is sequential IN1 → IN2 → IN3 → IN4; the AccelStepper library internally handles coil sequencing by using a 1,3,2,4 software order.
- Decoupling capacitors C1 (100 µF electrolytic) located at H30 (positive lead) and H29 (negative lead), with bridge jumpers **E30 ↔ F30** and **E29 ↔ F29** so the capacitor still spans the same ULN2003 VCC and GND nodes across the breadboard center gap. C2 (0.1 µF ceramic) remains at C30/C29 across the same VCC–GND nodes. Place these capacitors physically close to the ULN2003 board's VCC and GND pins for best stability.
- The wire "humps" in the diagram indicate wires crossing without electrical connection.
- **Breadboard rail caution:** some breadboards split long power rails internally. Verify continuity on the right-side VCC and GND rails with a multimeter if your board layout differs from the diagram or if the motor/LED behaves intermittently.

---

### Power Source

- Arduino Uno is powered via USB Battery Pack (10,000 mAh / 37 Wh).
- Stepper motor/ULN2003 are powered by a dedicated **4×AA NiMH pack (~4.8 V nominal, 2400 mAh)** on the **right-side rails** (Positive → right VCC rail, Negative → right GND rail).
- All grounds are common: right-side GND rail is tied to Arduino GND (common reference).
- Decoupling: C1 (100 µF) is physically placed at H30/H29 and tied across the same supply nodes with E30 ↔ F30 and E29 ↔ F29 bridge jumpers; C2 (0.1 µF) is placed at C30/C29.
