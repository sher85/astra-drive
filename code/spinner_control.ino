// MilkyWay Spinner – refactor-ready motor controller
// Supports today's ULN2003+28BYJ-48 and a future STEP/DIR (e.g., NEMA‑17 + A4988/DRV8825)
// Paste directly into the Arduino IDE.

#include <AccelStepper.h>

// ========================= DRIVER SELECTION =========================
// Pick ONE of the following by uncommenting it. Default: ULN2003 28BYJ‑48.
#define DRIVER_ULN2003_28BYJ
// #define DRIVER_STEP_DIR  // e.g., NEMA‑17 + A4988/DRV8825/STEP-DIR driver
// ===================================================================

// ========================= SPEED PRESETS ============================
// Human-friendly speeds (one revolution per X time). Change PRESET.
enum SpeedPreset { REV_4H, REV_2H, REV_1MIN, REV_10S };
SpeedPreset PRESET = REV_10S;  // <— change this to pick a different speed

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
  // ULN2003 + 28BYJ‑48 wiring (AccelStepper expects pins in 1,3,2,4 order)
  // Keep physical wiring sequential 1→2→3→4 and express sequence in software
  const int IN1_PIN = 8;   // ULN2003 IN1 → Arduino D8
  const int IN2_PIN = 9;   // ULN2003 IN2 → Arduino D9
  const int IN3_PIN = 10;  // ULN2003 IN3 → Arduino D10
  const int IN4_PIN = 11;  // ULN2003 IN4 → Arduino D11
  AccelStepper stepper(AccelStepper::HALF4WIRE, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);
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
unsigned long lastDebounceMs = 0;
const unsigned long debounceMs = 25;

// Derived speed (steps per second)
float rpm = 0.0f;
float sps = 0.0f;  // steps per second
// ===================================================================

void configureSpeedFromPreset() {
  rpm = rpmForPreset(PRESET);
  sps = rpm * (float)STEPS_PER_REV / 60.0f;  // AccelStepper expects steps/second
  // Reasonable safety limits to avoid nonsense if counts are mis-set
  if (sps < 0.0001f) sps = 0.0001f;
  if (sps > 5000.0f) sps = 5000.0f; // protect against accidental super-high speeds
  stepper.setMaxSpeed( max(1000.0f, sps * 2.0f) ); // headroom over target speed
  stepper.setAcceleration(0.0f); // constant speed for long exposures
  stepper.setSpeed(sps);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);

  configureSpeedFromPreset();
  stepper.disableOutputs(); // coils off when not running (cool & quiet)

  // --- Console summary ---
  Serial.println("MilkyWay Spinner – Refactor-Ready");
#if defined(DRIVER_ULN2003_28BYJ)
  Serial.println("Driver: ULN2003 + 28BYJ-48 (HALF4WIRE)");
#elif defined(DRIVER_STEP_DIR)
  Serial.print("Driver: STEP/DIR  (microstep = "); Serial.print(MICROSTEP); Serial.println(")");
#endif
  Serial.print("STEPS_PER_REV: "); Serial.println(STEPS_PER_REV);
  Serial.print("Preset RPM:    "); Serial.println(rpm, 6);
  Serial.print("Steps/sec:     "); Serial.println(sps, 6);
}

void loop() {
  // --- Debounced button toggle ---
  int reading = digitalRead(buttonPin);
  if (reading != lastButton) {
    lastDebounceMs = millis();
  }
  if (millis() - lastDebounceMs > debounceMs) {
    static int lastStable = HIGH;
    if (reading != lastStable) {
      lastStable = reading;
      if (reading == LOW) {
        motorOn = !motorOn;
        digitalWrite(ledPin, motorOn ? HIGH : LOW);
        Serial.println(motorOn ? "Motor ON" : "Motor OFF");
        if (motorOn) stepper.enableOutputs(); else stepper.disableOutputs();
      }
    }
  }
  lastButton = reading;

  // --- Run motor when ON (non-blocking) ---
  if (motorOn) {
    stepper.runSpeed();
  }
}