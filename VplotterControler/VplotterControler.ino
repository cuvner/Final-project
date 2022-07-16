/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.


  This example code is in the public domain.
  https://www.arduino.cc/reference/en/language/functions/communication/serial/parseint/
  https://eecs.blog/sending-multiple-values-over-serial-to-arduino/
  https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
  https://www.delftstack.com/howto/arduino/arduino-parse-string/
  https://github.com/mateusjunges/accel-stepper-with-distances
  https://hackaday.io/project/183279-accelstepper-the-missing-manual
  http://www.arduino.cc/en/Tutorial/SerialEvent
  https://www.arduino.cc/reference/en/language/functions/communication/serial/readstringuntil/in

*/


// Include the AccelStepper Library
#include <AccelStepper.h>
#include <elapsedMillis.h>


// Motor Connections (constant current, step/direction bipolar motor driver)

// Stepper Motor X
const int stepPinX = 2; //X.STEP
const int dirPinX = 5; // X.DIR

// Stepper Motor Y
const int stepPinY = 3;// x.step
const int dirPinY = 6;//x.dir

elapsedMillis printTime;

int sensorValueX = 0 ;
int sensorValueY = 0 ;
int bValue = 0 ;
int myDistVals[2];// delete
int xStepDist = 0;
int yStepDist = 0;


// include the library code:
#include <LiquidCrystal.h>
// Include the AccelStepper Library
#include <AccelStepper.h>
// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
AccelStepper myStepper(AccelStepper::DRIVER, stepPinX, dirPinX);           // works for a4988 (Bipolar, constant current, step/direction driver)


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);

 // set the maximum speed, acceleration factor, and the target position.
  myStepper.setMaxSpeed(800.0);   // the motor accelerates to this speed exactly without overshoot. Try other values.
  myStepper.setAcceleration(50.0);   // try other acceleration rates.
  myStepper.moveTo(10000); 
  // For negative rotation
  //myStepper.moveTo(-10000);    // This will demonstrate a move in the negative direction.

  //hand shake protocol
  establishContact();
}

int count = 0;    // tracks seconds to trigger an action if desired.

void loop() {
  float mSpeed;
  if (printTime >= 1000) {    // reports speed and position each second
    printTime = 0;
    mSpeed = myStepper.speed();
    Serial.print(mSpeed);
    Serial.print("  ");
    Serial.print(myStepper.currentPosition());
    Serial.print("  ");
    Serial.println(count);
    if (count++ == 4){      // uncomment an action to see the response in the serial monitor
      myStepper.moveTo(xStepDist);
//      myStepper.move(100);
//      myStepper.stop();
    }
  }
  myStepper.run();

  // print the string when a newline arrives:
  //  if (stringComplete) {
  //    Serial.println(inputString);
  //     delay(100);
  //  lcd.clear();
  //   lcd.print(inputString);
  //    // clear the string:
  //    inputString = "";
  //    stringComplete = false;
  //  }


  //  if (Serial.available() > 0) {
  //     delay(100);
  //     // clear the screen
  //    lcd.clear();
  //    while (Serial.available() > 0) {
  //    // read the incoming byte:
  //    int inByte = Serial.read();
  //    lcd.write(inByte);
  //    // read the sensor:
  ////    sensorValueX = analogRead(A0);
  ////    int xValue = map(sensorValueX, 0, 1023, 0, 255);
  ////    // print the results:
  ////    Serial.print(xValue);
  ////    Serial.print(",");
  ////    // read the sensor:
  ////    sensorValueY = analogRead(A1);
  ////    int yValue = map(sensorValueY, 0, 1023, 0, 255); // 0 to 255 range so that the digital pins can read them
  ////    // print the results:
  ////    Serial.print(yValue);
  ////    Serial.print(",");
  ////    // read the sensor:
  ////    bValue = digitalRead(2);
  ////    // print the results:
  ////    Serial.println(bValue);
  //    }
  //  }
}



void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("hello");   // send an initial string
    delay(300);
  }
}

void serialEvent() {


  while (Serial.available()) {
    // get the new byte:
int rcIntOne = Serial.parseInt();
int rcIntTwo = Serial.parseInt();
String x = String(rcIntOne);
String y = String(rcIntTwo);
xStepDist = rcIntOne;
yStepDist = rcIntTwo;


Serial.print("x" + x);
Serial.print("y" + y);
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

  }
}
