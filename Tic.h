#pragma once

#include <Stream.h>
#include <Wire.h>

enum class TicCommand
{
  EnableDriver                      = 0x80,
  DisableDriver                     = 0x83,
  SetTargetPosition                 = 0xE0,
  SetTargetVelocity                 = 0xE3,
  SetSpeedMin                       = 0xE5,
  SetSpeedMax                       = 0xE6,
  SetDecelMax                       = 0xE9,
  SetAccelMax                       = 0xEA,
  SetCurrentLimit                   = 0x91,
  SetDecayMode                      = 0x92,
  SetStepMode                       = 0x94,
  SetPinState                       = 0x97,
  ResetCommandTimeout               = 0x98,
  GetVariable                       = 0xA1,
  GetVariableAndClearErrorsOccurred = 0xA2,
  GetOperationState                 = 0xA4,
  GetMovingState                    = 0xA7,
  GetSetting                        = 0xA8,
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
    inline void enableDriver(void)                          { commandQuick(TicCommand::EnableDriver); }
    inline void disableDriver(void)                         { commandQuick(TicCommand::DisableDriver); }
    inline void setTargetPosition(int32_t position)         { commandW32(TicCommand::SetTargetPosition, position); }
    inline void setTargetVelocity(int32_t velocity)         { commandW32(TicCommand::SetTargetVelocity, velocity); }
    inline void setSpeedMin(uint32_t speed)                 { commandW32(TicCommand::SetSpeedMin, speed); }
    inline void setSpeedMax(uint32_t speed)                 { commandW32(TicCommand::SetSpeedMax, speed); }
    inline void setDecelMax(uint32_t decel)                 { commandW32(TicCommand::SetDecelMax, decel); }
    inline void setAccelMax(uint32_t accel)                 { commandW32(TicCommand::SetAccelMax, accel); }
    inline void setCurrentLimit(uint8_t limit)              { commandW7(TicCommand::SetCurrentLimit, limit); }
    void setCurrentLimitMilliamps(uint16_t limit_mA);
    inline void setDecayMode(TicDecayMode mode)             { commandW7(TicCommand::SetDecayMode, (uint8_t)mode); }
    inline void setStepMode(TicStepMode mode)               { commandW7(TicCommand::SetStepMode, (uint8_t)mode); }
    inline void setPinState(TicPin pin, TicPinState state)  { commandW2x7(TicCommand::SetPinState, (uint8_t)pin, (uint8_t)state); }
    inline void resetCommandTimeout(void)                   { commandQuick(TicCommand::ResetCommandTimeout); }
    
    inline uint8_t getOperationState(void)          { return commandR8(TicCommand::GetOperationState); }
    inline uint8_t getMovingState(void)             { return commandR8(TicCommand::GetMovingState); }
      
    inline uint16_t getErrorStatus(void)            { return getVar16(VarOffset::ErrorStatus); }
    uint32_t getErrorsOccurred(bool clear = false)  { return getVar32(VarOffset::ErrorsOccurred, clear); }
    inline uint8_t getSwitchStatus(void)            { return getVar8(VarOffset::SwitchStatus); }
    inline uint8_t getPlanningMode(void)            { return getVar8(VarOffset::PlanningMode); }
    inline int32_t getTargetPosition(void)          { return (int32_t)getVar32(VarOffset::TargetPosition); }
    inline int32_t getTargetVelocity(void)          { return (int32_t)getVar32(VarOffset::TargetVelocity); }
    inline uint32_t getSpeedMin(void)               { return getVar32(VarOffset::SpeedMin); }
    inline uint32_t getSpeedMax(void)               { return getVar32(VarOffset::SpeedMax); }
    inline uint32_t getDecelMax(void)               { return getVar32(VarOffset::DecelMax); }
    inline uint32_t getAccelMax(void)               { return getVar32(VarOffset::AccelMax); }
    inline int32_t getCurrentPosition(void)         { return (int32_t)getVar32(VarOffset::CurrentPosition); }
    inline int32_t getCurrentVelocity(void)         { return (int32_t)getVar32(VarOffset::CurrentVelocity); }
    inline int32_t getActingTargetPosition(void)    { return (int32_t)getVar32(VarOffset::ActingTargetPosition); }
    inline uint32_t getTimeSinceLastStep(void)      { return getVar32(VarOffset::TimeSinceLastStep); }
    inline uint8_t getDeviceReset(void)             { return getVar8(VarOffset::DeviceReset); }
    inline uint16_t getVinVoltage(void)             { return getVar16(VarOffset::VinVoltage); }
    inline uint32_t getUpTime(void)                 { return getVar32(VarOffset::UpTime); }
    inline int32_t getEncoderPosition(void)         { return (int32_t)getVar32(VarOffset::EncoderPosition); }
    inline uint16_t getRCPulseWidth(void)           { return getVar16(VarOffset::RCPulseWidth); }
    uint16_t getAnalogReading(TicPin pin);  
    uint8_t getDigitalReading(TicPin pin);  
    uint8_t getPinState(TicPin pin);  
    inline uint8_t getStepMode(void)                { return getVar8(VarOffset::StepMode); }
    inline uint8_t getDecayMode(void)               { return getVar8(VarOffset::DecayMode); }
    
    virtual void getSetting(uint8_t offset, uint8_t length, uint8_t * const buf) = 0;

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
      DecayMode             = 0x49, // uint8_t
    };

    virtual void commandQuick(TicCommand cmd) = 0;
    virtual void commandW32(TicCommand cmd, uint32_t val) = 0;
    virtual void commandW7(TicCommand cmd, uint8_t val) = 0;
    virtual void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2) = 0;
    virtual uint8_t commandR8(TicCommand cmd) = 0;
    
    virtual uint8_t getVar8(uint8_t offset) = 0;
    virtual uint16_t getVar16(uint8_t offset) = 0;
    virtual uint32_t getVar32(uint8_t offset, bool clear = false) = 0;
};

class TicSerial : public TicBase
{
  public:

    TicSerial(Stream & stream, uint8_t deviceNumber = 255) :
      _stream(&stream),
      _deviceNumber(deviceNumber)
    {}

    inline uint8_t getDeviceNumber(void) { return _deviceNumber; }
    
    void getSetting(uint8_t offset, uint8_t length, uint8_t * const buf);

  private:
    Stream * const _stream;
    uint8_t const _deviceNumber;

    inline void commandQuick(TicCommand cmd) { sendCommandHeader(cmd); }
    void commandW32(TicCommand cmd, uint32_t val);
    void commandW7(TicCommand cmd, uint8_t val);
    void commandW2x7(TicCommand cmd, uint8_t val1, uint8_t val2);
    uint8_t commandR8(TicCommand cmd);
    
    uint8_t getVar8(uint8_t offset);
    uint16_t getVar16(uint8_t offset);
    uint32_t getVar32(uint8_t offset, bool clear = false);

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
    uint32_t getVar32(uint8_t offset, bool clear = false);
};
