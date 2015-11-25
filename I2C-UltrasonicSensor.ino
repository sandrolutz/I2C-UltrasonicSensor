/*
  The MIT License (MIT)
  
  Copyright (c) 2015 Sandro Lutz
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * ------------------------------------------------------------------------------
 * I2C-UltrasonicSensor Project
 * You can turn a cheap ultrasonic sensor connected to an Arduino nano into an
 * fully functioning I2C ultrasonic sensor.
 * 
 * I2C commands (every byte is separated with "|")
 * ------------
 * <address>|81                     start measurment
 * <address>|70|<delay>             start free running mode (starts another measurment 
 *                                  automatically with the given delay in ms
 * <address>|69                     stops free running mode
 * <address>|170|165|<new_address>  set a new address for this device.
 *                                  It will no longer be accessible under the old address!
 *                                  The addresses 0, 80, 164, 170 are invalid.
 * 
 * Written by Sandro Lutz
 * ------------------------------------------------------------------------------
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
  byte address = EEPROM.read(1);
  if (address == 255 || address == 0 || address == 80 || address == 164 ||address == 170) {
    address = 112;              // default address is #112
  }
  Wire.begin(address);          // join I2C bus with the given address
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
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

// This function is executed whenever data is sent by master
// It is registered as an event, see setup()
void receiveEvent(int qty) {
  byte data[3];
  int pos = 0;
  while(Wire.available() && pos < 3) {
    data[pos] = Wire.read();
    ++pos;
  }
  while(Wire.available()) {
    Wire.read();  // empty receive buffer
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

// This function is executed whenever data is requested by master
// It is registered as an event, see setup()
void requestEvent() {
  if(freeRunningMode) {
    Wire.write(distance[0]);
  } else {
    Wire.write(ultrasonic.getDistance());
  }
}
