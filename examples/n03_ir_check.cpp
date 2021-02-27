/*
  n03_ir_check.cpp

  Check if one of the two PIR-Sensor trigger and 
  let the upper or lower LED stripe light up for some time

  The circuit:
   - upstairs PIR attached to digital input 2
   - upstairs PIR attached to digital input 2
   - PCA9685 LED-Driver hat attached and LED stripe at port 1-13

  created 14 December 2015
  by Sebastian Krüger

  This example code is in the public domain.
*/

#include <Arduino.h>
//#include <Wire.h>
#include "PCA9685.h"

PCA9685 ledDriver;

// Constants for pin numbers
const int PIR_UPPSTAIRS = 2;
const int PIR_DOWNSTAIRS = 4;

void allLedOff()
{
    // Dimm all LEDs
    for (int step=0; step<15; step++)
    {
        ledDriver.setLEDDimmed(step, 100);
    }
}

void setup() 
{
    pinMode(PIR_UPPSTAIRS, INPUT);
    pinMode(PIR_DOWNSTAIRS, INPUT);

    Wire.begin();              // Wire must be started!
    ledDriver.begin(B101000);  // Address pins A5-A0 set to B101000
    ledDriver.init();

    Serial.begin(19200);
    Serial.println("Init ready!");

    // Launch short test that first and last stair are function correct
    allLedOff();

    // give some time for user to get into watch position
    delay(10000);

    // check all LED by activate them step by step
    for (int step=0; step<13; step++) {
      ledDriver.setLEDDimmed(step, 0);  // stair ON
      delay(1000);
    }
    
    // some more time to let the PIR warm-up 
    delay(30000);

    // after init all LED off
    allLedOff();
}

void loop() 
{
  // Check if IR has fired
  if (digitalRead(PIR_UPPSTAIRS) == 1 ||  digitalRead(PIR_DOWNSTAIRS) == 1) {
      // ok we detect something
      delay(100);  // make a short debounce check
      if (digitalRead(PIR_UPPSTAIRS) == 1 ||  digitalRead(PIR_DOWNSTAIRS) == 1) {
        // ok we got reading looks like a real trigger
        if (digitalRead(PIR_UPPSTAIRS) == 1) {
          // set top LED for 1 sek to lit 
          ledDriver.setLEDDimmed(0, 0);
          
          // go up until 5 sek. or pin is going down again
          for(int i=0; i<5; i++) {
            delay(1000);
            if (digitalRead(PIR_UPPSTAIRS) == 0) {
               break; 
            }
          }

          ledDriver.setLEDDimmed(0, 100);

        } else if (digitalRead(PIR_DOWNSTAIRS) == 1) {
          // set bottom LED for 1 sek to lit 
          ledDriver.setLEDDimmed(12, 0);
          
          // go up until 5 sek. or pin is going down again
          for(int i=0; i<5; i++) {
            delay(1000);
            if (digitalRead(PIR_DOWNSTAIRS) == 0) {
               break; 
            }
          }
          
          ledDriver.setLEDDimmed(12, 100);
        }
      }
  }

  delay(500);
}
