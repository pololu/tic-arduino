#pragma once

#include <Stream.h>
#include <Wire.h>

uint8_t const TicCurrentUnits = 32;

enum class TicError
{
  IntentionallyDeenergized = 0,
  MotorDriverError         = 1,
  LowVin                   = 2,
  KillSwitch               = 3,
  RequiredInputInvalid     = 4,
  SerialError              = 5,
  CommandTimeout           = 6,
  SafeStartViolation       = 7,
  ErrLineHigh              = 8,
  SerialFraming            = 16,
  RxOverrun                = 17,
  Format                   = 18,
  Crc                      = 19,
  EncoderSkip              = 20,
};

enum class TicCommand
{
  SetTargetPosition                 = 0xE0,
  SetTargetVelocity                 = 0xE3,
  HaltAndSetPosition                = 0xEC,
  HaltAndHold                       = 0x89,
  ResetCommandTimeout               = 0x8C,
  Deenergize                        = 0x86,
  Energize                          = 0x85,
  ExitSafeStart                     = 0x83,
  EnterSafeStart                    = 0x8F,
  Reset                             = 0xB0,
  ClearDriverError                  = 0x8A,
  SetSpeedMax                       = 0xE6,
  SetStartingSpeed                  = 0xE5,
  SetAccelMax                       = 0xEA,
  SetDecelMax                       = 0xE9,
  SetStepMode                       = 0x94,
  SetCurrentLimit                   = 0x91,
  SetDecayMode                      = 0x92,
  GetVariable                       = 0xA1,
  GetVariableAndClearErrorsOccurred = 0xA2,
  GetSetting                        = 0xA8,
};

enum class TicOperationState
{
  Reset = 0,
  Deenergized = 2,
  SoftError = 4,
  WaitingForErrLine = 6,
  StartingUp = 8,
  Normal = 10,
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

enum class TicInputState
{
  NotReady = 0,
  Invalid = 1,
  Halt = 2,
  Position = 3,
  Velocity = 4,
};

enum class TicMiscFlags1
{
  Energized = 0,
  PositionUncertain = 1,
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

  void haltAndSetPosition(int32_t position)
  {
    commandW32(TicCommand::HaltAndSetPosition, position);
  }

  void haltAndHold()
  {
    commandQuick(TicCommand::HaltAndHold);
  }

  void resetCommandTimeout()
  {
    commandQuick(TicCommand::ResetCommandTimeout);
  }

  void deenergize()
  {
    commandQuick(TicCommand::Deenergize);
  }

  void energize()
  {
    commandQuick(TicCommand::Energize);
  }

  void exitSafeStart()
  {
    commandQuick(TicCommand::ExitSafeStart);
  }

  void enterSafeStart()
  {
    commandQuick(TicCommand::EnterSafeStart);
  }

  void reset()
  {
    commandQuick(TicCommand::Reset);
  }

  void clearDriverError()
  {
    commandQuick(TicCommand::ClearDriverError);
  }

  void setSpeedMax(uint32_t speed)
  {
    commandW32(TicCommand::SetSpeedMax, speed);
  }

  void setStartingSpeed(uint32_t speed)
  {
    commandW32(TicCommand::SetStartingSpeed, speed);
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

  TicOperationState getOperationState()
  {
    return (TicOperationState)getVar8(VarOffset::OperationState);
  }

  bool getEnergized()
  {
    return getVar8(VarOffset::MiscFlags1) >> (uint8_t)TicMiscFlags1::Energized & 1;
  }

  bool getPositionUncertain()
  {
    return getVar8(VarOffset::MiscFlags1) >> (uint8_t)TicMiscFlags1::PositionUncertain & 1;
  }

  uint16_t getErrorStatus()
  {
    return getVar16(VarOffset::ErrorStatus);
  }

  uint32_t getErrorsOccurred()
  {
    uint32_t result;
    getSegment(TicCommand::GetVariableAndClearErrorsOccurred,
      VarOffset::ErrorsOccurred, 4, &result);
    return result;
  }

  TicPlanningMode getPlanningMode()
  {
    return (TicPlanningMode)getVar8(VarOffset::PlanningMode);
  }

  int32_t getTargetPosition()
  {
    return getVar32(VarOffset::TargetPosition);
  }

  int32_t getTargetVelocity()
  {
    return getVar32(VarOffset::TargetVelocity);
  }

  uint32_t getStartingSpeed()
  {
    return getVar32(VarOffset::StartingSpeed);
  }

  uint32_t getSpeedMax()
  {
    return getVar32(VarOffset::SpeedMax);
  }

  uint32_t getDecelMax()
  {
    return getVar32(VarOffset::DecelMax);
  }

  uint32_t getAccelMax()
  {
    return getVar32(VarOffset::AccelMax);
  }

  int32_t getCurrentPosition()
  {
    return getVar32(VarOffset::CurrentPosition);
  }

  int32_t getCurrentVelocity()
  {
    return getVar32(VarOffset::CurrentVelocity);
  }

  uint8_t getDeviceReset()
  {
    return getVar8(VarOffset::DeviceReset);
  }

  uint16_t getVinVoltage()
  {
    return getVar16(VarOffset::VinVoltage);
  }

  uint32_t getUpTime()
  {
    return getVar32(VarOffset::UpTime);
  }

  int32_t getEncoderPosition()
  {
    return getVar32(VarOffset::EncoderPosition);
  }

  uint16_t getRCPulseWidth()
  {
    return getVar16(VarOffset::RCPulseWidth);
  }

  uint16_t getAnalogReading(TicPin pin)
  {
    uint8_t offset = VarOffset::AnalogReadingSCL + 2 * (uint8_t)pin;
    return getVar16(offset);
  }

  uint8_t getDigitalReading(TicPin pin)
  {
    uint8_t readings = getVar8(VarOffset::DigitalReadings);
    return (readings >> (uint8_t)pin) & 1;
  }

  TicPinState getPinState(TicPin pin)
  {
    uint8_t states = getVar8(VarOffset::PinStates);
    return (TicPinState)(states >> (2 * (uint8_t)pin) & 0b11);
  }

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

  TicInputState getInputState()
  {
    return (TicInputState)getVar8(VarOffset::InputState);
  }

  uint16_t getInputAfterAveraging()
  {
    return getVar16(VarOffset::InputAfterAveraging);
  }

  uint16_t getInputAfterHysteresis()
  {
    return getVar16(VarOffset::InputAfterHysteresis);
  }

  int32_t getInputAfterScaling()
  {
    return getVar32(VarOffset::InputAfterScaling);
  }

  void getSetting(uint8_t offset, uint8_t length, uint8_t * buffer)
  {
    getSegment(TicCommand::GetSetting, offset, length, buffer);
  }

private:
  enum VarOffset
  {
    OperationState        = 0x00, // uint8_t
    MiscFlags1            = 0x01, // uint8_t
    ErrorStatus           = 0x02, // uint16_t
    ErrorsOccurred        = 0x04, // uint32_t
    PlanningMode          = 0x09, // uint8_t
    TargetPosition        = 0x0A, // int32_t
    TargetVelocity        = 0x0E, // int32_t
    StartingSpeed         = 0x12, // uint32_t
    SpeedMax              = 0x16, // uint32_t
    DecelMax              = 0x1A, // uint32_t
    AccelMax              = 0x1E, // uint32_t
    CurrentPosition       = 0x22, // int32_t
    CurrentVelocity       = 0x26, // int32_t
    ActingTargetPosition  = 0x2A, // int32_t
    TimeSinceLastStep     = 0x2E, // uint32_t
    DeviceReset           = 0x32, // uint8_t
    VinVoltage            = 0x33, // uint16_t
    UpTime                = 0x35, // uint32_t
    EncoderPosition       = 0x39, // int32_t
    RCPulseWidth          = 0x3D, // uint16_t
    AnalogReadingSCL      = 0x3F, // uint16_t
    AnalogReadingSDA      = 0x41, // uint16_t
    AnalogReadingTX       = 0x43, // uint16_t
    AnalogReadingRX       = 0x45, // uint16_t
    DigitalReadings       = 0x47, // uint8_t
    PinStates             = 0x48, // uint8_t
    StepMode              = 0x49, // uint8_t
    CurrentLimit          = 0x4A, // uint8_t
    DecayMode             = 0x4B, // uint8_t
    InputState            = 0x4C, // uint8_t
    InputAfterAveraging   = 0x4D, // uint16_t
    InputAfterHysteresis  = 0x4F, // uint16_t
    InputAfterScaling     = 0x51, // uint16_t
  };

  uint8_t getVar8(uint8_t offset)
  {
    uint8_t result;
    getSegment(TicCommand::GetVariable, offset, 1, &result);
    return result;
  }

  uint16_t getVar16(uint8_t offset)
  {
    uint8_t buffer[2];
    getSegment(TicCommand::GetVariable, offset, 2, &buffer);
    return ((uint16_t)buffer[0] << 0) | ((uint16_t)buffer[1] << 8);
  }

  uint32_t getVar32(uint8_t offset)
  {
    uint8_t buffer[4];
    getSegment(TicCommand::GetVariable, offset, 4, buffer);
    return ((uint32_t)buffer[0] << 0) |
      ((uint32_t)buffer[1] << 8) |
      ((uint32_t)buffer[2] << 16) |
      ((uint32_t)buffer[3] << 24);
  }

  virtual void commandQuick(TicCommand cmd) = 0;
  virtual void commandW32(TicCommand cmd, uint32_t val) = 0;
  virtual void commandW7(TicCommand cmd, uint8_t val) = 0;
  virtual uint8_t commandR8(TicCommand cmd) = 0;
  virtual void getSegment(TicCommand cmd, uint8_t offset,
    uint8_t length, void * buffer);
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

  void getSetting(uint8_t offset, uint8_t length, uint8_t * buf);

private:
  Stream * const _stream;
  uint8_t const _deviceNumber;

  void commandQuick(TicCommand cmd) { sendCommandHeader(cmd); }
  void commandW32(TicCommand cmd, uint32_t val);
  void commandW7(TicCommand cmd, uint8_t val);
  uint8_t commandR8(TicCommand cmd);
  void getSegment(TicCommand cmd, uint8_t offset,
    uint8_t length, void * buffer);

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

private:
  uint8_t const _address;

  void commandQuick(TicCommand cmd);
  void commandW32(TicCommand cmd, uint32_t val);
  void commandW7(TicCommand cmd, uint8_t val);
  uint8_t commandR8(TicCommand cmd);
  void getSegment(TicCommand cmd, uint8_t offset,
    uint8_t length, void * buffer);
};
