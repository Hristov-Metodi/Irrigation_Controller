enum{
  WELL_PUMP_OFF = 0,
  WELL_PUMP_ON,
};

unsigned char stateWellPump = WELL_PUMP_OFF;

void WellPumpInitiate(void)
{
  stateWellPump = WELL_PUMP_ON;
}

void WellPump()
{
  unsigned char prevState = 0xFF;
  static unsigned long timeOut = 0;

  // WELL PUMP :: State Machine:

  if((timeOut == 0) || (timeOut < millis()))
  {
    switch(stateWellPump)
    {
      case WELL_PUMP_OFF:
      digitalWrite(WellPumpControl, HIGH);
      WELL_PUMP_STAT = 0;
      timeOut = millis() + 1000;

      if((cycleCounter >= 0) && (cycleCounter <= numberOfCycles))  // ADD CONDITION FOR SW REQUEST TO START THE WELL PUMP
      {
        stateWellPump = WELL_PUMP_ON;
      }
      break;
    
      case WELL_PUMP_ON:
      digitalWrite(WellPumpControl, LOW);
      WELL_PUMP_STAT = 1;

      if(generatorRunning && wellPumpRunning && (wellPumpingTimerChecker < wellPumpingTime))
      {
        break;
      }

      else
      {
        stateWellPump = WELL_PUMP_OFF;
        timeOut = millis() + wellRecoveryTime;
        break;
      }
      
      default:    
      break;
    }
  
    if(prevState != stateWellPump)
    {
    Log("WellPump current state is: " + String(stateGenStarting));
    prevState = stateWellPump;
    }
  }
}

