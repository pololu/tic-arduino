#include <Tic.h>

void Tic::enableDriver(void)
{
  beginCommand(CmdEnableDriver);
  endWrite();
}

void Tic::disableDriver(void)
{
  beginCommand(CmdDisableDriver);
  endWrite();
}

void Tic::setTargetPosition(int32_t position)
{
  beginCommand(CmdSetTargetPosition);
  write32Bit((uint32_t)position);
  endWrite();
}

void Tic::setTargetVelocity(int32_t velocity)
{
  if (velocity >  50000000) { velocity =  50000000; }
  if (velocity < -50000000) { velocity = -50000000; }
  
  beginCommand(CmdSetTargetVelocity);
  write32Bit((uint32_t)velocity);
  endWrite();
}

void Tic::setSpeedMin(uint32_t speed)
{
  if (speed > 50000000) { speed =  50000000; }
  
  beginCommand(CmdSetSpeedMin);
  write32Bit((uint32_t)speed);
  endWrite();
}

void Tic::setSpeedMax(uint32_t speed)
{
  if (speed > 50000000) { speed =  50000000; }
  
  beginCommand(CmdSetSpeedMax);
  write32Bit((uint32_t)speed);
  endWrite();
}

void Tic::setDecelMax(uint32_t decel)
{
  if (decel != 0 && decel < 100) { decel = 100; }
  if (decel > 0x7FFFFFFF) { decel = 0x7FFFFFFF; }
  
  beginCommand(CmdSetDecelMax);
  write32Bit((uint32_t)decel);
  endWrite();
}

void Tic::setAccelMax(uint32_t accel)
{
  if (accel < 100) { accel = 100; }
  if (accel > 0x7FFFFFFF) { accel = 0x7FFFFFFF; }
  
  beginCommand(CmdSetAccelMax);
  write32Bit((uint32_t)accel);
  endWrite();
}

void Tic::setCurrentLimit(uint8_t limit)
{
  if (limit > 124) { limit = 124; }
  
  beginCommand(CmdSetCurrentLimit);
  write8Bit(limit);
  endWrite();
}

void Tic::setCurrentLimitMilliamps(uint16_t limit_mA)
{
  // TODO
}

void Tic::setDecayMode(DecayMode mode)
{
  // TODO should this check bounds of enum? e.g. tic.setDecayMode(4);
  
  beginCommand(CmdSetDecayMode);
  write8Bit(mode);
  endWrite();
}

void Tic::setMicrosteppingMode(MicrosteppingMode mode)
{
  // TODO check enum?
  
  beginCommand(CmdSetDecayMode);
  write8Bit(mode);
  endWrite();
}

void Tic::setPin(Pin pin, PinState state)
{
  // TODO check enum?
  beginCommand(CmdSetDecayMode);
  write8Bit(pin);
  write8Bit(state);
  endWrite();
}

void Tic::resetCommandTimeout(void)
{
  beginCommand(CmdResetCommandTimeout);
  endWrite();
}

/*void Tic::GetVariable(void)
{
  
}*/

void TicSerial::beginCommand(Command cmd)
{
  if (_protocol == ProtocolCompact)
  {
    write8Bit(cmd);
  }
  else
  {
    write8Bit(0xAA);
    write8Bit(_deviceNum);
    write8Bit(cmd & 0x7F);
  }
}

void TicSerial::write32Bit(uint32_t val)
{
  /* byte with MSbs:
  bit 0 = MSb of first (least significant) data byte 
  bit 1 = MSb of second data byte
  bit 2 = MSb of third data byte
  bit 3 = MSb of fourth (most significant) data byte
  */
  write8Bit(((val >>  7) & 1) |
                 ((val >> 14) & 2) |
                 ((val >> 21) & 4) |
                 ((val >> 28) & 8));

  write8Bit( val        & 0x7F); // least significant byte with MSb cleared
  write8Bit((val >>  8) & 0x7F);
  write8Bit((val >> 16) & 0x7F);
  write8Bit((val >> 24) & 0x7F); // most significant byte with MSb cleared
}

void TicI2C::beginCommand(Command cmd)
{
  Wire.beginTransmission(_addr);
  Wire.write(cmd);
}

void TicI2C::write32Bit(uint32_t val)
{
  write8Bit( val        & 0xFF); // least significant byte
  write8Bit((val >>  8) & 0xFF);
  write8Bit((val >> 16) & 0xFF);
  write8Bit((val >> 24) & 0xFF); // most significant byte
}