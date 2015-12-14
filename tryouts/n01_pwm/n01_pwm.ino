/*
  n01_pwm

  Use a button on port 2 to trigger LED feade in on 3 Ports (9-11) in a row

  The circuit:
   LEDs attached from pin 9-11 to ground
   pushbutton attached from pin 2 to +5V
   10K resistor attached from pin 2 to ground

  created 12 December 2015
  by Sebastian Krüger

  This example code is in the public domain.

*/

// Constants for pin numbers
const int buttonPin = 2;    // the number of the pushbutton pin

const int ledPin = 9;       // the number of the LED pin

// Variables for button and LED state
int ledState = 0;            // start with dark LEDs

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}


void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {

        if (ledState > 0) {
          ledState = 0;
          digitalWrite(ledPin, ledState);
        } else {
          for (float i = 1; i <= 255; i = i * 1.3) {
            // set the LED with fade in to high
            analogWrite(ledPin, int(i));
            delay(10);
          }
          ledState = 255;
        }
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

