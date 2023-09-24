/*
 * defines.h
 */


// to show all debug messages
#define DEBUG_MSG

// Uncomment to print all DCC Packets
//#define  NOTIFY_DCC_MSG

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
//#define FORCE_RESET_FACTORY_DEFAULT_CV

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty 
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield


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
#define NUM_TRACKS 16

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

#define NUM_COMMANDS NUM_TRACKS + 7


#define SOLENOID_PIN 12
#define MOTOR_PIN 13

// Define input hall first and last sensor input pin
#define HALL_1  3      // D3
#define HALL_2  4      // D4
#define HALL_3  5      // D5
#define HALL_4  6      // D6
#define HALL_5  7      // D7
#define HALL_6  8      // D8
#define HALL_7  9      // D9
#define HALL_8  10     // D10
#define HALL_9  11     // D11
#define HALL_10 14     // A0
#define HALL_11 15     // A1
#define HALL_12 16     // A2
#define HALL_13 17     // A3
#define HALL_14 18     // A4
#define HALL_15 19     // A5
#define HALL_16 20     // A6

#define LEARN_BUTTON 21 // A7


// command defines single commands

#define CMD_MOTOR_CW 1
#define CMD_MOTOR_ACW 2
#define CMD_SOLENOID_ON 11
#define CMD_SOLENOID_OFF 12 
#define CMD_STEP_CW 181
#define CMD_STEP_ACW 182
#define CMD_180_CW 191
#define CMD_180_ACW 192
#define CMD_ROTATE_CW 201
#define CMD_ROTATE_ACW 202
#define CMD_STOP_AT_NEXT 211
#define CMD_SPARE 212

// command defines track commands

#define CMD_GOTO_1_CW 21
#define CMD_GOTO_1_ACW 22
#define CMD_GOTO_2_CW 31
#define CMD_GOTO_2_ACW 32
#define CMD_GOTO_3_CW 41
#define CMD_GOTO_3_ACW 42
#define CMD_GOTO_4_CW 51
#define CMD_GOTO_4_ACW 52
#define CMD_GOTO_5_CW 61
#define CMD_GOTO_5_ACW 62
#define CMD_GOTO_6_CW 71
#define CMD_GOTO_6_ACW 72
#define CMD_GOTO_7_CW 81
#define CMD_GOTO_7_ACW 82
#define CMD_GOTO_8_CW 91
#define CMD_GOTO_8_ACW 92
#define CMD_GOTO_9_CW 101
#define CMD_GOTO_9_ACW 102
#define CMD_GOTO_10_CW 111
#define CMD_GOTO_10_ACW 112
#define CMD_GOTO_11_CW 121
#define CMD_GOTO_11_ACW 122
#define CMD_GOTO_12_CW 131
#define CMD_GOTO_12_ACW 132
#define CMD_GOTO_13_CW 141
#define CMD_GOTO_13_ACW 142
#define CMD_GOTO_14_CW 151
#define CMD_GOTO_14_ACW 152
#define CMD_GOTO_15_CW 161
#define CMD_GOTO_15_ACW 162
#define CMD_GOTO_16_CW 171
#define CMD_GOTO_16_ACW 172


  
