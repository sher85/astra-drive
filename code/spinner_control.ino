// Astra Drive
// Supports today's ULN2003+28BYJ-48 and a future STEP/DIR (e.g., NEMA‑17 + A4988/DRV8825)
// Paste directly into the Arduino IDE.

#include <AccelStepper.h>
#include <EEPROM.h>

// ========================= DRIVER SELECTION =========================
// Pick ONE of the following by uncommenting it. Default: ULN2003 28BYJ‑48.
#define DRIVER_ULN2003_28BYJ
// #define DRIVER_STEP_DIR  // e.g., NEMA‑17 + A4988/DRV8825/STEP-DIR driver
// ===================================================================

// ========================= SPEED PRESETS ============================
// Human-friendly speeds (one revolution per X time). Change PRESET.
enum SpeedPreset { REV_4H, REV_2H, REV_1MIN, REV_10S };
const uint8_t PRESET_COUNT = 4;
const SpeedPreset PRESET_SEQUENCE[PRESET_COUNT] = {
  REV_10S,
  REV_1MIN,
  REV_2H,
  REV_4H
};
SpeedPreset PRESET = REV_10S;  // <— bench-test friendly default; change this for the final display speed

const int PRESET_EEPROM_ADDR = 0;
const unsigned long longPressMs = 900;

static inline float rpmForPreset(SpeedPreset p) {
  switch (p) {
    case REV_4H:   return 60.0f / 14400.0f; // 4 hours
    case REV_2H:   return 60.0f /  7200.0f; // 2 hours
    case REV_1MIN: return 1.0f;             // 1 minute
    case REV_10S:  return 6.0f;             // 10 seconds
    default:       return 60.0f / 14400.0f;
  }
}
// ===================================================================

// ========================= IO PINS ==================================
const int buttonPin = 2;  // momentary to GND; uses INPUT_PULLUP
const int ledPin    = 3;  // status LED mirrors motor state

#if defined(DRIVER_ULN2003_28BYJ)
  // ULN2003 + 28BYJ‑48 wiring
  // Keep physical wiring sequential IN1→IN2→IN3→IN4 on the board.
  // AccelStepper’s HALF4WIRE needs a *software* sequence; different boards sometimes want a different order.
  // Set ULN2003_PIN_SEQUENCE to one of: 1324, 1234, 1423, 1243
  #define ULN2003_PIN_SEQUENCE 1324

  const int IN1_PIN = 8;   // ULN2003 IN1 → Arduino D8
  const int IN2_PIN = 9;   // ULN2003 IN2 → Arduino D9
  const int IN3_PIN = 10;  // ULN2003 IN3 → Arduino D10
  const int IN4_PIN = 11;  // ULN2003 IN4 → Arduino D11

  #if   ULN2003_PIN_SEQUENCE == 1324
    AccelStepper stepper(AccelStepper::HALF4WIRE, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);
    const char* SEQ_LABEL = "1,3,2,4";
  #elif ULN2003_PIN_SEQUENCE == 1234
    AccelStepper stepper(AccelStepper::HALF4WIRE, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
    const char* SEQ_LABEL = "1,2,3,4";
  #elif ULN2003_PIN_SEQUENCE == 1423
    AccelStepper stepper(AccelStepper::HALF4WIRE, IN1_PIN, IN4_PIN, IN2_PIN, IN3_PIN);
    const char* SEQ_LABEL = "1,4,2,3";
  #elif ULN2003_PIN_SEQUENCE == 1243
    AccelStepper stepper(AccelStepper::HALF4WIRE, IN1_PIN, IN2_PIN, IN4_PIN, IN3_PIN);
    const char* SEQ_LABEL = "1,2,4,3";
  #else
    #error "Invalid ULN2003_PIN_SEQUENCE (use 1324, 1234, 1423, or 1243)"
  #endif
#elif defined(DRIVER_STEP_DIR)
  // STEP/DIR driver (A4988/DRV8825/TMC)—adjust pins to your wiring
  const int STEP_PIN = 8;
  const int DIR_PIN  = 9;
  AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
#else
  #error "Select a driver: define DRIVER_ULN2003_28BYJ or DRIVER_STEP_DIR"
#endif
// ===================================================================

// ========================= MOTOR COUNTS =============================
// Define how many *commanded* steps equal one output-shaft revolution.
// ULN2003+28BYJ-48 with HALF4WIRE runs half-steps; typical gearbox ≈ 1:64.
//  - 28BYJ full-step count ≈ 2048 steps/rev
//  - 28BYJ half-step count ≈ 4096 steps/rev (smoother, our default)
// For STEP/DIR drivers, effective steps = motor_base_steps * MICROSTEP.
#if defined(DRIVER_ULN2003_28BYJ)
  const long STEPS_PER_REV = 4096L;  // half-step equivalent for most 28BYJ-48 sets
#elif defined(DRIVER_STEP_DIR)
  const int  BASE_STEPS_PER_REV = 200;  // NEMA‑17 1.8° = 200 full steps/rev
  const int  MICROSTEP          = 16;   // match your driver jumpers (1,2,4,8,16,32…)
  const long STEPS_PER_REV      = (long)BASE_STEPS_PER_REV * MICROSTEP;
#endif
// ===================================================================

// ========================= STATE ====================================
bool motorOn = false;
int  lastButton = HIGH;
int  stableButton = HIGH;
unsigned long lastDebounceMs = 0;
unsigned long buttonPressedAtMs = 0;
const unsigned long debounceMs = 25;

// Derived speed (steps per second)
float rpm = 0.0f;
float sps = 0.0f;  // steps per second
// ===================================================================

static inline bool isValidPresetByte(uint8_t raw) {
  return raw < PRESET_COUNT;
}

const char* labelForPreset(SpeedPreset p) {
  switch (p) {
    case REV_4H:   return "REV_4H";
    case REV_2H:   return "REV_2H";
    case REV_1MIN: return "REV_1MIN";
    case REV_10S:  return "REV_10S";
    default:       return "UNKNOWN";
  }
}

uint8_t sequenceIndexForPreset(SpeedPreset p) {
  for (uint8_t i = 0; i < PRESET_COUNT; ++i) {
    if (PRESET_SEQUENCE[i] == p) return i;
  }
  return 0;
}

uint8_t blinkCountForPreset(SpeedPreset p) {
  return sequenceIndexForPreset(p) + 1u;
}

void savePresetToEeprom() {
  EEPROM.update(PRESET_EEPROM_ADDR, (uint8_t)PRESET);
}

void loadPresetFromEeprom() {
  uint8_t raw = EEPROM.read(PRESET_EEPROM_ADDR);
  if (isValidPresetByte(raw)) {
    PRESET = (SpeedPreset)raw;
  }
}

void restoreLedState() {
  digitalWrite(ledPin, motorOn ? HIGH : LOW);
}

void blinkSelectedPreset() {
  const uint8_t count = blinkCountForPreset(PRESET);
  for (uint8_t i = 0; i < count; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(170);
    digitalWrite(ledPin, LOW);
    delay((i + 1u < count) ? 170 : 280);
  }
  restoreLedState();
}

void printActivePreset() {
  Serial.print("Preset:        ");
  Serial.println(labelForPreset(PRESET));
  Serial.print("Preset RPM:    ");
  Serial.println(rpm, 6);
  Serial.print("Steps/sec:     ");
  Serial.println(sps, 6);
}

void toggleMotorState() {
  motorOn = !motorOn;
  restoreLedState();
  Serial.println(motorOn ? "Motor ON" : "Motor OFF");

  if (motorOn) {
    stepper.enableOutputs();
  } else {
    stepper.disableOutputs();
  }
}

void advancePreset() {
  uint8_t nextIndex = (uint8_t)((sequenceIndexForPreset(PRESET) + 1u) % PRESET_COUNT);
  PRESET = PRESET_SEQUENCE[nextIndex];
  configureSpeedFromPreset();
  savePresetToEeprom();
  Serial.println("[BUTTON] Preset changed.");
  printActivePreset();
  blinkSelectedPreset();
}

#if defined(DRIVER_ULN2003_28BYJ)
void coilSelfTest(unsigned long dwellMs = 400) {
  Serial.println("[TEST] Starting ULN2003 coil self-test (IN1→IN2→IN3→IN4)");
  stepper.disableOutputs();
  // Ensure inputs are OUTPUTs
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  auto allLow = [](){
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
  };
  allLow();
  // Energize each coil one by one so board LEDs should light 1→2→3→4
  digitalWrite(IN1_PIN, HIGH); Serial.println("[TEST] IN1 HIGH"); delay(dwellMs); allLow(); delay(100);
  digitalWrite(IN2_PIN, HIGH); Serial.println("[TEST] IN2 HIGH"); delay(dwellMs); allLow(); delay(100);
  digitalWrite(IN3_PIN, HIGH); Serial.println("[TEST] IN3 HIGH"); delay(dwellMs); allLow(); delay(100);
  digitalWrite(IN4_PIN, HIGH); Serial.println("[TEST] IN4 HIGH"); delay(dwellMs); allLow();
  Serial.println("[TEST] Coil self-test complete. Re-enabling stepper outputs.");
  stepper.enableOutputs();
}
#endif

void configureSpeedFromPreset() {
  rpm = rpmForPreset(PRESET);
  sps = rpm * (float)STEPS_PER_REV / 60.0f;  // AccelStepper expects steps/second
  if (sps < 0.0001f) sps = 0.0001f;
  if (sps > 5000.0f) sps = 5000.0f;

  stepper.setMaxSpeed(max(1000.0f, sps * 2.0f));
  stepper.setAcceleration(0.0f);
  stepper.setSpeed(sps);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);

  loadPresetFromEeprom();
  configureSpeedFromPreset();
  stepper.disableOutputs(); // coils off when not running (cool & quiet)

  // --- Console summary ---
#if defined(DRIVER_ULN2003_28BYJ)
  Serial.println("Driver: ULN2003 + 28BYJ-48 (HALF4WIRE)");
#elif defined(DRIVER_STEP_DIR)
  Serial.print("Driver: STEP/DIR  (microstep = "); Serial.print(MICROSTEP); Serial.println(")");
#endif
  Serial.print("STEPS_PER_REV: "); Serial.println(STEPS_PER_REV);
#if defined(DRIVER_ULN2003_28BYJ)
  Serial.print("Pin sequence:  "); Serial.println(SEQ_LABEL);
#endif
  printActivePreset();
  #if defined(DRIVER_ULN2003_28BYJ)
    Serial.println("[HINT] Send 't' in Serial Monitor to run coil self-test.");
  #endif
  Serial.println("[HINT] Short press button to toggle motor on/off.");
  Serial.println("[HINT] Long press while stopped to cycle presets (saved to EEPROM).");
  blinkSelectedPreset();
}

void loop() {
  // --- Serial commands (diagnostics) ---
  if (Serial.available()) {
    int c = Serial.read();
    if (c == 't' || c == 'T') {
#if defined(DRIVER_ULN2003_28BYJ)
      coilSelfTest();
#else
      Serial.println("[TEST] Coil self-test only available for ULN2003 build.");
#endif
    }
  }
  // --- Debounced button toggle ---
  int reading = digitalRead(buttonPin);
  if (reading != lastButton) {
    lastDebounceMs = millis();
  }
  if (millis() - lastDebounceMs > debounceMs) {
    if (reading != stableButton) {
      stableButton = reading;
      if (stableButton == LOW) {
        buttonPressedAtMs = millis();
      } else {
        unsigned long heldMs = millis() - buttonPressedAtMs;
        if (heldMs >= longPressMs) {
          if (motorOn) {
            Serial.println("[BUTTON] Stop the motor first to change presets.");
          } else {
            advancePreset();
          }
        } else {
          toggleMotorState();
        }
      }
    }
  }
  lastButton = reading;

  // --- Run motor when ON (non-blocking) ---
  if (motorOn) {
    stepper.runSpeed();
  }
}
