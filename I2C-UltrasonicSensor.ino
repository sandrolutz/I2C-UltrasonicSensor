/*
 * I2C-UltrasonicSensor Project
 * Author:        Sandro Lutz
 * Last modified: 2015-11-13 
 */
#include <EEPROM.h>
#include <Wire.h>

Ultrasonic ultrasonic(D2, D3);

void setup() {
  byte address EEPROM.read(1); // you should not use address 0 to store data.
  if (address == 255 || address == 0 || address == 80 || address == 164 ||address == 170) {
    address = 112; // default address #112
  }
  Wire.begin(address);          // join i2c bus with the given address
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  // nothing to be done here...
}

// function that executes whenever data is sent by master
// this function is registered as an event, see setup()
void receiveEvent(int qty) {
  byte data[3];
  int pos = 0;
  while(Wire.available() && pos < 3) {
    data[pos] = Wire.read();
    ++pos;
  }
  while(Wire.available()) {
    Wire.read(); // empty receive buffer
  }

  if (pos == 1 && data[0] == 81) {
    // start measurment
    ultrasonic.measure();
  } else if(pos == 3 && data[0] = 170 && data[1] == 165 && data[2] != 0 && data[2] != 80 && data[2] != 164 && data[2] != 170) {
    EEPROM.update(1, data[2]);  // Save new device ID
    Wire.begin(data[2]);        // reinitialize I2C
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(ultrasonic.getDistance());
  //Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}
