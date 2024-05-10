/*
 * This is an accessory decoder to control the Marklin 7186 turntable.
 * 
 * 
 * 
 */
#include <NmraDcc.h>

#include "version.h"

#include "defines.h"

#include "TtMover.h"

#include "variables.h"

#include "functions.h"



/**********************************************************************
 * setup
 */

void setup()
{
  Serial.begin(115200);

  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);

  setVersion();

  // Configure the DCC CV Programing ACK pin for an output
#ifdef ENABLE_DCC_ACK
  pinMode( DccAckPin, OUTPUT );
#endif

  initSensors();

  ttMover.init(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME) * 10);

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 

#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );

  Serial.print("Rosscoe Train Marklin 7186 Decoder: ");Serial.println(DCC_DECODER_VERSION_NUM,DEC);

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  Serial.println("Resetting CVs to Factory Defaults");
  notifyCVResetFactoryDefault(); 
#endif

  BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

//#ifdef DEBUG_MSG
  Serial.print("DCC Base Address: "); Serial.println(BaseTurnoutAddress, DEC);
//#endif

  Serial.print(F("Version: "));
  Serial.print(versionBuffer[0]);
  Serial.print(F("."));
  Serial.print(versionBuffer[1]);
  Serial.print(F("."));
  Serial.println(versionBuffer[2]);

  Serial.println("Init Done");
}

void loop()
{

  static int learningbuttonOldval = 0,learningbuttonVal = 0;
  
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  ttMover.process();
  
  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }


  learningbuttonVal = dr(LEARNINGBUTTON);

  if (learningbuttonOldval != learningbuttonVal) {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH) showAcknowledge(3);
   }
  learningbuttonOldval = learningbuttonVal;


    // see if there are serial commands
  readString="";              //empty for next input

  while (Serial.available())
   {
    char c = Serial.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }

  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   } 

}




  
