/*
 * @file functions.h
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H



/*
 * add the command to the command queue depending on address received.
 */

void setCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
 {
  if (learningMode == HIGH) {

    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println("setCommand");
    Serial3.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial3.print(F("Dir = ")); Serial.println(Direction,DEC);
    Serial3.print(F("Op = ")); Serial.println(OutputPower,DEC);
//    Serial3.print(F(" H = ")); Serial.println(H,DEC);
//    Serial3.print(F(" L = ")); Serial.println(L,DEC);
#else
    Serial.println("setCommand");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F("Dir = ")); Serial.println(Direction,DEC);
    Serial.print(F("Op = ")); Serial.println(OutputPower,DEC);
//    Serial.print(F(" H = ")); Serial.println(H,DEC);
//    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif

#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);

   }
  else {

    if(( Addr >= BaseTurnoutAddress ) && ( Addr < (BaseTurnoutAddress + NUM_COMMANDS )) && OutputPower )
     {

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
      Serial3.print(F("addr: "));Serial3.println(Addr);
      Serial3.print(F("baseTurnoutAddr: "));Serial3.println(BaseTurnoutAddress);
#else
      Serial.print(F("addr: "));Serial.println(Addr);
      Serial.print(F("baseTurnoutAddr: "));Serial.println(BaseTurnoutAddress);
#endif
#endif
      

// Direction = 1 move clockwise
// Direction = 0 move anti-clockwise

// this is where need to work out shortest route to next track
// then set Direction appropriately


      thisCommand = ( ( ( Addr - BaseTurnoutAddress) * 10 ) + 1 ) + Direction;

      ttMover.addCommand(thisCommand);

#ifdef  DEBUG_MSG_1
#ifdef ARDUINO_AVR_ATmega4809
      Serial3.println("");
      Serial3.print("ndato thisCommand: ");
      Serial3.println(thisCommand,DEC);
#else
      Serial.println("");
      Serial.print("ndato thisCommand: ");
      Serial.println(thisCommand,DEC);
#endif
#endif

     }

   }

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.println();
#endif

}




/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}


/*
 * read the analogue pin as if it was a digital pin and return 1 or 0
 */


bool dr (int pin)
 {
  int val = analogRead(pin);
  return ( val > 512 ) ;
 }


/*
 * this is a function to show via the onboard PCB led, the state of the decoder
 */

void showAcknowledge(int nb) {
  for (int i=0;i<nb;i++) {
#ifdef ARDUINO_AVR_ATmega4809
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on on curiosity nano LOW to turn on led
    delay(100);                       // wait for 1/10 second
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off by making the voltage HIGH
    delay(100);                       // wait for 1/10 second
#else
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for 1/10 second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for 1/10 second
#endif
  }
}



/*
 * soft reset function
 */

void(* resetFunc) (void) = 0;//declare reset function at address 0



/*
 * process serial commands
 */

#include "StringSplitter.h"

void doSerialCommand(String readString)
 {

  readString.trim();

#ifdef ARDUINO_AVR_ATmega4809
  Serial3.println(readString);  //so you can see the captured string
#else
  Serial.println(readString);  //so you can see the captured string
#endif

  if (readString == "<Z>")
   {

#ifdef ARDUINO_AVR_ATmega4809
        Serial3.println(F("Resetting"));
#else
        Serial.println(F("Resetting"));
#endif

    resetFunc();
   }

  if (readString == "<D>")
   {
#ifdef ARDUINO_AVR_ATmega4809
        Serial3.println(F("Reset factory default CVs"));
#else
        Serial.println(F("Reset factory default CVs"));
#endif

    notifyCVResetFactoryDefault();
   }

  if (readString == "<?>")
   {
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println(F("Help Text"));
    Serial3.println(F("Close a turnout: <C address>"));
    Serial3.println(F("Throw a turnout: <T address>"));
    Serial3.println(F("Set decoder base address: <A address>"));
    Serial3.println(F("Set decoder output pulse time: <P  mS / 10>"));
    Serial3.println(F("Reset decoder to Factory Defaults: <D>"));

    Serial3.println(F("Show current CVs: <>"));
    Serial3.println(F("Soft Reset: <Z>"));
#else
    Serial.println(F("Help Text"));
    Serial.println(F("Close a turnout: <C address>"));
    Serial.println(F("Throw a turnout: <T address>"));
    Serial.println(F("Set decoder base address: <A address>"));
    Serial.println(F("Set decoder output pulse time: <P  mS / 10>"));
    Serial.println(F("Reset decoder to Factory Defaults: <D>"));

    Serial.println(F("Show current CVs: <>"));
    Serial.println(F("Soft Reset: <Z>"));
#endif

   }
  else
   {
    if (readString.startsWith("<>"))
     {

#ifdef ARDUINO_AVR_ATmega4809
      Serial3.println(F("CVs are:"));
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));

      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_WAIT_TIME);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME));

      Serial3.print(F("CV"));
      Serial3.print(CV_29_CONFIG);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_29_CONFIG));
#else
      Serial.println(F("CVs are:"));
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));

      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_WAIT_TIME);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME));

      Serial.print(F("CV"));
      Serial.print(CV_29_CONFIG);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_29_CONFIG));
#endif

     }
    else
     {
      if (readString.startsWith("<"))
       {
        // this is where commands are completed

        // command to close turnout <C address>

        if (readString.startsWith("<C"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 1, 1 );

           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
                        Serial3.println(F("Invalid command: should be <C address>"));
#else
                        Serial.println(F("Invalid command: should be <C address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }


         // command to throw turnout <T address>

        if (readString.startsWith("<T"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 0, 1 );

           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <T address>"));
#else
            Serial.println(F("Invalid command: should be <T address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }

         // command to set address <A address>
         // address will be adjusted to the correct base turnout address
         // eg if address is 2 this will be corrected to 1 as the address are groups of 8 with an offset of 4
         // ie 1..8, 5..12, ...

        if (readString.startsWith("<A"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            byte L = (addr + 3) / 4;
            byte H = (addr + 3) / 1024;

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(addr);
            Serial3.print(F(" H = ")); Serial.println(H);
            Serial3.print(F(" L = ")); Serial.println(L);
#else
            Serial.print(F("Value = ")); Serial.println(addr);
            Serial.print(F(" H = ")); Serial.println(H);
            Serial.print(F(" L = ")); Serial.println(L);
#endif
#endif
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <A address>"));
#else
            Serial.println(F("Invalid command: should be <A address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }


        if (readString.startsWith("<P"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(addr);
#else
            Serial.print(F("Value = ")); Serial.println(addr);
#endif
#endif

            Dcc.setCV(CV_ACCESSORY_DECODER_WAIT_TIME, addr);

           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <P ms/10>"));
#else
            Serial.println(F("Invalid command: should be <P ms/10>"));
#endif
           }
          delete splitter;
          splitter = NULL;
          resetFunc();
         }

       }
      else
       {
#ifdef ARDUINO_AVR_ATmega4809
                Serial3.println(F("ERROR: Unknown command"));
#else
                Serial.println(F("ERROR: Unknown command"));
#endif
       }
     }
   }
 }



/*
 * these are the NMRA dcc override functions
 */

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};



#ifdef ENABLE_DCC_ACK

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
void notifyCVAck(void)
{
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println("notifyCVAck") ;
#else
    Serial.println("notifyCVAck") ;
#endif

  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  
  digitalWrite( ENABLE_DCC_ACK, LOW );
}
#endif


#ifdef NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
 {
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print("notifyDccMsg: ");
#else
  Serial.print("notifyDccMsg: ");
#endif

  for(uint8_t i = 0; i < Msg->Size; i++)
   {
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.print(Msg->Data[i], HEX);
    Serial3.write(' ');
#else
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
#endif
   }
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.println();
#else
  Serial.println();
#endif
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


/*
// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
 {
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print("notifyDccSigOutputState: ");
  Serial3.print(Addr,DEC);
  Serial3.print(',');
  Serial3.println(State, HEX);
#else
  Serial.print("notifyDccSigOutputState: ");
  Serial.print(Addr,DEC);
  Serial.print(',');
  Serial.println(State, HEX);
#endif

  setCommand(Addr + State - 1, 1, 1);
 }
*/


// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
 {
#ifdef  NOTIFY_TURNOUT_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial3.print(Addr,DEC) ;
  Serial3.print(" Direction: ");
  Serial3.print(Direction ? "Closed" : "Thrown") ;
  Serial3.print(" Output: ");
  Serial3.println(OutputPower ? "On" : "Off") ;
#else
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Closed" : "Thrown") ;
  Serial.print(" Output: ");
  Serial.println(OutputPower ? "On" : "Off") ;
#endif
#endif

  setCommand(Addr, Direction, OutputPower);

// check to see if in learning mode and update address
 }
  
#endif
