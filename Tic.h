#pragma once

#include <Stream.h>
#include <Wire.h>

class Tic
{
  
  public:    
    
  enum Command
  {
    CmdEnableDriver              = 0x80,
    CmdDisableDriver             = 0x83,
    CmdSetTargetPosition         = 0xE0,
    CmdSetTargetVelocity         = 0xE3,
    CmdSetSpeedMin               = 0xE5,
    CmdSetSpeedMax               = 0xE6,
    CmdSetDecelMax               = 0xE9,
    CmdSetAccelMax               = 0xEA,
    CmdSetCurrentLimit           = 0x91,
    CmdSetDecayMode              = 0x92,
    CmdSetMicrosteppingMode      = 0x94,
    CmdSetPin                    = 0x97,
    CmdResetCommandTimeout       = 0x98,
    CmdGetVariable               = 0xA1,
    CmdGetVariableAndClearEvents = 0xA2,
    CmdGetOperationState         = 0xA4,
    CmdGetMovingState            = 0xA7,
    CmdReadSetting               = 0xA8,
  };
  
  enum DecayMode
  {
    DecayMixed  = 0,
    DecaySlow   = 1,
    DecayFast   = 2,
  };
  
  enum MicrosteppingMode
  {
    StepFull    = 0,
    StepHalf    = 1,
    
    Microstep1  = 0,
    Microstep2  = 1,
    Microstep4  = 2,
    Microstep8  = 3,
    Microstep16 = 4,
    Microstep32 = 5,
  };

  enum Pin
  {
    PinSCL  = 0,
    PinSDA  = 1,
    PinTX   = 2, // TODO omit these because not valid?
    PinRX   = 3,
  };
  
  enum PinState
  {
    HighImpedance = 0,
    InputPullUp   = 1,
    OutputLow     = 2,
    OutputHigh    = 3,
  };
  
    void enableDriver(void);
    void disableDriver(void);
    void setTargetPosition(int32_t position);
    void setTargetVelocity(int32_t velocity);
    void setSpeedMin(uint32_t speed);
    void setSpeedMax(uint32_t speed);
    void setDecelMax(uint32_t decel);
    void setAccelMax(uint32_t accel);
    void setCurrentLimit(uint8_t limit);
    void setCurrentLimitMilliamps(uint16_t limit_mA);
    void setDecayMode(DecayMode mode);
    void setMicrosteppingMode(MicrosteppingMode mode);
    void setPin(Pin pin, PinState state);
    void resetCommandTimeout(void);
    
  private:
    virtual void beginCommand(Command cmd) = 0;
    virtual void endWrite(void) = 0;
    virtual void write8Bit(uint8_t val) = 0;
    virtual void write32Bit(uint32_t val) = 0;
};

class TicSerial : public Tic
{
  public:
    enum Protocol { ProtocolCompact, ProtocolPololu };
  
    // TODO streamline this to assume compact if no deviceNum given and require deviceNum for pololu?
    TicSerial(Stream & serial, Protocol protocol = ProtocolCompact, uint8_t deviceNum = 13) : 
      _serial(&serial),
      _protocol(protocol),
      _deviceNum(deviceNum & 0x7F)
    {}
    
  private:
    Stream * _serial;
    Protocol _protocol;
    uint8_t _deviceNum;
    
    void beginCommand(Command cmd);
    inline void endWrite(void) {}
    inline void write8Bit(uint8_t val) { _serial->write(val); }
    void write32Bit(uint32_t val);
};

class TicI2C : public Tic
{
  public:
    // TODO support Wire1 on Due?
    TicI2C(uint8_t addr = 0x13) :
      _addr(addr)
    {}
    
  private:
    uint8_t _addr;
    
    void beginCommand(Command cmd);
    inline void endWrite(void) { Wire.endTransmission(); }
    inline void write8Bit(uint8_t val) { Wire.write(val); }
    void write32Bit(uint32_t val);
};
