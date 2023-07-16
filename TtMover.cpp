/*
 * TtMover.cpp
 * 
 * A class to control the Marklin 7186 turntable
 * 
 */

#include "TtMover.h"
#include "defines.h"

#define TT_MOVER_SLOT_EMPTY 255

//void TtMover::init(uint16_t onMs, uint16_t cduRechargeMs, uint8_t activeOutputState)
void TtMover::init(uint16_t interval)
{
//  this->onMs = onMs;
//  this->cduRechargeMs = cduRechargeMs;
//  this->activeOutputState = activeOutputState;

  this->interval = interval;
  this->state = TT_IDLE;
//  targetMs = 0;
  memset(this->commandQueue, TT_MOVER_SLOT_EMPTY, TT_MOVER_MAX_TRACKS + 1);
}

uint8_t TtMover::addCommand(uint8_t command)
{
//  Serial.print(" PinPulser::addPin: "); Serial.print(Pin,DEC);
  
  for(uint8_t i = 0; i < TT_MOVER_MAX_TRACKS; i++)
  {
    if(this->commandQueue[i] == command)
    {
//      Serial.print(" Already in Index: "); Serial.println(i,DEC);
      return i;
    }

    else if(commandQueue[i] == TT_MOVER_SLOT_EMPTY)
    {
//      Serial.print(" pinQueue Index: "); Serial.println(i,DEC);
      this->commandQueue[i] = command;
      this->process();
      return i;
    }
  }  

//  Serial.println();
  return TT_MOVER_SLOT_EMPTY;
}

TT_State TtMover::process(void)
{
  unsigned long now;
  
  switch(this->state)
  {

/*
 * 
 */

    case TT_IDLE:                             // check to see if there is a command then set state to move
    if(this->commandQueue[0] != TT_MOVER_SLOT_EMPTY)
    {
      this->thisCommand = this->commandQueue[0];

      this->state = TT_MOVE;
    }
    
    break;

/*
 * 
 */

    case TT_MOVE:                             // start the move

#ifdef DEBUG_MSG

    Serial.print("TT_MOVE thisCommand = ");Serial.println(this->thisCommand);
    Serial.print("TT_MOVE command = ");
    this->InterpretCommand(this->thisCommand);
    Serial.println(this->CommandName);

#endif
        // just the basic solenoid and motor commands
      if (this->thisCommand == CMD_MOTOR_CW)
      {
        digitalWrite(MOTOR_PIN, LOW);        //motor cw - default
      }
      if (this->thisCommand == CMD_MOTOR_ACW)
      {
        digitalWrite(MOTOR_PIN, HIGH);      //motor acw
      }
      if (this->thisCommand == CMD_SOLENOID_ON)
      {
        digitalWrite(SOLENOID_PIN, HIGH); //solenoid on
      }
      if (this->thisCommand == CMD_SOLENOID_OFF)
      {
        digitalWrite(SOLENOID_PIN, LOW); //solenoid off - default
      }

  // step commands
      if (this->thisCommand == CMD_STEP_CW)          // step cw
      {
        digitalWrite(MOTOR_PIN,LOW);
        digitalWrite(SOLENOID_PIN, HIGH);
        delay(2000);
        digitalWrite(SOLENOID_PIN, LOW);
        delay(5000);
        CheckSensors();
      }

      if (this->thisCommand == CMD_STEP_ACW)         // step acw
      {
        digitalWrite(MOTOR_PIN, HIGH); //motor acw
        digitalWrite(SOLENOID_PIN, HIGH); //solenoid on
        delay(2000);
        digitalWrite(SOLENOID_PIN, LOW); //solenoid off
        delay(5000);
        CheckSensors();
      }


  // rotate commands
      if (this->thisCommand == CMD_ROTATE_CW)          // step cw
      {
        digitalWrite(MOTOR_PIN, LOW);
        digitalWrite(SOLENOID_PIN, HIGH);

        memmove(this->commandQueue, this->commandQueue + 1, TT_MOVER_MAX_TRACKS);
        this->state = TT_MOVING;

      }

      if (this->thisCommand == CMD_ROTATE_ACW)          // step cw
      {

        digitalWrite(MOTOR_PIN, HIGH);
        digitalWrite(SOLENOID_PIN, HIGH);

        memmove(this->commandQueue, this->commandQueue + 1, TT_MOVER_MAX_TRACKS);
        this->state = TT_MOVING;

      }

  // stop at next command
      if (this->thisCommand == CMD_STOP_AT_NEXT)        // stop at next
      {
        digitalWrite(SOLENOID_PIN, LOW);
        this->state = TT_STOP;
      }


  //GoTo commands
      if (( CMD_GOTO_1_CW <= this->thisCommand ) && ( this->thisCommand <= CMD_GOTO_16_ACW ))
        {

#ifdef DEBUG_MSG
          Serial.print("thisCommand = ");Serial.println(this->thisCommand);
#endif

          this->direction = this->thisCommand - (int ( this->thisCommand / 10 ) * 10);

#ifdef DEBUG_MSG
          Serial.print("direction = ");Serial.println(this->direction);
#endif

          if (this->direction == 1)
          {
            this->target = this->thisCommand - 1;
            digitalWrite(MOTOR_PIN, LOW);    //motor cw
          }
          if (this->direction == 2)
          {
            this->target = this->thisCommand - 2;
            digitalWrite(MOTOR_PIN, HIGH);   //motor acw
          }

#ifdef DEBUG_MSG
          Serial.print("target = ");Serial.println(this->target);
#endif

      // work out target
          switch (this->target)
            {
              case 20:
                this->target = 1;
                break;
              case 30:
                this->target = 2;            
                break;     
              case 40:
                this->target = 3;
                break;
              case 50:
                this->target = 4;
                break;
              case 60:
                this->target = 5;
                break;
              case 70:
                this->target = 6;
                break;
              case 80:
                this->target = 7;
                break;
              case 90:
                this->target = 8;
                break;
              case 100:
                this->target = 9;
                break;
              case 110:
                this->target = 10;
                break;
              case 120:
                this->target = 11;
                break;
              case 130:
                this->target = 12;
                break;
              case 140:
                this->target = 13;
                break;
              case 150:
                this->target = 14;
                break;
              case 160:
                this->target = 15;
                break;
              case 170:
                this->target = 16;
                break;
            }
#ifdef DEBUG_MSG
          Serial.print(" T: "); Serial.println(target);
#endif
       }


    // 180 turn
       if ((CMD_180_CW <= this->thisCommand) && (this->thisCommand <= CMD_180_ACW))
       {
         if (this->thisCommand == CMD_180_CW) digitalWrite(MOTOR_PIN, LOW);    //motor cw
         if (this->thisCommand == CMD_180_ACW) digitalWrite(MOTOR_PIN, HIGH);  //motor acw
      // work out target
         switch (this->track)
         {
           case 1:
             this->target = 8;
             break;
           case 2:
             this->target = 10;
             break;
           case 3:
             this->target = 11;
             break;
           case 4:
             this->target = 12;
             break;
           case 5:
             this->target = 13;
             break;
           case 6:
             this->target = 14;
             break;
           case 7:
             this->target = 15;
             break;
           case 8:
             this->target = 1;
             break;
           case 9:
             this->target = 16;
             break;
           case 10:
             this->target = 2;
             break;
           case 11:
             this->target = 3;
             break;
           case 12:
             this->target = 4;
             break;
           case 13:
             this->target = 5;
             break;
           case 14:
             this->target = 6;
             break;
           case 15:
             this->target = 7;
             break;
           case 16:
             this->target = 9;
             break;
         }

#ifdef DEBUG_MSG
      Serial.print(" T: "); Serial.println(this->target);
#endif
      }
      this->state = TT_MOVING;
      this->startMs = millis();
    break;

/*
 * 
 */

    case TT_MOVING:                           // continue moving until the target track is reached

      if ( ( this->thisCommand > CMD_SOLENOID_OFF) && ( this->thisCommand < CMD_ROTATE_CW ))
      {
        digitalWrite(SOLENOID_PIN, HIGH); //solenoid on

        if ( ( millis() - this->startMs ) > this->interval)
        {
          this->CheckSensors();
          if ( this->target == this->track )
          {
            this->state = TT_STOP;
          }
          this->startMs = millis();
        }
      }
      else
      {
        this->state = TT_STOP;
      }



      if ( (this->thisCommand == CMD_ROTATE_CW) || (this->thisCommand == CMD_ROTATE_ACW) )
      {
        
      }

      if ( (this->thisCommand == CMD_STOP_AT_NEXT) )
      {
        this->state = TT_STOP;
      }


    break;

/*
 * 
 */

    case TT_STOP:                             // at the target track turn of the solenoid and return to TT_IDLE move next command in queue to begining of queue
      if ( (this->thisCommand != CMD_ROTATE_CW) && (this->thisCommand != CMD_ROTATE_ACW) )
      {
       memmove(this->commandQueue, this->commandQueue + 1, TT_MOVER_MAX_TRACKS);
      }

      digitalWrite(SOLENOID_PIN, LOW); //solenoid off      

      this->state = TT_IDLE;
    break;
  }
}


// function to give converted command number to command name
//          >  is clockwise
//          <  is anti-clockwise
//
// number is from (address - BaseTurnoutAddress) * 10 + 1


void TtMover::InterpretCommand(byte newcommand)
{
  this->CommandName = "";
  switch (newcommand)
  {
    case CMD_MOTOR_CW:
      this->CommandName = "motor >";
      break;
    case CMD_MOTOR_ACW:
      this->CommandName = "motor <";        //
      break;
    case CMD_SOLENOID_ON:
      this->CommandName = "solenoid ON";
      break;
    case CMD_SOLENOID_OFF:
      this->CommandName = "solenoid OFF";
      break;
    case CMD_GOTO_1_CW:
      this->CommandName = "goto 1 >";
      break;
    case CMD_GOTO_1_ACW:
      this->CommandName = "goto 1 <";
      break;
    case CMD_GOTO_2_CW:
      this->CommandName = "goto 2 >";
      break;
    case CMD_GOTO_2_ACW:
      this->CommandName = "goto 2 <";
      break;
    case CMD_GOTO_3_CW:
      this->CommandName = "goto 3 >";
      break;
    case CMD_GOTO_3_ACW:
      this->CommandName = "goto 3 <";
      break;
    case CMD_GOTO_4_CW:
      this->CommandName = "goto 4 >";
      break;
    case CMD_GOTO_4_ACW:
      this->CommandName = "goto 4 <";
      break;
    case CMD_GOTO_5_CW:
      this->CommandName = "goto 5 >";
      break;
    case CMD_GOTO_5_ACW:
      this->CommandName = "goto 5 <";
      break;
    case CMD_GOTO_6_CW:
      this->CommandName = "goto 6 >";
      break;
    case CMD_GOTO_6_ACW:
      this->CommandName = "goto 6 <";
      break;
    case CMD_GOTO_7_CW:
      this->CommandName = "goto 7 >";
      break;
    case CMD_GOTO_7_ACW:
      this->CommandName = "goto 7 <";
      break;
    case CMD_GOTO_8_CW:
      this->CommandName = "goto 8 >";
      break;
    case CMD_GOTO_8_ACW:
      this->CommandName = "goto 8 <";
      break;
    case CMD_GOTO_9_CW:
      this->CommandName = "goto 9 >";
      break;
    case CMD_GOTO_9_ACW:
      this->CommandName = "goto 9 <";
      break;
    case CMD_GOTO_10_CW:
      this->CommandName = "goto 10 >";
      break;
    case CMD_GOTO_10_ACW:
      this->CommandName = "goto 10 <";
      break;
    case CMD_GOTO_11_CW:
      this->CommandName = "goto 11 >";
      break;
    case CMD_GOTO_11_ACW:
      this->CommandName = "goto 11 <";
      break;
    case CMD_GOTO_12_CW:
      this->CommandName = "goto 12 >";
      break;
    case CMD_GOTO_12_ACW:
      this->CommandName = "goto 12 <";
      break;
    case CMD_GOTO_13_CW:
      this->CommandName = "goto 13 >";
      break;
    case CMD_GOTO_13_ACW:
      this->CommandName = "goto 13 <";
      break;
    case CMD_GOTO_14_CW:
      this->CommandName = "goto 14 >";
      break;
    case CMD_GOTO_14_ACW:
      this->CommandName = "goto 14 <";
      break;
    case CMD_GOTO_15_CW:
      this->CommandName = "goto 15 >";
      break;
    case CMD_GOTO_15_ACW:
      this->CommandName = "goto 15 <";
      break;
    case CMD_GOTO_16_CW:
      this->CommandName = "goto 16 >";
      break;
    case CMD_GOTO_16_ACW:
      this->CommandName = "goto 16 <";
      break;

    case CMD_STEP_CW:
      this->CommandName = "step >";
      break;
    case CMD_STEP_ACW:
      this->CommandName = "step <";
      break;
    case CMD_180_CW:
      this->CommandName = "turn 180 >";
      break;
    case CMD_180_ACW:
      this->CommandName = "turn 180 <";
      break;
    case CMD_ROTATE_CW:
      this->CommandName = "rotate >";
      break;
    case CMD_ROTATE_ACW:
      this->CommandName = "rotate <";
      break;
    case CMD_STOP_AT_NEXT:
      this->CommandName = "stop at next";
      break;
    case CMD_SPARE:
      this->CommandName = "spare";
      break;  
  }
}


void TtMover::CheckSensors()
{
  // need to look at noise suppression
  int x = 0;
  int z = 0; //for getting average

  this->lastTrack = this->track;

  for (int j = HALL_1; j < HALL_10; j++)
   {
    //crude attempt at suppression LOW = ON
    z = 0;
    for (int n=1; n<12; n++)
     {
      z = z + (digitalRead(j));
     }

   if ( z < 4) x = j;
   }
  if (x = 0)
   {
    for (int j = HALL_10; j <= HALL_16; j++)
     {
      //crude attempt at suppression LOW = ON
      z = 0;
      for (int n=1; n<12; n++)
       {
//        z = z + (digitalRead(j));
        z = z + (dr(j));
       }
      if ( z < 4) x = j;
     }
   }

  
  if ( x > 0)
  {
    switch (x)
    {
      case HALL_1:
        this->track = 1;      // Also track 8 reverse
        break;
      case HALL_2:
        this->track = 2;
        break;
      case HALL_3:
        this->track = 3;      // Also track 11 reverse
        break;
      case HALL_4:
        this->track = 4;
        break;
      case HALL_5:
        this->track = 5;
        break;
      case HALL_6:
        this->track = 6;
        break;
      case HALL_7:
        this->track = 7;
        break;
      case HALL_8:      // Also track 1 reverse
        this->track = 8;
        break;
      case HALL_9:
        this->track = 9;
        break;
      case HALL_10:
        this->track = 10;     // track 2 reverse
        break;
      case HALL_11:
        this->track = 11;     // Also track 3 reverse
        break;
      case HALL_12:
        this->track = 12;     // track 4 reverse
        break;
      case HALL_13:
        this->track = 13;     // track 5 reverse
        break;
      case HALL_14:
        this->track = 14;     // track 6 reverse
        break;  
      case HALL_15:
        this->track = 15;     // track 7 reverse
        break;  
      case HALL_16:
        this->track = 16;     // track 9 reverse
        break;
    }

#ifdef DEBUG_MSG
  Serial.print("Track = "); Serial.println(track);
#endif 

  }
  else
  { 

#ifdef DEBUG_MSG_2
  Serial.println("checksensors moving ");
  Serial.print("last track = "); Serial.println(lastTrack);
#endif 

  }

}


bool TtMover::dr(int pin)
{
  int val = analogRead(pin);
  return ( val > 512 ) ;
}


  
