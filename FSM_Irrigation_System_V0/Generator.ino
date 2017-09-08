/*----------------------------------------------------------------------------------------
                                    GENERATOR :: IDLE
 ----------------------------------------------------------------------------------------*/
void GeneratorIdle(void)
{
    digitalWrite(FuelValveRelay, HIGH);  // Turns OFF the fuel line
    FUEL_VALVE_STAT = 0; 
}

/*----------------------------------------------------------------------------------------
                                    GENERATOR :: RUNNING
 ----------------------------------------------------------------------------------------*/
void GeneratorRunning(void)
{
    digitalWrite(FuelValveRelay, LOW);  // Turns ON the fuel line
    FUEL_VALVE_STAT = 1;
}

/*----------------------------------------------------------------------------------------
                                    GENERATOR :: STARTING
 NOTE: ADD LIGHT CKECK - follow light conditions form previous SW versions!!!
 ----------------------------------------------------------------------------------------*/
enum{
  GENSTARTING_IDLE = 0,
  GENSTARTING_FUEL_VALVE_ON,
  GENSTARTING_CHOKE_SERVO_OPEN_ATTACH,
  GENSTARTING_CHOKE_SERVO_OPEN,
  GENSTARTING_CHOKESERVO_OPEN_DETACH,
  GENSTARTING_CRANCK_RESET_CHECK,
  GENSTARTING_CRANCKING_ON,
  GENSTARTING_CRANCKING_OFF,
  GENSTARTING_OPEN_CHOKE_TIMER,
  GENSTARTING_CHOKE_SERVO_CLOSE_ATTACH,
  GENSTARTING_CHOKE_SERVO_CLOSE,
  GENSTARTING_CHOKE_SERVO_CLOSE_DETACH,
  GENSTARTING_CRANCK_RESET_SET_TO_ZERO,
  GENSTARTING_STARTING_GENERATOR_COMPLETE,
};

unsigned char stateGenStarting = GENSTARTING_IDLE;

void GeneratorStartingInitiate(void)
{
  stateGenStarting = GENSTARTING_FUEL_VALVE_ON;
}

void GeneratorStarting()
{
  unsigned char prevState = 0xFF;
  static unsigned long timeOut = 0;
  
  // GENERATOR :: STARTING State Machine:

  if((timeOut == 0) || (timeOut < millis()))
  {
    switch(stateGenStarting)
    {
      case GENSTARTING_IDLE:
      break;
    
      case GENSTARTING_FUEL_VALVE_ON:
      digitalWrite(FuelValveRelay, LOW);  // Turns ON the fuel line
      FUEL_VALVE_STAT = 1;
      timeOut = millis() + 3000;          // wait for the fuel to reach the carborator
      stateGenStarting = GENSTARTING_CHOKE_SERVO_OPEN_ATTACH;
      break;
      
      case GENSTARTING_CHOKE_SERVO_OPEN_ATTACH:
      ChokeServoPos = chokeOpenAngle;     // the Choke Servo's OPENED position
      ChokeServo.attach(ChokeServoPin);   // attaches the servo on pin 3 to the servo object
      timeOut = millis() + 15;
      stateGenStarting = GENSTARTING_CHOKE_SERVO_OPEN;
      break;
      
      case GENSTARTING_CHOKE_SERVO_OPEN:
      ChokeServo.write(ChokeServoPos);    // sets the servo position to OPEN
      timeOut = millis() + 1000;                        // waits for it to get to the position
      stateGenStarting = GENSTARTING_CHOKESERVO_OPEN_DETACH;
      break;

      case GENSTARTING_CHOKESERVO_OPEN_DETACH:
      ChokeServo.detach();                // cuts off power to the servo
      CHOKE_STAT = 1;
      timeOut = millis() + 500;
      stateGenStarting = GENSTARTING_CRANCK_RESET_CHECK;
      break;
      
      case GENSTARTING_CRANCK_RESET_CHECK:
      cranck_reset_check = 1;
      EEPROM.put(10, cranck_reset_check);  // Store cranck_reset_check in EEPROM
      delay(10); // MUST BE A DELAY TO ALLOW TIME FOR WRITING!
      stateGenStarting = GENSTARTING_CRANCKING_ON;
      break;
      
      case GENSTARTING_CRANCKING_ON:
      digitalWrite(CrankControl, LOW);    // start CRANCKING
      CRANK_CONTROL_STAT = 1;
      timeOut = millis() + crankTime;     // defines CRANKING DURATION
      stateGenStarting = GENSTARTING_CRANCKING_OFF;
      break;
      
      case GENSTARTING_CRANCKING_OFF:
      digitalWrite(CrankControl, HIGH);   // stop CRANKING
      CRANK_CONTROL_STAT = 0;
      stateGenStarting = GENSTARTING_OPEN_CHOKE_TIMER;
      break;
      
      case GENSTARTING_OPEN_CHOKE_TIMER:
      timeOut = millis() + openChokeTime; // defines the duration the engine will run with an onpen choke
      stateGenStarting = GENSTARTING_CHOKE_SERVO_CLOSE_ATTACH;
      break;

      case GENSTARTING_CHOKE_SERVO_CLOSE_ATTACH:
      ChokeServoPos = chokeCloseAngle;    // the Choke Servo's CLOSE position angle = 100
      ChokeServo.attach(ChokeServoPin);   // attaches the servo on pin 3 to the servo object
      timeOut = millis() + 15;
      stateGenStarting = GENSTARTING_CHOKE_SERVO_CLOSE;
      break;  

      case GENSTARTING_CHOKE_SERVO_CLOSE:
      ChokeServo.write(ChokeServoPos);    // sets the servo position to CLOSE
      timeOut = millis() + 1000;                        // waits for it to get to the position
      stateGenStarting = GENSTARTING_CHOKE_SERVO_CLOSE_DETACH;
      break;

      case GENSTARTING_CHOKE_SERVO_CLOSE_DETACH:
      ChokeServo.detach();
      timeOut = millis() + 500;
      CHOKE_STAT = 0;
      stateGenStarting = GENSTARTING_CRANCK_RESET_SET_TO_ZERO;
      break;
      
      case GENSTARTING_CRANCK_RESET_SET_TO_ZERO:
      cranck_reset_check = 0;
      EEPROM.put(10, cranck_reset_check);  // Store cranck_reset_check in EEPROM
      delay(10);
      stateGenStarting = GENSTARTING_STARTING_GENERATOR_COMPLETE;
      break;
          
      case GENSTARTING_STARTING_GENERATOR_COMPLETE:
      break;
      
      default:    
      break;
    }
  
    if(prevState != stateGenStarting)
    {
    Log("GeneratorStarting current state is: " + String(stateGenStarting));
    prevState = stateGenStarting;
    }
  }
}

/*----------------------------------------------------------------------------------------
                                    GENERATOR :: STOP
 ----------------------------------------------------------------------------------------*/
enum
{
  GENSTOP_IDLE = 0,
  GENSTOP_FUEL_VALVE_OFF,
  GENSTOP_ENGINE_CUT_OFF_ENAGAGED,
  GENSTOP_ENGINE_CUT_OFF_DISENAGAGED,
  GENSTOP_STOPPING_GENERATOR_COMPLETE,
};

unsigned char stateGenStop = GENSTOP_IDLE;

void GeneratorStopInitiate(void)
{
  stateGenStop = GENSTOP_FUEL_VALVE_OFF;
}

void GeneratorStop()
{
  unsigned char prevState=0xFF;
  static unsigned long timeOut = 0;
  
  // GENERATOR :: STOP State Machine:

  if((timeOut ==0) || (timeOut < millis()))
  {
    switch(stateGenStop)
    {
      case GENSTOP_IDLE:
      break;

      case GENSTOP_FUEL_VALVE_OFF:
      digitalWrite(FuelValveRelay, HIGH);  // Turns OFF the fuel line
      FUEL_VALVE_STAT = 0;
      timeOut = millis() + 100;
      stateGenStop = GENSTOP_ENGINE_CUT_OFF_ENAGAGED;
      break;
      
      case GENSTOP_ENGINE_CUT_OFF_ENAGAGED:
      if(GeneratorStatusCheck != 0)
      {
        digitalWrite(EngineCutOffControl, LOW);  // turns the engine OFF
        ENGINE_CUT_OFF_STAT = 1;
        timeOut = millis() + 500;
      }
      else{
        stateGenStop = GENSTOP_ENGINE_CUT_OFF_DISENAGAGED;
        break;
      }
      
      case GENSTOP_ENGINE_CUT_OFF_DISENAGAGED:
      digitalWrite(EngineCutOffControl, HIGH);  // turns the engine OFF
      ENGINE_CUT_OFF_STAT = 0;
      stateGenStop = GENSTOP_STOPPING_GENERATOR_COMPLETE;
      break;
      
      case GENSTOP_STOPPING_GENERATOR_COMPLETE:
      break;
      
      default:
      break;
    }
    
    if(prevState != stateGenStop)
    {
    Log("GeneratorStop current state is: " + String(stateGenStop));
    prevState = stateGenStop;
    }
  }
}


/*----------------------------------------------------------------------------------------
                                    GENERATOR STATUS CHECK FUNCTION
 ----------------------------------------------------------------------------------------*/
enum
{
  GENSTATUS_IDLE = 0,
  GENSTATUS_MEASUREMENT_LOOP,
  GENSTATUS_EVALUATION_RESULTS,
  GENSTATUS_GENERATOR_STATUSCHECK_COMPLETE,
};

unsigned char stateGenStatusCheck = GENSTATUS_IDLE;

void GeneratorStatusCheckInitiate(void)
{
  stateGenStatusCheck = GENSTATUS_MEASUREMENT_LOOP;
}

void GeneratorStatusCheck()
{
  unsigned char prevState=0xFF;
  static unsigned long timeOut = 0;
  
  // GENERATOR :: STATUS CHECK State Machine:

  if((timeOut ==0) || (timeOut < millis()))
  {
    switch(stateGenStatusCheck)
    {
      case GENSTATUS_IDLE:
      break;

      case GENSTATUS_MEASUREMENT_LOOP:
        int meascycles; // number of IGN VOLTAGE measureents
        int counter = 0;
        boolean value; 
      for(meascycles = 0 ; meascycles < 100; meascycles++)
      {
        value = digitalRead(VoltageMeasurement);
        
        if(value == 0)
        {
          counter++;
        }
        timeOut = millis() + 10;
      }
      stateGenStatusCheck = GENSTATUS_EVALUATION_RESULTS;
      break;
      
      case GENSTATUS_EVALUATION_RESULTS:
      if(counter >= 80)
      {
        IGN_STAT = 1;
        GeneratorStatusCheck = GENSTATUS_GENERATOR_STATUSCHECK_COMPLETE;           
      }
      IGN_STAT = 0;
      GeneratorStatusCheck = GENSTATUS_GENERATOR_STATUSCHECK_COMPLETE;
      break;
      
      case GENSTATUS_GENERATOR_STATUSCHECK_COMPLETE:
      break;

      default:
      break;
    }
    
    if(prevState != stateGenStatusCheck)
    {
    Log("GeneratorStatusCheck current state is: " + String(stateGenStatusCheck));
    prevState = stateGenStatusCheck;
    }
  }
}
