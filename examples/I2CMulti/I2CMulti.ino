// This example shows how to send I2C commands to two Tic Stepper
// Motor Controllers on the same I2C bus.
//
// Each Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number of one Tic must be set to its default
// value of 14, and the serial device number of another Tic must
// be set to 15.
//
// The GND, SCL, and SDA pins of the Arduino must each be
// connected to the corresponding pins on each Tic.  You might
// consider connecting the ERR lines of both Tics so that if
// either one experiences an error, both of them will shut down
// until you reset the Arduino.
//
// See the comments and instructions in I2CSpeedControl.ino for
// more information.

#include <Tic.h>

TicI2C tic1(14);
TicI2C tic2(15);

void setup()
{
  Wire.begin();
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
