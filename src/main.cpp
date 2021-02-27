/*
  Arduino Stairways / main.cpp

  Read in two input pins to detect top or bottom IR-Sensor trigger 
  After that it switch on the leds from the direction the trigger was set

  The circuit:
   - upstairs PIR attached to digital input 2
   - upstairs PIR attached to digital input 2
   - PCA9685 LED-Driver hat attached and LED stripe at port 1-13

  created 14 December 2015
  by Sebastian Krüger

  This code is in the public domain.
*/

#include <Arduino.h>
#include "PCA9685.h"

PCA9685 ledDriver;

// Constants for pin numbers
const int PIR_UPSTAIRS = 2;
const int PIR_DOWNSTAIRS = 4;

const int STAIR_COUNT = 13;   // max. by driver is 16

// some timing constant
const int TRIGGER_TIME = 0;  // extend the debounce check in millisec.
const int LIGHT_UP_TIME = 12; // this time will stairs light up after last trigger

int currentLightUpTime;  // how long are all LED up
int lastStartDirection;  // to switch off in correct direction

/**
 * Helper function to set all stairs to off
 */
void allLedOff()
{
    // Dimm all LEDs
    for (int step=0; step<STAIR_COUNT; step++)
    {
        ledDriver.setLEDDimmed(step, 100);
    }
}

/**
 * Light up or dim all LED Steps in a row to make a "snake" effect
 * 
 * direction - 'u'=up-stairs / 'd'=down-stairs
 * light     - 'l'=light-up / 'o'=swith-off   
 * 
 */
void lightAndDimmRow(char direction, char light) 
{
    int lightLevel = 100;  // default off
    if (light == 'l') {
        lightLevel = 0;    // light switch on
    }

    for (int step = 0; step < STAIR_COUNT; step++) {
        int currentStep = step;
        if (direction == 'd') {
            // reverse to down-strairs 
            currentStep = STAIR_COUNT - step - 1;  // step index start at zero
        }

        ledDriver.setLEDDimmed(currentStep, lightLevel); 
        delay(50);
    }
}

/**
 * Setup and init functions
 */
void setup() 
{
    pinMode(PIR_UPSTAIRS, INPUT);
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

    currentLightUpTime = 0;
}

/**
 * Main loop
 */
void loop() 
{
  // Check if IR has fired
  if (digitalRead(PIR_UPSTAIRS) == 1 ||  digitalRead(PIR_DOWNSTAIRS) == 1) {
      // ok we detect something/someone
      delay(100);  // make a short debounce check

      // only go on, if LED are switch to off
      if (currentLightUpTime == 0) {
        if (TRIGGER_TIME > 0) {
          delay(TRIGGER_TIME);  // extend the debounce check 
        }

        if (digitalRead(PIR_UPSTAIRS) == 1 ||  digitalRead(PIR_DOWNSTAIRS) == 1) {
          // ok we got reading looks like a real trigger

          char direction = 'd';            
          if (digitalRead(PIR_UPSTAIRS) == 1) {
            // if top step trigger, we have to go upstairs
            direction = 'u';
          }

          lastStartDirection = direction;
          lightAndDimmRow(direction, 'l'); 
        }

        // set the timer
        currentLightUpTime = LIGHT_UP_TIME;
      }   
  }

  if (currentLightUpTime > 0) {
     currentLightUpTime--;
     if (currentLightUpTime <= 0) {
         // sitch of all LED and start in last trigger direction
        lightAndDimmRow(lastStartDirection, 'o');
     }
  }

  // don't change this ... otherwise it will mess-up LED on timing
  delay(1000);
}
