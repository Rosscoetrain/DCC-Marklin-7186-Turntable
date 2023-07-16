/*
 * Ttmover.h
 * 
 * A class to control the Marklin 7186 Turntable
 * 
 */

#include <Arduino.h>

#define TT_MOVER_MAX_TRACKS    16

enum TT_State
{  
  TT_IDLE = 0,
  TT_MOVE,
  TT_MOVING,
  TT_STOP,
};

class TtMover
{
  private:
    TT_State state = TT_IDLE;
    uint8_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
    byte thisCommand, newCommand;
    byte target, track, lastTrack;
    byte direction;
    String CommandName;

    uint16_t startMs;
    uint16_t interval = 200;

    void InterpretCommand(byte newcommand);
    void CheckSensors();
    bool dr(int pin);

  public:
    void init(uint16_t interval);
    uint8_t addCommand(uint8_t command);
    TT_State process(void);

};


  
