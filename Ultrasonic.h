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
 * Ultrasonic library
 * This library can be used with SR(F)04 an compatible ultrasonic devices.
 * 
 * Written by Sandro Lutz
 * ------------------------------------------------------------------------------
 */

#ifndef Ultrasonic_h
#define Ultrasonic_h

#define NO_SENSOR_TIMEOUT 10000        // in µs
#define INVALID_DATA_TIMEOUT 50000  // in µs

#include <Arduino.h>

class Ultrasonic
{
    public:
        Ultrasonic(int pin);        // used for One-Wire connection
        Ultrasonic(int triggerPin, int echoPin);
        int getDistance();
        bool measure();             // Note: This is a blocking method!
    private:
        bool _oneWireConnection;
        int _triggerPin;
        int _echoPin;
        int _distance;
        unsigned long _startTime;

        void _init(int triggerPin, int echoPin);
        unsigned long _getTimeDifference(unsigned long time1, unsigned long time2);
};
#endif
