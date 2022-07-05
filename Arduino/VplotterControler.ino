int sensorValueX = 0 ;
int sensorValueY = 0 ;
int bValue = 0 ;

void setup(){
  Serial.begin(9600) ;
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  //hand shake protocol
establishContact();
}

void loop(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    int inByte = Serial.read();
    // read the sensor:
    sensorValueX = analogRead(A0);
    int xValue = map(sensorValueX, 0, 1023, 0, 255);
    // print the results:
    Serial.print(xValue);
    Serial.print(",");
    // read the sensor:
    sensorValueY = analogRead(A1);
    int yValue = map(sensorValueY, 0, 1023, 0, 255); // 0 to 255 range so that the digital pins can read them
    // print the results:
    Serial.print(yValue);
    Serial.print(",");
    // read the sensor:
    bValue = digitalRead(2);
    // print the results:
    Serial.println(bValue);

  }
}

  void establishContact(){
    while (Serial.available() <= 0) {
      Serial.println("hello");   // send an initial string
      delay(300);
    }
  }
