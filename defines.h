/*
 * @file defines.h
 */

#include <Arduino.h>


#ifndef DEFINES_H
#define DEFINES_H


// to show all debug messages
//#define DEBUG_MSG

#ifdef DEBUG_MSG
#define DEBUG_MSG_1
#define DEBUG_MSG_2
#endif

// to show level 1 debug messages
//#define DEBUG_MSG_1

// to show level 2 debug messages
//#define DEBUG_MSG_2

// to show level 3 debug messages
//#define DEBUG_MSG_3

// to show level 4 debug messages
//#define DEBUG_MSG_4

// to show level 5 debug messages
#define DEBUG_MSG_5


// Uncomment to print all DCC Packets
//#define  NOTIFY_DCC_MSG

// Uncomment to print all DCC turnout messages
//#define NOTIFY_TURNOUT_MSG

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
//#define FORCE_RESET_FACTORY_DEFAULT_CV

#ifdef ARDUINO_AVR_ATmega4809
#define LED_BUILTIN 39           // this is on ATmega4809 Curiosity Nano.
#endif

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty 
// For reference only not usable in this decoder as all pins in use for sensors.
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield
//#define DccAckPin ENABLE_DCC_ACK
//#ifdef ARDUINO_AVR_ATmega4809
//#define ENABLE_DCC_ACK 36   // PF2
//#endif

#define LEDCONTROL LED_BUILTIN

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

// Set the Decoder Version - Used by JMRI to Identify the decoder
#define DCC_DECODER_VERSION_NUM 11

// decoder CV wait time between checksensors in ms
#define CV_ACCESSORY_DECODER_WAIT_TIME 2

// define clockwise value
#define DIR_CW 1

// define anti clockwise
#define DIR_ACW !DIR_CW

// Set the number of tracks
#ifdef ARDUINO_AVR_NANO
#define NUM_TRACKS 16
#elif ARDUINO_ARCH_ESP32
#define NUM_TRACKS 16
#elif ARDUINO_AVR_ATmega4809
#define NUM_TRACKS 24
#endif

// Set the number of commands that are required.  This is number of tracks plus the other commands
//
// The other commands are:
//
// motor cw acw
// solenoid on off
// step cw acw
// turn 180 deg cw acw
// rotate cw acw
// stop at next
// spare

#define NUM_COMMANDS NUM_TRACKS + 12


// Define input hall first and last sensor input pin

#ifdef ARDUINO_AVR_NANO

#define HALL_1  1      // 
#define HALL_2  3      // 
#define HALL_3  4      // 
#define HALL_4  5      // 
#define HALL_5  6      // 
#define HALL_6  7      // 
#define HALL_7  8      // 
#define HALL_8  9      // 
#define HALL_9  13     // 
#define HALL_10 15     // 
#define HALL_11 16     // 
#define HALL_12 17     // 
#define HALL_13 18     // 
#define HALL_14 19     // 
#define HALL_15 20     // 
#define HALL_16 21     // 


#define LEARN_BUTTON 21 // A7

#define SOLENOID_PIN 12
#define MOTOR_PIN 13

#elif ARDUINO_AVR_ATmega4809

#define HALL_1  3      // D3
#define HALL_2  4      // D4
#define HALL_3  5      // D5
#define HALL_4  6      // D6
#define HALL_5  7      // D7
#define HALL_6  8      // D8
#define HALL_7  9      // D9
#define HALL_8  10     // D10
#define HALL_9  11     // D11
#define HALL_10 12     // D12
#define HALL_11 13     // D13
#define HALL_12 14     // D14
#define HALL_13 15     // D15
#define HALL_14 16     // D16
#define HALL_15 17     // D17
#define HALL_16 18     // D18
#define HALL_17 19     // D19
#define HALL_18 20     // D20
#define HALL_19 21     // D21
#define HALL_20 22     // D22
#define HALL_21 23     // D23
#define HALL_22 24     // D24
#define HALL_23 25     // D25
#define HALL_24 26     // D26


#define LEARN_BUTTON 39 //   PF5

#define SOLENOID_PIN 37 //   PF3
#define MOTOR_PIN 38    //   PF4


#endif




// command defines single commands
// these are still referenced as turnouts eg <T 25> = CMD_MOTOR_CW <C 25> = CMD_MOTOR_ACW

#define CMD_MOTOR_CW 241        // T 25
#define CMD_MOTOR_ACW 242       // C 25
#define CMD_SOLENOID_ON 251     // T 26
#define CMD_SOLENOID_OFF 252    // C 26

#define CMD_STEP_CW 261         // T 27
#define CMD_STEP_ACW 262        // C 27
#define CMD_180_CW 271          // T 28
#define CMD_180_ACW 272         // C 28
#define CMD_ROTATE_CW 281       // T 29
#define CMD_ROTATE_ACW 282      // C 29
#define CMD_STOP_AT_NEXT 291    // T 30
#define CMD_SPARE 292           // C 30


// command defines track commands

#define CMD_GOTO_1_CW 1
#define CMD_GOTO_1_ACW 2
#define CMD_GOTO_2_CW 11
#define CMD_GOTO_2_ACW 12
#define CMD_GOTO_3_CW 21
#define CMD_GOTO_3_ACW 22
#define CMD_GOTO_4_CW 31
#define CMD_GOTO_4_ACW 32
#define CMD_GOTO_5_CW 41
#define CMD_GOTO_5_ACW 42
#define CMD_GOTO_6_CW 51
#define CMD_GOTO_6_ACW 52
#define CMD_GOTO_7_CW 61
#define CMD_GOTO_7_ACW 62
#define CMD_GOTO_8_CW 71
#define CMD_GOTO_8_ACW 72
#define CMD_GOTO_9_CW 81
#define CMD_GOTO_9_ACW 82
#define CMD_GOTO_10_CW 91
#define CMD_GOTO_10_ACW 92
#define CMD_GOTO_11_CW 101
#define CMD_GOTO_11_ACW 102
#define CMD_GOTO_12_CW 111
#define CMD_GOTO_12_ACW 112
#define CMD_GOTO_13_CW 121
#define CMD_GOTO_13_ACW 122
#define CMD_GOTO_14_CW 131
#define CMD_GOTO_14_ACW 132
#define CMD_GOTO_15_CW 141
#define CMD_GOTO_15_ACW 142
#define CMD_GOTO_16_CW 151
#define CMD_GOTO_16_ACW 152

#define CMD_GOTO_17_CW 161
#define CMD_GOTO_17_ACW 162
#define CMD_GOTO_18_CW 171
#define CMD_GOTO_18_ACW 172
#define CMD_GOTO_19_CW 181
#define CMD_GOTO_19_ACW 182
#define CMD_GOTO_20_CW 191
#define CMD_GOTO_20_ACW 192
#define CMD_GOTO_21_CW 201
#define CMD_GOTO_21_ACW 202
#define CMD_GOTO_22_CW 211
#define CMD_GOTO_22_ACW 212
#define CMD_GOTO_23_CW 221
#define CMD_GOTO_23_ACW 222
#define CMD_GOTO_24_CW 231
#define CMD_GOTO_24_ACW 232


#ifdef ARDUINO_ARCH_ESP32
#define LED_BUILTIN 2
#endif


struct sensor_t
{
  uint8_t pin;
  uint8_t analog;
};


#endif
