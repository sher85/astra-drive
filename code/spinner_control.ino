#include <AccelStepper.h>

// Constants
const int stepsPerRevolution = 2048; // for 28BYJ-48 stepper motor
const int motorSpeedRPM = 5;         // Desired RPM
const int buttonPin = 2;             // Button input pin
const int ledPin = 3;                // LED control pin

// Create stepper instance (pin order: IN1, IN3, IN2, IN4)
AccelStepper myStepper(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

bool motorOn = false;
int lastButtonState = HIGH;

void setup() {
  myStepper.setMaxSpeed(motorSpeedRPM * stepsPerRevolution / 60.0);
  myStepper.setSpeed(motorSpeedRPM * stepsPerRevolution / 60.0);
  pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up
  pinMode(ledPin, OUTPUT);          // Set LED pin as output
  digitalWrite(ledPin, LOW);        // Ensure LED starts off
  Serial.begin(9600);
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

  if (motorOn) {
    myStepper.runSpeed();
  }
}