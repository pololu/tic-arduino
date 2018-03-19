// This example shows how to send serial commands to the Tic
// Stepper Motor Controller to control the speed of a Stepper
// Motor.
//
// The Tic's control mode must be set to "Serial/I2C/USB".  The
// baud rate should be set to 9600 and CRC should not be enabled.
// This example uses the Compact Protocol, so the Tic's device
// number is not used, and can be set to anything.
//
// If you have sent a De-energize command to the Tic, for example
// by clicking "De-energize" in the Tic Control Center, you will
// need to undo that by clicking "Resume" or power-cycling the
// Tic.
//
// Please see https://github.com/pololu/tic-arduino for details
// on how to make the connections between the Arduino and the
// Tic.

#include <Tic.h>

// On boards with a hardware serial port available for use, use
// that port to communicate with the Tic. For other boards,
// create a SoftwareSerial object using pin 10 to receive (RX)
// and pin 11 to transmit (TX).
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define ticSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial ticSerial(10, 11);
#endif

TicSerial tic(ticSerial);

void setup()
{
  // Set the baud rate.
  ticSerial.begin(9600);

  // Give the Tic some time to start up.
  delay(20);

  // Tells the Tic that it is OK to start driving the motor.  The
  // Tic's safe-start feature helps avoid unexpected, accidental
  // movement of the motor: if an error happens, the Tic will not
  // drive the motor again until it receives the Exit Safe Start
  // command.  The safe-start feature can be disbled in the Tic
  // Control Center.
  tic.exitSafeStart();
}

// Sends a "Reset command timeout" command to the Tic.  We must
// call this at least once per second, or else a command timeout
// error will happen.  The Tic's default command timeout period
// is 1000 ms, but it can be changed or disabled in the Tic
// Control Center.
void resetCommandTimeout()
{
  tic.resetCommandTimeout();
}

// Delays for the specified number of milliseconds while
// resetting the Tic's command timeout so that its movement does
// not get interrupted.
void delayWhileResettingCommandTimeout(uint32_t ms)
{
  uint32_t start = millis();
  do
  {
    resetCommandTimeout();
  } while ((uint32_t)(millis() - start) <= ms);
}

void loop()
{
  // Move forward at 200 steps per second for 2 seconds.
  tic.setTargetVelocity(2000000);
  delayWhileResettingCommandTimeout(2000);

  // Decelerate to a stop.
  tic.setTargetVelocity(0);
  delayWhileResettingCommandTimeout(1000);

  // Move in reverse at 100 steps per second for 1 second.
  tic.setTargetVelocity(-1000000);
  delayWhileResettingCommandTimeout(1000);

  // Decelerate to a stop.
  tic.setTargetVelocity(0);
  delayWhileResettingCommandTimeout(1000);
}
