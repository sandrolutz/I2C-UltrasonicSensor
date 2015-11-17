/*
 * I2C-UltrasonicSensor Project
 * Author:        Sandro Lutz
 * Copyright(c) 2015. All rights reserved.
 */
#include <EEPROM.h>
#include <Wire.h>

#include "Ultrasonic.h"

#define FILTER_DISTANCE 50

Ultrasonic ultrasonic(2, 3);
int distance[5];
int distancePosition;
bool freeRunningMode;
int freeRunningDelay;

void setup() {
  byte address = EEPROM.read(1); // you should not use address 0 to store data.
  if (address == 255 || address == 0 || address == 80 || address == 164 ||address == 170) {
    address = 112; // default address #112
  }
  Wire.begin(address);          // join i2c bus with the given address
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  freeRunningMode = false;
  distancePosition = 1;
}

void loop() {
  if(freeRunningMode) {
    makeMeasurment();
    delay(freeRunningDelay);
  }
}

void makeMeasurment()
{
  ultrasonic.measure();
  distance[distancePosition] = ultrasonic.getDistance();
  bool error = false;
  for(int i=1; i<5; ++i) {
    if(i != distancePosition && distance[i] != 0 && abs(distance[distancePosition] - distance[i]) > FILTER_DISTANCE) {
      error = true;
    }
  }
  if(!error) {
    distance[0] = distance[distancePosition];
  }
  
  if(++distancePosition == 5) {
    distancePosition = 1;
  }
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

  if (pos == 1) {
    if(data[0] == 81 && !freeRunningMode) {
      // start measurment
      ultrasonic.measure();
    } else if(data[0] == 69) {
      // stop free running mode
      freeRunningMode = false;
    }
  } else if(pos == 2 && data[0] == 70 && data[1] > 19) {
    // start free running mode
    freeRunningMode = true;
    freeRunningDelay = data[1];
  } else if(pos == 3 && data[0] == 170 && data[1] == 165 && data[2] != 0 && data[2] != 80 && data[2] != 164 && data[2] != 170) {
    EEPROM.update(1, data[2]);  // Save new device ID
    Wire.begin(data[2]);        // reinitialize I2C
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  if(freeRunningMode) {
    Wire.write(distance[0]);
  } else {
    Wire.write(ultrasonic.getDistance());
  }
}
