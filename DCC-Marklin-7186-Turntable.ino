/*
 * This is a stationary decoder to control the Marklin 7186 turntable.
 * 
 * 
 * 
 */
#include <NmraDcc.h>

#include "defines.h"

NmraDcc  Dcc ;
DCC_MSG  Packet ;

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};

CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_ACCESSORY_DECODER_WAIT_TIME, 20},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},
};

uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseTurnoutAddress;

// for address learning mode
int LEARNINGBUTTON = A7;    // pin A6
#define LEDCONTROL LED_BUILTIN
int learningMode = LOW;

byte thisCommand = 0;

// buffer to hold serial commands
String readString;



void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};

#include "functions.h"
#include "TtMover.h"

TtMover ttMover;


#ifdef ENABLE_DCC_ACK
//const int DccAckPin = 3 ;

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
void notifyCVAck(void)
{
  Serial.println("notifyCVAck") ;
  
  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  
  digitalWrite( ENABLE_DCC_ACK, LOW );
}
#endif


#ifdef NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  Serial.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
  }
  Serial.println();
}
#endif

/*
// This function is called whenever a normal DCC Turnout Packet is received and we're in Board Addressing Mode
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower )
{
  Serial.print("notifyDccAccTurnoutBoard: ") ;
  Serial.print(BoardAddr,DEC) ;
  Serial.print(',');
  Serial.print(OutputPair,DEC) ;
  Serial.print(',');
  Serial.print(Direction,DEC) ;
  Serial.print(',');
  Serial.println(OutputPower, HEX) ;
}
*/

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
#ifdef  NOTIFY_TURNOUT_MSG
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Closed" : "Thrown") ;
  Serial.print(" Output: ");
  Serial.print(OutputPower ? "On" : "Off") ;
#endif

// check to see if in learning mode and update address

  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
    Serial.println("");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F(" H = ")); Serial.println(H,DEC);
    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);

   }
  else {

    if(( Addr >= BaseTurnoutAddress ) && ( Addr < (BaseTurnoutAddress + NUM_COMMANDS )) && OutputPower )
     {

//      uint16_t pinIndex = ( (Addr - BaseTurnoutAddress) << 1 ) + Direction ;
//      pinPulser.addPin(outputs[pinIndex]);
      thisCommand = ( ( ( Addr - BaseTurnoutAddress) * 10 ) + 1 ) + Direction;

      ttMover.addCommand(thisCommand);

#ifdef  DEBUG_MSG
      Serial.print("ndato thisCommand: ");
      Serial.println(thisCommand,DEC);
#endif
     }


   }

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.println();
#endif

}

/*
// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
{
  Serial.print("notifyDccSigOutputState: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(',');
  Serial.println(State, HEX) ;
}
*/

/**********************************************************************
 * setup
 */

void setup()
{
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);

  // Configure the DCC CV Programing ACK pin for an output
  //  pinMode( DccAckPin, OUTPUT );

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

//  Serial.println("NMRA DCC Example 1");
  Serial.print("Rosscoe Train Marklin 7186 Decoder Version: ");Serial.println(DCC_DECODER_VERSION_NUM,DEC);

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  Serial.println("Resetting CVs to Factory Defaults");
  notifyCVResetFactoryDefault(); 
#endif

  BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

#ifdef DEBUG_MSG
  Serial.print("DCC Base Address: "); Serial.println(BaseTurnoutAddress, DEC);
#endif

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




  
