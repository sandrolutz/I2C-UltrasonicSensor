/*
 * Ultrasonic library
 * This library can be used with SR(F)04 an compatible ultrasonic devices.
 * 
 * Author:        Sandro Lutz
 * Copyright(c) 2015. All rights reserved.
 */

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int pin)
{
	_init(pin, pin);
}

Ultrasonic::Ultrasonic(int triggerPin, int echoPin)
{
  _init(triggerPin, echoPin);
}

void Ultrasonic::_init(int triggerPin, int echoPin)
{
  if(triggerPin == echoPin) {
    _oneWireConnection = true;
  } else {
    _oneWireConnection = false;
  }
  _triggerPin = triggerPin;
  _echoPin = echoPin;
  _distance = -1;

  pinMode(_triggerPin, OUTPUT);
  digitalWrite(_triggerPin, HIGH);
  if(!_oneWireConnection) {
    pinMode(_echoPin, INPUT);
    digitalWrite(_echoPin, HIGH);   // enable pull-up resistor
  }
}

unsigned long Ultrasonic::_getTimeDifference(unsigned long time1, unsigned long time2)
{
  if(time1 > time2) { // an overflow occured
    return 4294967295 - time1 + time2;
  }
  return time2 - time1;
}

bool Ultrasonic::measure()
{
  digitalWrite(_triggerPin, LOW);
  delayMicroseconds(10);
  if (_oneWireConnection) {
    pinMode(_echoPin, INPUT);
    digitalWrite(_echoPin, HIGH);   // enable pull-up resistor
  } else {
    digitalWrite(_triggerPin, HIGH);
  }
  _startTime = micros();
  while(digitalRead(_echoPin) == LOW && _getTimeDifference(_startTime, micros()) < SENSOR_TIMEOUT);  // wait for the sensor to answer
  if(digitalRead(_echoPin) == LOW) {  // ultrasonic device did not answer within time.
    return false;
  }
  _startTime = micros();
  while(digitalRead(_echoPin) == HIGH && _getTimeDifference(_startTime, micros()) < INVALID_DATA_TIMEOUT); 
  if(digitalRead(_echoPin) == HIGH) {  // ultrasonic device did not receive an echo (no objects in range)
    _distance = -1;
    return true;
  }
  int duration = _getTimeDifference(_startTime, micros());
  _distance = duration/58;        // according to datasheet of SR(F)04
  return true;
}

int Ultrasonic::getDistance()
{
  return _distance;
}
