#include <Stepper.h>

// Constants
const int stepsPerRevolution = 2048; // for 28BYJ-48 stepper motor
const int motorSpeedRPM = 15;         // Desired RPM
const int buttonPin = 2;             // Button input pin

// Create stepper instance (pin order: IN1, IN3, IN2, IN4)
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

bool motorOn = false;
int lastButtonState = HIGH;

void setup() {
  myStepper.setSpeed(motorSpeedRPM);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // Toggle motor on button press
  if (buttonState == LOW && lastButtonState == HIGH) {
    motorOn = !motorOn;
    Serial.println(motorOn ? "Motor ON" : "Motor OFF");
    delay(250); // Debounce delay
  }

  lastButtonState = buttonState;

  if (motorOn) {
    myStepper.step(1); // Step 1 step at a time
    // Timing is handled automatically by setSpeed()
  }
}