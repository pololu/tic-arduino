// This example shows how to control a stepper motor with three
// buttons.  This code is meant to run on an Arduino or
// Arudino-compatible board that has three momentary pushbuttons
// connected to it.  The Arduino reads button presses and
// controls the Tic Stepper Motor Controller via I2C.
//
// The Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number must be set to its default value of 14.
//
// If you have sent a De-energize command to the Tic, for example
// by clicking "De-energize" in the Tic Control Center, you will
// need to undo that by clicking "Resume" or power-cycling the
// Tic.
//
// The Arduino must have three normally open pushbuttons
// connected to pins 4, 5, and 6.  Each pushbutton should have
// one lead connected to the Arduino I/O pin and the other lead
// connected to GND.
//
// Please see https://github.com/pololu/tic-arduino for details
// on how to make the connections between the Arduino and the
// Tic.
//
// This example depends on the Pushbutton library:
//
//   https://github.com/pololu/pushbutton-arduino

#include <Tic.h>
#include <Pushbutton.h>

TicI2C tic;

Pushbutton buttonA(4);
Pushbutton buttonB(5);
Pushbutton buttonC(6);

// Define a variable that we can use to keep track of whether we
// are in speed control or position control mode.
enum class Mode {
  Off,
  Position,
  Velocity
};
auto mode = Mode::Off;

// This is a series of positions to step to, in units of
// microsteps.  Pressing button A makes the stepper motor cycle
// through these positions.
int32_t positionTable[] = {
  0,
  50,
  75,
  -100
};

// Keep track of which position in the position table is selected.
uint8_t positionIndex = 0;

#define POSITION_TABLE_SIZE (sizeof(positionTable)/sizeof(positionTable[0]))

void setup()
{
  // Set up I2C.
  Wire.begin();

  // Give the Tic some time to start up.
  delay(20);

  // Set the Tic's current position to 0, so that when we command
  // it to move later, it will move a predictable amount.
  tic.haltAndSetPosition(0);
}

void loop()
{
  if (buttonB.isPressed() && buttonC.isPressed())
  {
    // Both button B and button C are pressed.
    // Decelerate to velocity zero.
    tic.setTargetVelocity(0);
    tic.exitSafeStart();
    mode = Mode::Velocity;
  }
  else if (buttonB.isPressed())
  {
    // Button B is pressed and C is not pressed.
    // Move at +40 steps per second.
    tic.setTargetVelocity(400000);
    tic.exitSafeStart();
    mode = Mode::Velocity;
  }
  else if (buttonC.isPressed())
  {
    // Button C is pressed and B is not pressed.
    // Move at -40 steps per second.
    tic.setTargetVelocity(-400000);
    tic.exitSafeStart();
    mode = Mode::Velocity;
  }
  else
  {
    // Neither B nor C are pressed.  If we are in velocity mode,
    // decelerate to velocity zero.
    if (mode == Mode::Velocity)
    {
      tic.setTargetVelocity(0);
    }
  }

  // Use debouncing to detect distinct presses of button A.
  if (buttonA.getSingleDebouncedPress())
  {
    // Button A was pressed.
    // Advance to the next position in the table.
    positionIndex++;
    if (positionIndex >= POSITION_TABLE_SIZE)
    {
      positionIndex = 0;
    }
    tic.setTargetPosition(positionTable[positionIndex]);
    tic.exitSafeStart();
    mode = Mode::Position;
  }

  // Prevent the command timeout error from happening.
  tic.resetCommandTimeout();
}
