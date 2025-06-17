#include <Stepper.h>

// Constants
const int stepsPerRevolution = 2048; // for 28BYJ-48 stepper motor
const float motorSpeedRPM = 0.0042;  // For 1 rev every 4 hours
const int buttonPin = 2;             // Button input pin
const int ledPin = 3;                // LED control pin

// Create Stepper instance (pin order: IN1, IN3, IN2, IN4)
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

// State tracking
bool motorOn = false;
int lastButtonState = HIGH;
unsigned long lastStepTime = 0;
const unsigned long stepIntervalMicros = (unsigned long)(1000000.0 / (motorSpeedRPM * stepsPerRevolution / 60.0)); // µs per step

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up
  pinMode(ledPin, OUTPUT);          // Set LED pin as output
  digitalWrite(ledPin, LOW);        // Ensure LED starts off
  Serial.begin(9600);
  myStepper.setSpeed(5);            // Required by library, not used in timed stepping
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // Toggle motor on button press
  if (buttonState == LOW && lastButtonState == HIGH) {
    motorOn = !motorOn;
    Serial.println(motorOn ? "Motor ON" : "Motor OFF");

    // LED matches motor state
    digitalWrite(ledPin, motorOn ? HIGH : LOW);

    delay(250); // Debounce delay
  }

  lastButtonState = buttonState;

  // Run motor at ultra-slow interval
  if (motorOn && (micros() - lastStepTime >= stepIntervalMicros)) {
    myStepper.step(1);  // Take a single step
    lastStepTime = micros();
  }
}