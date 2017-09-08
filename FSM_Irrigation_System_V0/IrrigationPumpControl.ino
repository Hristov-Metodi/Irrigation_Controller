enum{
  IRRIGATION_PUMP_OFF = 0,
  IRRIGATION_PUMP_ON,
  GEN_CHAMBER_TOO_HOT_REST,
};

unsigned char stateIrrigationPump = WELL_PUMP_OFF;

void IrrigationPumpInitiate(void)
{
  stateIrrigationPump = IRRIGATION_PUMP_ON;
}

void IrrigationPump()
{
  unsigned char prevState = 0xFF;
  static unsigned long timeOut = 0;

  // IRRIGATION PUMP :: State Machine:

  if((timeOut == 0) || (timeOut < millis()))
  {
    switch(stateIrrigationPump)
    {
      case IRRIGATION_PUMP_OFF:
      digitalWrite(IrrigationPumpControl, HIGH);
      IRRIGATION_PUMP_STAT = 0;
      timeOut = millis() + 1000;

      if(itIsNight && tankIsFull || restingPeriodOver)
      {
        stateIrrigationPump = IRRIGATION_PUMP_ON;
      }
      break;
    
      case IRRIGATION_PUMP_ON:
      digitalWrite(IrrigationPumpControl, LOW);
      IRRIGATION_PUMP_STAT = 1;

      if(generatorRunning && tankIsNotEmpty)
      {
        break;
      }

      else if(tankIsEmpty)
      {
        stateIrrigationPump = IRRIGATION_PUMP_OFF;
        break;
      }

      else if (chamberTemperature >= 55)
      {
        stateIrrigationPump = GEN_CHAMBER_TOO_HOT_REST;
        break;
      }

      case GEN_CHAMBER_TOO_HOT_REST:
      stateIrrigationPump = IRRIGATION_PUMP_OFF;
      timeOut = millis() + oneHour;
      break;
      
      default:    
      break;
    }
  
    if(prevState != stateIrrigationPump)
    {
    Log("IrrigationPump current state is: " + String(stateGenStarting));
    prevState = stateIrrigationPump;
    }
  }
}

