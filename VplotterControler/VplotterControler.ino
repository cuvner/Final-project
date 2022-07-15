/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.


  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
  https://www.arduino.cc/reference/en/language/functions/communication/serial/readstringuntil/in

*/

int sensorValueX = 0 ;
int sensorValueY = 0 ;
int bValue = 0 ;


// include the library code:
#include <LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600) ;
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);

  //hand shake protocol
  establishContact();
}

void loop() {


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
   delay(100);
  lcd.clear();
 
  while (Serial.available()) {
    // get the new byte:
    String inChar = Serial.readStringUntil(",");
    // add it to the inputString:
    inputString += inChar;
     lcd.print(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
