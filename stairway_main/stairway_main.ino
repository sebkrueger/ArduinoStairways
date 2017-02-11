/*
  stairway_main

  Read in two input pins to detect top or bottom IR-Sensor trigger 
  After that it switch on the leds from the direction the trigger was set
   
  created 14 December 2015
  by Sebastian Krüger
  This code is in the public domain.
*/

#include <Wire.h>
#include <Bounce2.h>
#include <PCA9685.h>

PCA9685 ledDriver;

#define TOP_BUTTON_PIN 2
#define BOTTOM_BUTTON_PIN 4

const int stepCount = 8; // Count of all Stairway steps

Bounce debouncer_top = Bounce();
Bounce debouncer_bottom = Bounce();

void setup()
{
    // Attach the top and bottom inputs
    pinMode(TOP_BUTTON_PIN, INPUT);
    pinMode(BOTTOM_BUTTON_PIN, INPUT);

    debouncer_top.attach(TOP_BUTTON_PIN);
    debouncer_top.interval(10); // interval in ms
    debouncer_bottom.attach(BOTTOM_BUTTON_PIN);
    debouncer_bottom.interval(10); // interval in ms

    Wire.begin();              // Wire must be started!
    ledDriver.begin(B101000);  // Address pins A5-A0 set to B101000
    ledDriver.init();

    // init Serial connection for debug
    Serial.begin(19200);

    Serial.println("Init ready!");
}

void loop()
{
    debouncer_top.update();   
    if(debouncer_top.fell())
    {
        Serial.println("Red Button pressed");
        riseSteps(stepCount-1, 0, 50);
        delay(2000);
        allLedOff();
    }

    debouncer_bottom.update();   
    if(debouncer_bottom.fell())
    {
        Serial.println("Black Button pressed");        
        // Flash all LEDs up
        riseSteps(0, stepCount-1, 50);
        delay(2000);
        allLedOff(); 
    }
}

// Flash up LED in row 
// begin : step to begin 
// end : last step 
void riseSteps(int begin, int end, int delaytime) 
{      
    int steps;           // Count of steps that between begin and end
    int step;            // Pointer to step array with led level
    int decIncPointer;   // Show point in array until we decrmenet/incremnet in last round

    int stepIndex;       // Pointer to step on LED Driverboard
    int direction;       // Indcate if we have to go up or down
    
    if(end>=begin) 
    {
        steps = end-begin+1;    // Rember begin and end are le position on LED driver board
        direction = 1;
    } 
    else 
    {
        steps = begin-end+1;    // Rember begin and end are le position on LED driver board
        direction = -1;
    }

    // Init Array to store the actual LED VALUE
    int stepLevel[steps];

    // set all level to 100
    for(step=0; step<steps; step++)
    {
        stepLevel[step] = 100;
    }

    // Start loop that decrement and shift a falling value over all array values, until they are all zero
    decIncPointer = 1; // Start at the first element
    while(stepLevel[steps-1]>0) {
        // go on until last LED is on top level

        // Start Loop but step to step, in every loop one step more until max step count
        for(step=0; step<decIncPointer; step++) 
        {
            if(stepLevel[step]>0) 
            {
                // decrement value if not zero
                stepLevel[step] = stepLevel[step] - 25;
            }

            if(stepLevel[step]<0)
            {
                // make shure we are not below zero, in this case set value to zero
                stepLevel[step] = 0;
            }
            
        }
        
        /* Debug write array content to Serial
        String debug = "";
    
        for(step=0; step<steps; step++) {
            debug =  debug  + " " + step + ":" + stepLevel[step];
        }
        Serial.println(debug); */

        // Always start with the first step
        stepIndex = begin;
        
        
        // Write all the array values to LED driver                   
        for(step=0; step<steps; step++)
        {
            ledDriver.setLEDDimmed(stepIndex, stepLevel[step]);
            stepIndex = stepIndex + direction;
        }

        // Make a short stop 
        delay(delaytime);
          
        // take the next step until are on the last step
        if(decIncPointer<steps) {
            decIncPointer++;
        }
    }
}

void allLedOff()
{
    // Dimm all LEDs
    for(int step=0; step<stepCount; step++)
    {
        ledDriver.setLEDDimmed(step, 100);
    }
}

