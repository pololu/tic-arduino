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

uint8_t TicSerial::getVar8(uint8_t offset)
{
  sendCommandHeader(TicCommand::GetVariable);
  serialW7(offset);
  serialW7(1);
  while (_stream->available() < 1) {}
  return _stream->read();
}

uint16_t TicSerial::getVar16(uint8_t offset)
{
  uint16_t val;

  sendCommandHeader(TicCommand::GetVariable);
  serialW7(offset);
  serialW7(2);
  while (_stream->available() < 2) {}
  val  =           _stream->read();       // low byte
  val |= (uint16_t)_stream->read() << 8;  // high byte
  return val;
}

uint32_t TicSerial::getVar32(uint8_t offset)
{
  uint32_t val;

  sendCommandHeader(TicCommand::GetVariable);
  serialW7(offset);
  serialW7(4);
  while (_stream->available() < 4) {}
  val  =           _stream->read();       // lowest byte
  val |= (uint16_t)_stream->read() <<  8;
  val |= (uint32_t)_stream->read() << 16;
  val |= (uint32_t)_stream->read() << 24; // highest byte
  return val;
}

void TicSerial::getSetting(uint8_t offset, uint8_t length, uint8_t * buf)
{
  uint8_t count = 0;
  uint8_t * ptr = buf;

  length &= 0x7F;

  sendCommandHeader(TicCommand::GetSetting);
  serialW7(offset);
  serialW7(length);

  while (count < length)
  {
    while (_stream->available() < 1) {}
    *ptr = _stream->read();
    ptr++;
  }
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

uint8_t TicI2C::getVar8(uint8_t offset)
{
  uint8_t val;
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)TicCommand::GetVariable);
  Wire.write(offset);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)1);
  val = Wire.read();
  Wire.endTransmission();
  return val;
}

uint16_t TicI2C::getVar16(uint8_t offset)
{
  uint16_t val;
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)TicCommand::GetVariable);
  Wire.write(offset);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)2);
  val  =           Wire.read();       // low byte
  val |= (uint16_t)Wire.read() << 8;  // high byte
  Wire.endTransmission();
  return val;
}

uint32_t TicI2C::getVar32(uint8_t offset)
{
  uint32_t val;
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)TicCommand::GetVariable);
  Wire.write(offset);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)4);
  val  =           Wire.read();       // lowest byte
  val |= (uint16_t)Wire.read() <<  8;
  val |= (uint32_t)Wire.read() << 16;
  val |= (uint32_t)Wire.read() << 24; // highest byte
  Wire.endTransmission();
  return val;
}

void TicI2C::getSetting(uint8_t offset, uint8_t length, uint8_t * buf)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)TicCommand::GetSetting);
  Wire.write(offset);
  Wire.endTransmission(false); // no stop (repeated start)
  Wire.requestFrom(_address, (uint8_t)length);

  uint8_t count = 0;
  uint8_t * ptr = buf;
  while (count < length && Wire.available())
  {
    *ptr = Wire.read();
    ptr++;
  }
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
