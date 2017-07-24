# Tic Stepper Motor Controller library for Arduino

Version: 1.0.0<br>
Release date: 2017-07-24<br>
[![Build Status](https://travis-ci.org/pololu/tic-arduino.svg?branch=master)](https://travis-ci.org/pololu/tic-arduino)<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that helps interface with a [Tic Stepper
Motor Controller][tic] using serial or I&sup2;C.

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.8.x or later;
we have not tested it with earlier versions.  This library should support any
Arduino-compatible board, including the [Pololu A-Star 32U4
controllers][a-star].

## Getting started

### Hardware

The Tic Stepper Motor Controllers can be purchased from Pololu's website.
Before continuing, careful reading of the [Tic Stepper Controller User's
Guide][guide] is recommended.

### I2C connections

To control the Tic using I2C, you should connect your board's SCL pin to the
Tic's SCL pin, connect your board's SDA pin to the Tic's SDA pin, and connect
your board's GND pin to one of the Tic's GND pins.

If you are not sure whether to use serial or I2C, we recommend using I2C because
the connections are simpler and there is no baud rate to worry about.

### Serial connections

To control the Tic asynchronous TTL serial, you need to at least connect your
board's TX pin (as defined in the table below) to the Tic's RX pin, and connect
your board's GND pin to one of the Tic's GND pins.  If you want to read
information from the Tic (as is done in this library's SerialPositionControl
example), you must also connect your board's RX pin (as defined in the table
below) to the Tic's TX pin.

The example sketches for this library use a hardware serial port on your Arduino
if one is available: if you Arduino environment defines
`SERIAL_PORT_HARDWARE_OPEN`, the examples will use that port.  The pins for this
serial port are different depending on which Arduino you are using.

| Microcontroller Board | Hardware serial? | MCU RX pin | MCU TX pin |
|-----------------------|------------------|------------|------------|
| A-Star 32U4           |        Yes       |      0     |      1     |
| Arduino Leonardo      |        Yes       |      0     |      1     |
| Arduino Micro         |        Yes       |      0     |      1     |
| Arduino Mega 2560     |        Yes       |     19     |     18     |
| Arduino Due           |        Yes       |     19**   |     18     |
| Arduino Uno           |        No        |     10     |     11     |
| Arduino Yun           |        No        |     10     |     11     |

** The Due's serial port is 3.3&nbsp;V, so it should not be directly connected
to the Maestro's 5&nbsp;V TX line. A voltage divider or level shifter can be
used.

### Tic configuration

Before using the example sketches, you should use the Tic Control Center
software to apply these settings:

* Control mode: Serial/I&sup2;C/USB.
* Baud rate: 9600
* CRC disabled
* Device number: 14

### Software

If you are using version 1.6.2 or later of the [Arduino software (IDE)][ide],
you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then
   "Manage Libraries...".
2. Search for "Tic".
3. Click the Tic entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/tic-arduino/releases)
   and decompress it.
2. Rename the folder "tic-arduino-xxxx" to "Tic".
3. Drag the "Tic" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples

Several example sketches are available that show how to use the library. You can
access them from the Arduino IDE by opening the "File" menu, selecting
"Examples", and then selecting "TicMaestro". If you cannot find these
examples, the library was probably installed incorrectly and you should retry
the installation instructions above.

## Classes

The main classes provided by the library are listed below:

* TicBase
* TicSerial
* TicI2C

## Documentation

For complete documentation of this library, see [the tic-arduino documentation][doc].  If you are already on that page, then click the links in the "Classes" section above.

[a-star]: https://www.pololu.com/a-star
[doc]: https://pololu.github.io/tic-arduino/
[guide]: https://www.pololu.com/docs/0J71
[ide]: https://www.arduino.cc/en/Main/Software
[tic]: https://www.pololu.com/tic

## Version history

* 1.0.0 (2017-07-24): Original release.