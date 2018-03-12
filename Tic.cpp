#include <Tic.h>
#include <Arduino.h>

static const uint16_t Tic03aCurrentTable[33] =
{
  0,
  145,
  315,
  468,
  608,
  736,
  854,
  963,
  1065,
  1161,
  1252,
  1338,
  1420,
  1499,
  1575,
  1649,
  1722,
  1793,
  1863,
  1933,
  2002,
  2072,
  2143,
  2215,
  2290,
  2366,
  2446,
  2529,
  2617,
  2711,
  2812,
  2922,
  3042,
};

void TicBase::setCurrentLimit(uint16_t limit)
{
  uint8_t code = 0;

  if (product == TicProduct::T500)
  {
    for (uint8_t i = 0; i < 33; i++)
    {
      if (Tic03aCurrentTable[i] <= limit)
      {
        code = i;
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    code = limit / TicCurrentUnits;
  }

  commandW7(TicCommand::SetCurrentLimit, code);
}


/**** TicSerial ****/

void TicSerial::commandW32(TicCommand cmd, uint32_t val)
{
  sendCommandHeader(cmd);

  // byte with MSbs:
  // bit 0 = MSb of first (least significant) data byte
  // bit 1 = MSb of second data byte
  // bit 2 = MSb of third data byte
  // bit 3 = MSb of fourth (most significant) data byte
  serialW7(((val >>  7) & 1) |
           ((val >> 14) & 2) |
           ((val >> 21) & 4) |
           ((val >> 28) & 8));

  serialW7(val >> 0); // least significant byte with MSb cleared
  serialW7(val >> 8);
  serialW7(val >> 16);
  serialW7(val >> 24); // most significant byte with MSb cleared

  _lastError = 0;
}

void TicSerial::commandW7(TicCommand cmd, uint8_t val)
{
  sendCommandHeader(cmd);
  serialW7(val);

  _lastError = 0;
}

void TicSerial::getSegment(TicCommand cmd, uint8_t offset,
  uint8_t length, void * buffer)
{
  length &= 0x7F;
  sendCommandHeader(cmd);
  serialW7(offset);
  serialW7(length);

  uint8_t byteCount = _stream->readBytes((uint8_t *)buffer, length);
  if (byteCount != length)
  {
    _lastError = 50;

    // Set the buffer bytes to 0 so the program will not use an uninitialized
    // variable.
    memset(buffer, 0, length);
    return;
  }

  _lastError = 0;
}

void TicSerial::sendCommandHeader(TicCommand cmd)
{
  if (_deviceNumber == 255)
  {
    // Compact protocol
    _stream->write((uint8_t)cmd);
  }
  else
  {
    // Pololu protocol
    _stream->write(0xAA);
    serialW7(_deviceNumber);
    serialW7((uint8_t)cmd);
  }
  _lastError = 0;
}

/**** TicI2C ****/

void TicI2C::commandQuick(TicCommand cmd)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  _lastError = Wire.endTransmission();
}

void TicI2C::commandW32(TicCommand cmd, uint32_t val)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(val >> 0); // lowest byte
  Wire.write(val >> 8);
  Wire.write(val >> 16);
  Wire.write(val >> 24); // highest byte
  _lastError = Wire.endTransmission();
}

void TicI2C::commandW7(TicCommand cmd, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(val & 0x7F);
  _lastError = Wire.endTransmission();
}

void TicI2C::getSegment(TicCommand cmd, uint8_t offset,
  uint8_t length, void * buffer)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(offset);
  _lastError = Wire.endTransmission(false); // no stop (repeated start)
  if (_lastError)
  {
    // Set the buffer bytes to 0 so the program will not use an uninitialized
    // variable.
    memset(buffer, 0, length);
    return;
  }

  uint8_t byteCount = Wire.requestFrom(_address, (uint8_t)length);
  if (byteCount != length)
  {
    _lastError = 50;
    memset(buffer, 0, length);
    delayAfterRead();
    return;
  }

  _lastError = 0;
  uint8_t * ptr = (uint8_t *)buffer;
  for (uint8_t i = 0; i < length; i++)
  {
    *ptr = Wire.read();
    ptr++;
  }
  delayAfterRead();
}

// For reliable I2C operation, the Tic requires the bus to stay idle for 2 ms
// after any read is completed.
void TicI2C::delayAfterRead()
{
  delay(2);
}
