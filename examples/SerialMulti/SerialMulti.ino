// This example shows how to send serial commands to two Tic
// Stepper Motor Controllers on the same serial bus.
//
// Each Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number of one Tic must be set to its default
// value of 14, and the serial device number of another Tic must
// be set to 15.
//
// The GND pin of the Arduino must be be connected to a GND pin
// on each Tic.  The TX pin of the Arduino must be connected to
// the RX pins of each Tic.  This example does not read data, so
// there is no need to connect the Tics' TX pins.
//
// See the comments and instructions in SerialSpeedControl.ino for
// more information.

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

TicSerial tic1(ticSerial, 14);
TicSerial tic2(ticSerial, 15);

void setup()
{
  ticSerial.begin(9600);
  delay(20);

  tic1.exitSafeStart();
  tic2.exitSafeStart();
}

void resetCommandTimeout()
{
  tic1.resetCommandTimeout();
  tic2.resetCommandTimeout();
}

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
  tic1.setTargetVelocity(2000000);
  tic2.setTargetVelocity(0);
  delayWhileResettingCommandTimeout(1000);

  tic1.setTargetVelocity(0);
  tic2.setTargetVelocity(1000000);
  delayWhileResettingCommandTimeout(1000);

  tic1.setTargetVelocity(-1000000);
  tic2.setTargetVelocity(0);
  delayWhileResettingCommandTimeout(1000);

  tic1.setTargetVelocity(0);
  tic2.setTargetVelocity(-2000000);
  delayWhileResettingCommandTimeout(1000);
}
