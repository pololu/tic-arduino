// This example shows how to send I2C commands to the Tic
// Stepper Motor Controller to control the position of a Stepper
// Motor.
//
// The Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number must be set to its default value of 14.
//
// If you have sent a De-energize command to the Tic, for example
// by clicking "De-energize" in the Tic Control Center, you will
// need to undo that by clicking "Resume" or power-cycling the
// Tic.

#include <Tic.h>

TicI2C tic;

#include <AStar32U4.h>
AStar32U4LCD lcd;

void setup()
{
  lcd.clear();
  lcd.print(F("hi"));

  // Set up I2C.
  Wire.begin();

  // Give the Tic some time to start up.
  delay(20);

  // Set the Tic's current position to 0, so that when we command
  // it to move later, it will move a predictable amount.
trySetPos:
  lcd.gotoXY(0, 0); lcd.print(F("setpos  "));
  tic.haltAndSetPosition(0);
  if (tic.getLastError())
  {
    lcd.gotoXY(0, 0); lcd.print(F("E setpos"));
    lcd.gotoXY(0, 1); lcd.print(tic.getLastError());
    ledRed(1);
    delay(1000);
    goto trySetPos;
    while (1);
  }

  // Tells the Tic that it is OK to start driving the motor.  The
  // Tic's safe-start feature helps avoid unexpected, accidental
  // movement of the motor: if an error happens, the Tic will not
  // drive the motor again until it receives the Exit Safe Start
  // command.  The safe-start feature can be disbled in the Tic
  // Control Center.
  //lcd.gotoXY(0, 0); lcd.print(F("exit ss "));
  tic.exitSafeStart();
  if (tic.getLastError())
  {
    //lcd.gotoXY(0, 0); lcd.print(F("E ess   "));
    //lcd.gotoXY(0, 1); lcd.print(tic.getLastError());
    //ledRed(1);
    while (1);
  }
}

// Sends a Reset Command Timeout command to the Tic.  We must
// should call this at least once per second, or else a command
// timeout error will happen.  The Tic's default command timeout
// period is 1000 ms, but it can be changed or disabled in the
// Tic Control Center.
void resetCommandTimeout()
{
  tic.resetCommandTimeout();
}

// Delays for the specified number of milliseconds while
// resetting the Tic.
void delayWithResetCommandTimeout(uint32_t ms)
{
  uint32_t start = millis();
  do
  {
    lcd.gotoXY(0, 0); lcd.print(F("delay   "));
    resetCommandTimeout();
  }
  while ((uint32_t)(millis() - start) <= ms);
  ///lcd.gotoXY(0, 0); lcd.print(F("delayed "));
}

// Polls the Tic, waiting for it to reach the specified target
// position.  Note that if the Tic detects an error, the Tic will
// probably go into safe-start mode and never reach its target
// position, so this function will loop infinitely.  If that
// happens, you will need to reset your Arduino.
void waitForTargetPositionOrError(int32_t targetPosition)
{
  do
  {
    //lcd.gotoXY(0, 0); lcd.print(F("wait    "));
    resetCommandTimeout();
    //lcd.gotoXY(0, 0); lcd.print(F("waitp   ");
  } while (tic.getCurrentPosition() != targetPosition);
  //lcd.gotoXY(0, 0); lcd.print(F("waited  "));
}

int32_t position = 0;

void loop()
{
  if (position <= 0) { position = 100; }
  else { position = -100; }

  // Tell the Tic to move to position 100, and delay for 2500 ms
  // to give it time to get there.
trySetTar:
  lcd.gotoXY(0, 0); lcd.print(F("+100    "));
  tic.setTargetPosition(100);
  if (tic.getLastError())
  {
    lcd.gotoXY(0, 0); lcd.print(F("E settar"));
    lcd.gotoXY(0, 1); lcd.print(tic.getLastError());
    ledRed(1);
    delay(1000);
    goto trySetTar;
    while (1);
  }
  delayWithResetCommandTimeout(2500);

  // Tell the Tic to move to position -100, and wait until it
  // gets there.
  tic.setTargetPosition(-100);
  waitForTargetPositionOrError(-100);
}
