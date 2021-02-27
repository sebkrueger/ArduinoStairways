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

const int ledPins[] = {9, 10, 11};       // the LED pin with pwd
const int pinCount = 3;                  // Count of Pin Array

// Variables for button and LED state
int pin = 0;              // Index var  

int ledState = 0;            // start with dark LEDs

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  pinMode(buttonPin, INPUT);
  for(pin=0;pin<pinCount;pin++) {
    pinMode(ledPins[pin], OUTPUT);
    // set initial LED state
    digitalWrite(ledPins[pin], ledState);
  }
  
  // init Serial connection for debug
  Serial.begin(9600);
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

        Serial.println("Button pressed");
        
        if (ledState > 0) {
          ledState = false;
                
          for(pin=pinCount;pin>0;pin--) {
             // LED off
             fadeLedOut(ledPins[pin-1]);
           }
          
        } else {
          ledState = true; 

          for(pin=0;pin<pinCount;pin++) {
             // LED off
             fadeLedIn(ledPins[pin]);
           }
        }
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

// Function that fade LED in 
void fadeLedIn(int ledFadePin) {
  for (float i = 1; i <= 255; i = i * 2.3) {
     // set the LED with fade in to high
     analogWrite(ledFadePin, int(i));
     delay(50);
  }

  analogWrite(ledFadePin, 255);  
}

// Function that fade LED out 
void fadeLedOut(int ledFadePin) {
  for (float i = 255; i > 0.5; i = i / 2.3) {
     // set the LED with fade out to low
     analogWrite(ledFadePin, i);
     delay(50);
  }

  analogWrite(ledFadePin, 0);  
}

