let serial; // variable for the serial object
let latestData = "waiting for data"; // variable to hold the data


let xMargin, yMargin, pitchMotors, motorOne, motorTwo, pulley;

let cradleX, cradleY;

let valueX, valueY, xDiff, yDiff;

let L, R, xL, xR, yL, yR;

let buttonX, buttonY;

let locH, locV;

let spray;

let portSelector; // a select menu for the port list

let positionalArr;

let prevX, prevY;

function setup() {
  createCanvas(400, 400);
  setupSerial();
  xMargin = 50;
  yMargin = 50;
  pitch = 300;
  motorOne = xMargin;
  motorTwo = motorOne + pitch;
  pulley = 20;
  locH = 0;
  locV = 0;
  spray = 0;

  positionalArr = [2];
  prevX = 0;
  prevY = 0;

  cradleX = width / 2;
  cradleY = height / 2;

  buttonX = createButton("X-pos");
  buttonX.position(0, 0);
  buttonX.mousePressed(xpos);

  buttonY = createButton("Y-pos");
  buttonY.position(50, 0);
  buttonY.mousePressed(ypos);
}

function serverConnected() {
  console.log("Connected to Server");
}

// list the ports
function gotList(thelist) {
  // make a select menu and position it:
  portSelector = createSelect();
  portSelector.position(10, height - 50);
  console.log("List of Serial Ports:");

  for (let i = 0; i < thelist.length; i++) {
    console.log(i + " " + thelist[i]);
    portSelector.option(thelist[i]);
  }
  // set a handler for when a port is selected from the menu:
  portSelector.changed(mySelectEvent);
}

function gotOpen() {
  console.log("Serial Port is Open");
  // send a byte to prompt the microcontroller to send:
  let test = 1;
  serial.write(test);
}

function gotClose() {
  console.log("Serial Port is Closed");
  latestData = "Serial Port is Closed";
}

function gotError(theerror) {
  console.log(theerror);
}

function mySelectEvent() {
  let item = portSelector.value();
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}

// when data is received in the serial buffer
function gotData() {
  // read a string from the serial port
  // until you get carriage return and newline:
  let inString = serial.readStringUntil("\n");
  //check to see that there's actually a string there:
  if (inString.length > 0) {
    if (inString !== "hello") {
      // split the string on the commas
      let sensors = split(inString, ",");
      // if there are three elements
      if (sensors.length > 0) {
        //map JS movement to an incremental
        //value up to 10.
        locV = int(map(sensors[0], 255 / 2, 255, 0, 10));
        locH = int(map(sensors[1], 255 / 2, 255, 0, 10));
        spray = int(sensors[2]);
        print("spray" + spray);
      }
    }
    let test = 1;
    serial.write(test); // send a byte requesting more serial
    sendStepsArduino();
  }
}

function draw() {
  background(220);
  updateXY();
  drawMotors();
  drawCradle();
  leftSide();
}

function drawMotors() {
  ellipse(motorOne, yMargin, pulley, pulley);
  ellipse(motorTwo, yMargin, pulley, pulley);
}

function drawCradle() {
  push();
  fill(255, 0, 0);
  line(motorOne, yMargin, cradleX, cradleY);
  line(motorTwo, yMargin, cradleX, cradleY);
  rectMode(CENTER);

  rect(cradleX, cradleY, 30, 80);
  let rectCntr = createVector(cradleX, cradleY);
  fill(0);
  let xyText = xyCoords();
  text("x" + xyText.x + ", y" + xyText.y, cradleX, cradleY + 55);
  pop();
}

//change this function name
//calculates side lengths of the polygraph
function leftSide() {
  L = dist(motorOne, yMargin, cradleX, cradleY);
  push();
  translate((motorOne + cradleX) / 2, (yMargin + cradleY) / 2);
  rotate(atan2(cradleY - yMargin, cradleX - motorOne));
  text(nfc(L, 1), 0, -5);
  pop();

  R = dist(motorTwo, yMargin, cradleX, cradleY);
  push();
  translate((motorTwo + cradleX) / 2, (yMargin + cradleY) / 2);
  rotate(atan2(cradleY - yMargin, cradleX - motorTwo) + PI);
  text(nfc(R, 1), 0, -5);
  pop();
}

//Press button increments
function xpos() {
  cradleX += 5;
}

function ypos() {
  cradleY += 5;
}

// mouse drag cradle on screen
function mouseDragged() {
  if (
    mouseX > cradleX - 15 &&
    mouseX < cradleX + 15 &&
    mouseY > cradleY - 40 &&
    mouseY < cradleY + 40
  ) {
    cradleX = mouseX;
    cradleY = mouseY;
  }
}

// updates xy pos from joystick
function updateXY() {
  cradleX += locH;
  cradleY += locV;
}

//Returns xy of the cradle
function xyCoords() {
  //returns a vec coord

  let x = cradleX - motorOne;
  let y = cradleY - yMargin;
  let xy = createVector(x, y);
  if (x != prevX && y != prevY) {
    // sendStepsArduino();
    print(prevX, prevY);
  }

  prevX = x;
  prevY = y;

  return xy;
}

function setupSerial() {
  // serial constructor
  serial = new p5.SerialPort();
  // get a list of all connected serial devices
  serial.list();
  // serial port to use - you'll need to change this
  // serial.open("/dev/tty.usbmodem14201"); //turned off bc of drop down
  // callback for when the sketchs connects to the server
  serial.on("connected", serverConnected);
  // callback to print the list of serial devices
  serial.on("list", gotList);
  // what to do when we get serial data
  serial.on("data", gotData);
  // what to do when there's an error
  serial.on("error", gotError);
  // when to do when the serial port opens
  serial.on("open", gotOpen);
  // what to do when the port closes
  serial.on("close", gotClose);
}

//send message to arduino
function sendStepsArduino() {
  let message = stepperRot(L, R);
  console.log(message);
  serial.write(message);
}

//convert side lengths into stepper rotations and return string
function stepperRot(_left, _right) {
  //maps steps
  // let lRot = map(_left, 0, 275, 0, 2000);
  // let rRot = map(_right, 0, 275, 0, 2000);
  //Distance
  
  let sideStrg = [3];
  sideStrg[0] = str(round(_left));
  sideStrg[1] = str(round(_right));
  // sideStrg[2] = "3";
  let separator = " ";
  let message = join(sideStrg, separator)

  return message;
}
