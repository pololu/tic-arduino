#include <Tic.h>

/*** TicSerial ***/

void TicSerial::commandW32(TicCommand cmd, uint32_t val)
{
  sendCommandHeader(cmd);

  /* byte with MSbs:
  bit 0 = MSb of first (least significant) data byte
  bit 1 = MSb of second data byte
  bit 2 = MSb of third data byte
  bit 3 = MSb of fourth (most significant) data byte
  */
  serialW7(((val >>  7) & 1) |
           ((val >> 14) & 2) |
           ((val >> 21) & 4) |
           ((val >> 28) & 8));

  serialW7(val      ); // least significant byte with MSb cleared
  serialW7(val >>  8);
  serialW7(val >> 16);
  serialW7(val >> 24); // most significant byte with MSb cleared
}

void TicSerial::commandW7(TicCommand cmd, uint8_t val)
{
  sendCommandHeader(cmd);
  serialW7(val);
}

void TicSerial::commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2)
{
  sendCommandHeader(cmd);
  serialW7(val1);
  serialW7(val2);
}

uint8_t TicSerial::commandR8(TicCommand cmd)
{
  sendCommandHeader(cmd);
  while (_stream->available() < 1) {}
  return _stream->read();
}

void TicSerial::getSegment(TicCommand cmd, uint8_t offset,
  uint8_t length, void * buffer)
{
  length &= 0x7F;
  sendCommandHeader(cmd);
  serialW7(offset);
  serialW7(length);

  uint8_t * ptr = buffer;
  for (uint8_t i = 0; i < length; i++)
  {
    while (_stream->available() < 1) {}
    *ptr++ = _stream->read();
  }
}

void TicSerial::sendCommandHeader(TicCommand cmd)
{
  if (_deviceNumber == 255)
  {
    // compact protocol
    _stream->write((uint8_t)cmd);
  }
  else
  {
    // Pololu protocol
    _stream->write(0xAA);
    serialW7(_deviceNumber);
    serialW7((uint8_t)cmd);
  }
}


/*** TicI2C ***/

void TicI2C::commandQuick(TicCommand cmd)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.endTransmission();
}

void TicI2C::commandW32(TicCommand cmd, uint32_t val)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(val      ); // lowest byte
  Wire.write(val >>  8);
  Wire.write(val >> 16);
  Wire.write(val >> 24); // highest byte
  Wire.endTransmission();
}

void TicI2C::commandW7(TicCommand cmd, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(val & 0x7F);
  Wire.endTransmission();
}

void TicI2C::commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(val1 & 0x7F);
  Wire.write(val2 & 0x7F);
  Wire.endTransmission();
}

uint8_t TicI2C::commandR8(TicCommand cmd)
{
  uint8_t val;
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)1);
  val = Wire.read();
  Wire.endTransmission();
  return val;
}

void TicI2C::getSegment(TicCommand cmd, uint8_t offset,
  uint8_t length, void * buffer)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)cmd);
  Wire.write(offset);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)length);

  // TODO: check Wire.available() and handle errors

  uint8_t * ptr = buffer;
  for (uint8_t i = 0; i < length; i++)
  {
    *ptr = Wire.read();
    ptr++;
  }
}
