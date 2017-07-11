#pragma once

#include <Stream.h>
#include <Wire.h>

const uint8_t TicCurrentUnits = 32;

enum class TicCommand
{
  SetTargetPosition                 = 0xE0,
  SetTargetVelocity                 = 0xE3,
  SetCurrentPosition                = 0xEC,
  Stop                              = 0x89,
  EnableDriver                      = 0x85,
  DisableDriver                     = 0x86,
  SetSpeedMax                       = 0xE6,
  SetSpeedMin                       = 0xE5,
  SetAccelMax                       = 0xEA,
  SetDecelMax                       = 0xE9,
  SetStepMode                       = 0x94,
  SetCurrentLimit                   = 0x91,
  SetDecayMode                      = 0x92,
  GetVariable                       = 0xA1,
  GetSetting                        = 0xA8,
};

enum class TicPlanningMode
{
  Off            = 0,
  TargetPosition = 1,
  TargetVelocity = 2,
};

enum class TicDecayMode
{
  Mixed  = 0,
  Slow   = 1,
  Fast   = 2,
};

enum class TicStepMode
{
  Full    = 0,
  Half    = 1,

  Microstep1  = 0,
  Microstep2  = 1,
  Microstep4  = 2,
  Microstep8  = 3,
  Microstep16 = 4,
  Microstep32 = 5,
};

enum class TicPin
{
  SCL = 0,
  SDA = 1,
  TX  = 2,
  RX  = 3,
  RC  = 4,
};

enum class TicPinState
{
  HighImpedance = 0,
  InputPullUp   = 1,
  OutputLow     = 2,
  OutputHigh    = 3,
};

class TicBase
{
public:
  void setTargetPosition(int32_t position)
  {
    commandW32(TicCommand::SetTargetPosition, position);
  }

  void setTargetVelocity(int32_t velocity)
  {
    commandW32(TicCommand::SetTargetVelocity, velocity);
  }

  void setCurrentPosition(int32_t position)
  {
    commandW32(TicCommand::SetCurrentPosition, position);
  }

  void stop()
  {
    commandQuick(TicCommand::Stop);
  }

  void enableDriver()
  {
    commandQuick(TicCommand::EnableDriver);
  }

  void disableDriver()
  {
    commandQuick(TicCommand::DisableDriver);
  }

  void setSpeedMax(uint32_t speed)
  {
    commandW32(TicCommand::SetSpeedMax, speed);
  }

  void setSpeedMin(uint32_t speed)
  {
    commandW32(TicCommand::SetSpeedMin, speed);
  }

  void setAccelMax(uint32_t accel)
  {
    commandW32(TicCommand::SetAccelMax, accel);
  }

  void setDecelMax(uint32_t decel)
  {
    commandW32(TicCommand::SetDecelMax, decel);
  }

  void setStepMode(TicStepMode mode)
  {
    commandW7(TicCommand::SetStepMode, (uint8_t)mode);
  }

  void setCurrentLimit(uint16_t limit)
  {
    commandW7(TicCommand::SetCurrentLimit, limit / TicCurrentUnits);
  }

  void setDecayMode(TicDecayMode mode)
  {
    commandW7(TicCommand::SetDecayMode, (uint8_t)mode);
  }

  uint16_t getErrorStatus()
  {
    return getVar16(VarOffset::ErrorStatus);
  }

  uint8_t getSwitchStatus()
  {
    return getVar8(VarOffset::SwitchStatus);
  }

  TicPlanningMode getPlanningMode()
  {
    return (TicPlanningMode)getVar8(VarOffset::PlanningMode);
  }

  int32_t getTargetPosition()          { return (int32_t)getVar32(VarOffset::TargetPosition); }
  int32_t getTargetVelocity()          { return (int32_t)getVar32(VarOffset::TargetVelocity); }
  uint32_t getSpeedMin()               { return getVar32(VarOffset::SpeedMin); }
  uint32_t getSpeedMax()               { return getVar32(VarOffset::SpeedMax); }
  uint32_t getDecelMax()               { return getVar32(VarOffset::DecelMax); }
  uint32_t getAccelMax()               { return getVar32(VarOffset::AccelMax); }
  int32_t getCurrentPosition()         { return (int32_t)getVar32(VarOffset::CurrentPosition); }
  int32_t getCurrentVelocity()         { return (int32_t)getVar32(VarOffset::CurrentVelocity); }
  int32_t getActingTargetPosition()    { return (int32_t)getVar32(VarOffset::ActingTargetPosition); }
  uint32_t getTimeSinceLastStep()      { return getVar32(VarOffset::TimeSinceLastStep); }
  uint8_t getDeviceReset()             { return getVar8(VarOffset::DeviceReset); }
  uint16_t getVinVoltage()             { return getVar16(VarOffset::VinVoltage); }
  uint32_t getUpTime()                 { return getVar32(VarOffset::UpTime); }
  int32_t getEncoderPosition()         { return (int32_t)getVar32(VarOffset::EncoderPosition); }
  uint16_t getRCPulseWidth()           { return getVar16(VarOffset::RCPulseWidth); }
  uint16_t getAnalogReading(TicPin pin);
  uint8_t getDigitalReading(TicPin pin);
  uint8_t getPinState(TicPin pin);

  TicStepMode getStepMode()
  {
    return (TicStepMode)getVar8(VarOffset::StepMode);
  }

  uint16_t getCurrentLimit()
  {
    return getVar8(VarOffset::CurrentLimit) * TicCurrentUnits;
  }

  TicDecayMode getDecayMode()
  {
    return (TicDecayMode)getVar8(VarOffset::DecayMode);
  }

  virtual void getSetting(uint8_t offset, uint8_t length, uint8_t * buf) = 0;

private:
  enum VarOffset
  {
    OperationState        = 0x00, // uint8_t
    ErrorStatus           = 0x01, // uint16_t
    ErrorsOccurred        = 0x03, // uint32_t
    SwitchStatus          = 0x07, // uint8_t
    PlanningMode          = 0x08, // uint8_t
    TargetPosition        = 0x09, // int32_t
    TargetVelocity        = 0x0D, // int32_t
    SpeedMin              = 0x11, // uint32_t
    SpeedMax              = 0x15, // uint32_t
    DecelMax              = 0x19, // uint32_t
    AccelMax              = 0x1D, // uint32_t
    CurrentPosition       = 0x21, // int32_t
    CurrentVelocity       = 0x25, // int32_t
    ActingTargetPosition  = 0x29, // int32_t
    TimeSinceLastStep     = 0x2D, // uint32_t
    DeviceReset           = 0x31, // uint8_t
    VinVoltage            = 0x32, // uint16_t
    UpTime                = 0x34, // uint32_t
    EncoderPosition       = 0x38, // int32_t
    RCPulseWidth          = 0x3C, // uint16_t
    AnalogReadingSCL      = 0x3E, // uint16_t
    AnalogReadingSDA      = 0x40, // uint16_t
    AnalogReadingTX       = 0x42, // uint16_t
    AnalogReadingRX       = 0x44, // uint16_t
    DigitalReadings       = 0x46, // uint8_t
    PinStates             = 0x47, // uint8_t
    StepMode              = 0x48, // uint8_t
    CurrentLimit          = 0x49, // uint8_t
    DecayMode             = 0x4A, // uint8_t
  };

  virtual void commandQuick(TicCommand cmd) = 0;
  virtual void commandW32(TicCommand cmd, uint32_t val) = 0;
  virtual void commandW7(TicCommand cmd, uint8_t val) = 0;
  virtual void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2) = 0;
  virtual uint8_t commandR8(TicCommand cmd) = 0;

  virtual uint8_t getVar8(uint8_t offset) = 0;
  virtual uint16_t getVar16(uint8_t offset) = 0;
  virtual uint32_t getVar32(uint8_t offset) = 0;
};

class TicSerial : public TicBase
{
public:

  TicSerial(Stream & stream, uint8_t deviceNumber = 255) :
    _stream(&stream),
    _deviceNumber(deviceNumber)
  {
  }

  uint8_t getDeviceNumber() { return _deviceNumber; }

  void getSetting(uint8_t offset, uint8_t length, uint8_t * const buf);

private:
  Stream * const _stream;
  uint8_t const _deviceNumber;

  void commandQuick(TicCommand cmd) { sendCommandHeader(cmd); }
  void commandW32(TicCommand cmd, uint32_t val);
  void commandW7(TicCommand cmd, uint8_t val);
  void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2);
  uint8_t commandR8(TicCommand cmd);

  uint8_t getVar8(uint8_t offset);
  uint16_t getVar16(uint8_t offset);
  uint32_t getVar32(uint8_t offset);

  void sendCommandHeader(TicCommand cmd);
  void serialW7(uint8_t val) { _stream->write(val & 0x7F); }
};

class TicI2C : public TicBase
{
public:
  // TODO: support Wire1 on Arduino Due?
  TicI2C(uint8_t address = 14) : _address(address)
  {
  }

  uint8_t getAddress() { return _address; }

  void getSetting(uint8_t offset, uint8_t length, uint8_t * const buf);

private:
  uint8_t const _address;

  void commandQuick(TicCommand cmd);
  void commandW32(TicCommand cmd, uint32_t val);
  void commandW7(TicCommand cmd, uint8_t val);
  void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2);
  uint8_t commandR8(TicCommand cmd);

  uint8_t getVar8(uint8_t offset);
  uint16_t getVar16(uint8_t offset);
  uint32_t getVar32(uint8_t offset);
};
