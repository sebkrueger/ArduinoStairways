/*
  n02_brightness

  Read value from analoge pin 5

  The circuit:
   Bightnesssensor attached on analoge pin 5

  created 14 December 2015
  by Sebastian Krüger

  This example code is in the public domain.
*/

// Constants for pin numbers
const int lightsensorPin = 5;            // the number of the sensor pin

void setup() {
  // init Serial connection for debug
  Serial.begin(9600);
}

void loop() {
  // read the state of the switch into a local variable:
  int lightvalue = analogRead(lightsensorPin);

  Serial.print("Lightvalue: ");
  Serial.println(lightvalue); 

  delay(1000);

}

