I2C-UltrasonicSensor
====================
You can turn a cheap ultrasonic sensor (**SR(F)04** or compatible) connected to an 
**Arduino** into a fully functioning I2C ultrasonic sensor. 

I used an Arduino nano. It should be working on any other Arduino board too.

> **Note:** You can easily change the pins the sensor is connected to by changing line 49 of the main file to <br/>
> ```Ultrasonic ultrasonic(\<trigger_pin\>, \<echo_pin\>)```

## Download
1. Download the current version of this project [here](https://github.com/sandrolutz/I2C-UltrasonicSensor/archive/master.zip).
2. Unzip the files into a folder named ```I2C-UltrasonicSensor``` to ensure correct functionality of the Arduino IDE.

## Usage
The communication with this I2C sensor is as follows:

| Packet (Bytes separated by slash)  | Description |
|------------------------------------|-------------|
| *\<address\>* / 81 | start measurment |
| *\<address\>* / 70 / *\<delay\>*            | Start free running mode (starts another measurment automatically after the given delay in ms |
| *\<address\>* / 69                    | Stops free running mode |
| *\<address\>* / 170 / 165 / *\<new_address\>* | Set a new address for this device. It will no longer be accessible with the old address! <br/>**Note:** The addresses 0, 80, 164, 170 are invalid. |

## License
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
