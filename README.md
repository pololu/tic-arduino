# Tic Stepper Motor Controller library for Arduino

Version: 2.1.1<br>
Release date: 2021-06-22<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that helps interface with a
[Tic Stepper Motor Controller][tic] ([T500], [T834], [T825], [T249], [36v4])
using serial or I&sup2;C.

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.8.x or later;
we have not tested it with earlier versions.  This library should support any
Arduino-compatible board, including the [Pololu A-Star controllers][a-star].

## Getting started

### Hardware

The Tic Stepper Motor Controllers can be purchased from Pololu's website.
Before continuing, careful reading of the [Tic Stepper Controller User's
Guide][guide] is recommended.

### I2C connections

To control the Tic using I2C, you should connect your board's SCL pin to the
Tic's SCL pin, connect your board's SDA pin to the Tic's SDA pin, and connect
one of your board's GND pins to one of the Tic's GND pins.

### Serial connections

To control the Tic using asynchronous TTL serial, you need to at least connect
your board's TX pin (as defined in the table below) to the Tic's RX pin, and
connect your board's GND pin to one of the Tic's GND pins.  If you want to read
information from the Tic (as is done in this library's SerialPositionControl
example), you must also connect your board's RX pin (as defined in the table
below) to the Tic's TX pin.

The example sketches for this library use a hardware serial port on your Arduino
if one is available: if your Arduino environment defines
`SERIAL_PORT_HARDWARE_OPEN`, the examples will use that port.  The pins for this
serial port are different depending on which Arduino you are using.

| Microcontroller Board | Hardware serial? | MCU RX pin | MCU TX pin |
|-----------------------|------------------|------------|------------|
| A-Star 32U4           |        Yes       |      0     |      1     |
| A-Star 328PB          |        Yes       |     12     |     11     |
| Arduino Leonardo      |        Yes       |      0     |      1     |
| Arduino Micro         |        Yes       |      0     |      1     |
| Arduino Mega 2560     |        Yes       |     19     |     18     |
| Arduino Due           |        Yes       |     19**   |     18     |
| Arduino Uno           |        No        |     10     |     11     |
| Arduino Yun           |        No        |     10     |     11     |

** The Due's serial port is 3.3&nbsp;V, so it should not be directly connected
to the Tic's 5&nbsp;V TX line. A voltage divider or level shifter can be
used.

### Tic configuration

Before using the example sketches, you should use the Tic Control Center
software to apply these settings:

* Control mode: Serial/I&sup2;C/USB.
* Baud rate: 9600
* CRC disabled
* Device number: 14

### Software

You can use the Library Manager to install this library:

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
"Examples", and then selecting "Tic". If you cannot find these
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
[T500]: https://www.pololu.com/product/3135
[T834]: https://www.pololu.com/product/3133
[T825]: https://www.pololu.com/product/3131
[T249]: https://www.pololu.com/product/3139
[36v4]: https://www.pololu.com/product/3141

## Version history

* 2.1.1 (2021-06-22):
  - Fixed some compilation errors and warnings.
* 2.1.0 (2019-09-16):
  - Added support for the new [Tic 36v4][36v4].
* 2.0.0 (2019-02-06):
  - Removed the 2 ms delay inserted after I2C reads.  This makes the
    library incompatible with Tic firmware versions 1.00 and 1.01.
  - Added support for the new [Tic T249][T249].
  - Added features to support limit switches and homing, which were
    added in Tic firmware version 1.06.
* 1.2.1 (2018-03-19):
  - Fixed compilation errors in the SerialSpeedControl example.
* 1.2.0 (2018-03-16):
  - Added support for the new [Tic T500][T500].
  - Added the I2CSetCurrentLimit example.
* 1.1.0 (2017-11-02):
  - Added support for the new [Tic T834][T834].
  - Added the SerialMulti, I2CMulti, and I2CThreeButtons examples.
  - Added keywords.txt.
* 1.0.1 (2017-07-25): Removed the call to `haltAndSetPosition` in the `SerialSpeedControl` example because it is not necessary.
* 1.0.0 (2017-07-24): Original release.
