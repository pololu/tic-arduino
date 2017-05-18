#pragma once

#include <Stream.h>
#include <Wire.h>

enum class TicCommand
{
  EnableDriver              = 0x80,
  DisableDriver             = 0x83,
  SetTargetPosition         = 0xE0,
  SetTargetVelocity         = 0xE3,
  SetSpeedMin               = 0xE5,
  SetSpeedMax               = 0xE6,
  SetDecelMax               = 0xE9,
  SetAccelMax               = 0xEA,
  SetCurrentLimit           = 0x91,
  SetDecayMode              = 0x92,
  SetStepMode               = 0x94,
  SetPin                    = 0x97,
  ResetCommandTimeout       = 0x98,
  GetVariable               = 0xA1,
  GetVariableAndClearEvents = 0xA2,
  GetOperationState         = 0xA4,
  GetMovingState            = 0xA7,
  ReadSetting               = 0xA8,
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
  SCL  = 0,
  SDA  = 1,
  TX   = 2,
  RX   = 3,
  RC   = 4,
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
    inline void enableDriver(void)                    { commandQuick(TicCommand::EnableDriver); }
    inline void disableDriver(void)                   { commandQuick(TicCommand::DisableDriver); }
    inline void setTargetPosition(int32_t position)   { commandW32(TicCommand::SetTargetPosition, position); }
    inline void setTargetVelocity(int32_t velocity)   { commandW32(TicCommand::SetTargetVelocity, velocity); }
    inline void setSpeedMin(uint32_t speed)           { commandW32(TicCommand::SetSpeedMin, speed); }
    inline void setSpeedMax(uint32_t speed)           { commandW32(TicCommand::SetSpeedMax, speed); }
    inline void setDecelMax(uint32_t decel)           { commandW32(TicCommand::SetDecelMax, decel); }
    inline void setAccelMax(uint32_t accel)           { commandW32(TicCommand::SetAccelMax, accel); }
    inline void setCurrentLimit(uint8_t limit)        { commandW7(TicCommand::SetCurrentLimit, limit); }
    void setCurrentLimitMilliamps(uint16_t limit_mA);
    inline void setDecayMode(TicDecayMode mode)       { commandW7(TicCommand::SetDecayMode, (uint8_t)mode); }
    inline void setStepMode(TicStepMode mode)         { commandW7(TicCommand::SetStepMode, (uint8_t)mode); }
    inline void setPin(TicPin pin, TicPinState state) { commandW2x7(TicCommand::SetPin, (uint8_t)pin, (uint8_t)state); }
    inline void resetCommandTimeout(void)             { commandQuick(TicCommand::ResetCommandTimeout); }

    inline uint8_t getOperationState(void)        { return getVar8(VarOffset::OperationState); }
    inline uint16_t getErrorStatus(void)          { return getVar16(VarOffset::ErrorStatus); }
    inline uint32_t getErrorOccurred(void)        { return getVar32(VarOffset::ErrorOccurred); }
    inline uint8_t getSwitchStatus(void)          { return getVar8(VarOffset::SwitchStatus); }
    inline uint8_t getPlanningMode(void)          { return getVar8(VarOffset::PlanningMode); }
    inline int32_t getTargetPosition(void)        { return (int32_t)getVar32(VarOffset::TargetPosition); }
    inline int32_t getTargetVelocity(void)        { return (int32_t)getVar32(VarOffset::TargetVelocity); }
    inline uint32_t getSpeedMin(void)             { return getVar32(VarOffset::SpeedMin); }
    inline uint32_t getSpeedMax(void)             { return getVar32(VarOffset::SpeedMax); }
    inline uint32_t getDecelMax(void)             { return getVar32(VarOffset::DecelMax); }
    inline uint32_t getAccelMax(void)             { return getVar32(VarOffset::AccelMax); }
    inline int32_t getCurrentPosition(void)       { return (int32_t)getVar32(VarOffset::CurrentPosition); }
    inline int32_t getCurrentVelocity(void)       { return (int32_t)getVar32(VarOffset::CurrentVelocity); }
    inline int32_t getActingTargetPosition(void)  { return (int32_t)getVar32(VarOffset::ActingTargetPosition); }
    inline uint32_t getTimeSinceLastStep(void)    { return getVar32(VarOffset::TimeSinceLastStep); }
    inline uint8_t getDeviceReset(void)           { return getVar8(VarOffset::DeviceReset); }
    inline uint16_t getVinVoltage(void)           { return getVar16(VarOffset::VinVoltage); }
    inline uint32_t getUpTime(void)               { return getVar32(VarOffset::UpTime); }
    inline int32_t getEncoderPosition(void)       { return (int32_t)getVar32(VarOffset::EncoderPosition); }
    inline uint16_t getRCPulseWidth(void)         { return getVar16(VarOffset::RCPulseWidth); }
    uint16_t getAnalogReading(TicPin pin);
    uint8_t getDigitalReading(TicPin pin);
    uint8_t getPinState(TicPin pin);
    inline uint8_t getStepMode(void)              { return getVar8(VarOffset::StepMode); }
    inline uint8_t getDecayMode(void)             { return getVar8(VarOffset::DecayMode); }

  private:
    enum VarOffset
    {
      OperationState        =  0, // uint8_t
      ErrorStatus           =  1, // uint16_t
      ErrorOccurred         =  3, // uint32_t
      SwitchStatus          =  7, // uint8_t
      PlanningMode          =  8, // uint8_t
      TargetPosition        =  9, // int32_t
      TargetVelocity        = 13, // int32_t
      SpeedMin              = 17, // uint32_t
      SpeedMax              = 21, // uint32_t
      DecelMax              = 25, // uint32_t
      AccelMax              = 29, // uint32_t
      CurrentPosition       = 33, // int32_t
      CurrentVelocity       = 37, // int32_t
      ActingTargetPosition  = 41, // int32_t
      TimeSinceLastStep     = 45, // uint32_t
      DeviceReset           = 49, // uint8_t
      VinVoltage            = 50, // uint16_t
      UpTime                = 52, // uint32_t
      EncoderPosition       = 56, // int32_t
      RCPulseWidth          = 60, // uint16_t
      AnalogReadingSCL      = 62, // uint16_t
      AnalogReadingSDA      = 64, // uint16_t
      AnalogReadingTX       = 66, // uint16_t
      AnalogReadingRX       = 68, // uint16_t
      DigitalReadings       = 70, // uint8_t
      PinStates             = 71, // uint8_t
      StepMode              = 72, // uint8_t
      DecayMode             = 73, // uint8_t
    };

    virtual void commandQuick(TicCommand cmd) = 0;
    virtual void commandW32(TicCommand cmd, uint32_t val) = 0;
    virtual void commandW7(TicCommand cmd, uint8_t val) = 0;
    virtual void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2) = 0;
    
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
    {}

    inline uint8_t getDeviceNumber(void) { return _deviceNumber; }

  private:
    Stream * const _stream;
    uint8_t const _deviceNumber;

    inline void commandQuick(TicCommand cmd) { sendCommandHeader(cmd); }
    void commandW32(TicCommand cmd, uint32_t val);
    void commandW7(TicCommand cmd, uint8_t val);
    void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2);
    
    uint8_t getVar8(uint8_t offset);
    uint16_t getVar16(uint8_t offset);
    uint32_t getVar32(uint8_t offset);

    void sendCommandHeader(TicCommand cmd);
    inline void serialW7(uint8_t val) { _stream->write(val & 0x7F); }
};

class TicI2C : public TicBase
{
  public:
    // TODO support Wire1 on Due?
    TicI2C(uint8_t address = 0x13) :
      _address(address)
    {}

    inline uint8_t getAddress(void) { return _address; }

  private:
    uint8_t const _address;

    void commandQuick(TicCommand cmd);
    void commandW32(TicCommand cmd, uint32_t val);
    void commandW7(TicCommand cmd, uint8_t val);
    void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2);
    
    uint8_t getVar8(uint8_t offset);
    uint16_t getVar16(uint8_t offset);
    uint32_t getVar32(uint8_t offset);
};
