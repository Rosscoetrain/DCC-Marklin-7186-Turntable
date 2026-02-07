/*
 * @file Ttmover.h
 * 
 * A class to control the Marklin 7186 Turntable
 * 
 */

#ifndef TTMOVER_H
#define TTMOVER_H

#include <Arduino.h>

#define TT_MOVER_MAX_TRACKS  5

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
//    uint8_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
//    byte thisCommand, newCommand;
    uint16_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
    uint16_t thisCommand, newCommand;
    byte target, track, lastTrack;
    byte direction;
    String CommandName;

    uint16_t startMs;
    uint16_t interval = 200;                    // time between checking for change of solenoid state


    void InterpretCommand(uint16_t newcommand);
    void CheckSensors();

#ifdef ARDUINO_AVR_NANO
    bool dr(int pin);
#endif

    bool solenoidState = 0;



  public:
    void init(uint16_t interval);
    uint16_t addCommand(uint16_t command);
    TT_State process(void);

  private:
#ifdef ARDUINO_AVR_NANO

    sensor_t sensors[24] = {{3, 0}, {0 , 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {0, 0}, {0, 0}, {0, 0}, {11, 0}, {0, 0},           // nano digital pins
                          // 0       1       2       3       4       5       6       7       8        9       10      11      12       13                // array index
                          // 1       2       3       4       5       6       7       8       9        10      11      12      13       14                // sensor board position
                            {14, 0}, {15, 1}, {16, 1}, {17, 1}, {18, 1}, {19, 1}, {20, 1}, {0, 0}, {0, 0}, {0, 0}, };                   // nano analog pins
                          // 14       15       16       17       18       19       20       21      22      23
                          // 15       16       17       18       19       20       21       22      23      24
 
 #elif ARDUINO_ARCH_ESP32

/* TODO when circuit is completed for ESP32 module

    sensor_t sensors[24] = {{3, 0}, {0 , 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {0, 0}, {0, 0}, {0, 0}, {11, 0}, {0, 0},           // nano digital pins
                          // 0       1       2       3       4       5       6       7       8        9       10      11      12       13                // array index
                          // 1       2       3       4       5       6       7       8       9        10      11      12      13       14                // sensor board position
                           {14, 0}, {15, 1}, {16, 1}, {17, 1}, {18, 1}, {19, 1}, {20, 1}, {0, 0}, {0, 0}, {0, 0}, };                   // nano analog pins
                         // 14       15       16       17       18       19       20       21      22      23
                         // 15       16       17       18       19       20       21       22      23      24
*/

 #elif ARDUINO_AVR_ATmega4809
 
    sensor_t sensors[24] = {{3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0},
                          // 0       1       2       3       4       5        6        7        8        9        10       11                // array index
                          // 1       2       3       4       5       6        7        8        9        10       11       12                // sensor board position
                          // PA3     PA4     PA5     PA6     PA7     PB2      PB3      PB4      PB5      PC0      PC1      PC2
                            {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}};
                          // 12       13       14       15       16       17       18       19       20       21       22       23          // array index
                          // 13       14       15       16       17       18       19       20       21       22       23       24          // sensor board position
                          // PC3      PC4      PC5      PC6      PC7      PD0      PD1      PD2      PD3      PD4      PD5      PD6

 #endif
 

};


#endif
