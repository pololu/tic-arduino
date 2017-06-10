// This sketch is intended to be used by library developers to test this library.
// It should be run on an A-Star Prime.

#include <Tic.h>
#include <AStar32U4.h>

AStar32U4LCD lcd;

void setup(void)
{
  Wire.begin();
  lcd.clear();
  lcd.print(F("hi"));
}

uint8_t lastError = 0;
const uint8_t address = 14;

void loop(void)
{
  lastError = 0;

  if (!lastError)
  {
    Wire.beginTransmission(address);
    Wire.write(0xA1);
    Wire.write(0x34);
    Wire.write(4);
    lastError = Wire.endTransmission();
  }

  uint8_t values[4];

  if (!lastError)
  {
    uint8_t byteCount = Wire.requestFrom((uint8_t)address, (uint8_t)4);
    if (byteCount != 4) { lastError = 50; }

    values[0] = Wire.read();
    values[1] = Wire.read();
    values[2] = Wire.read();
    values[3] = Wire.read();
  }

  lcd.clear();
  if (lastError)
  {
    lcd.print(F("err "));
    lcd.print(lastError);
  }
  else
  {
    char buffer[9];
    snprintf_P(buffer, sizeof(buffer), PSTR("%02x%02x%02x%02x"),
      values[0], values[1], values[2], values[3]);
    lcd.print(buffer);
  }

  delay(100);
}
