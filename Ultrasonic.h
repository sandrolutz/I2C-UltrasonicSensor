/*
 * Ultrasonic library
 * This library can be used with SR(F)04 an compatible ultrasonic devices.
 * 
 * Author:        Sandro Lutz
 * Copyright(c) 2015. All rights reserved.
 */

#ifndef Ultrasonic_h
#define Ultrasonic_h

#define SENSOR_TIMEOUT 10000        // in µs
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
