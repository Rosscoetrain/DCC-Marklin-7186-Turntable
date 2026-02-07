/*
 * @file variables.h
 */

#ifndef VARIABLES_H
#define VARIABLES_H




NmraDcc  Dcc ;
DCC_MSG  Packet ;


struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};

CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_ACCESSORY_DECODER_WAIT_TIME, 50},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},
};

uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseTurnoutAddress;

// for address learning mode
//#ifdef ARDUINO_AVR_ATmega4809
//int LEARNINGBUTTON = 39;    // pin PF5
//#else
//int LEARNINGBUTTON = A7;    // pin A6
//#endif

int learningMode = LOW;

uint16_t thisCommand = 0;

// buffer to hold serial commands
String readString;

TtMover ttMover;

char* version;
uint8_t versionBuffer[3];





#endif
