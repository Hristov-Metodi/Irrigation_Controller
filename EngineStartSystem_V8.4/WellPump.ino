/* ------------------------------------------------------------------
                        WELL PUMP START FUNCTION V2
---------------------------------------------------------------------*/
void StartWellPump(){
  boolean generator_running;
  boolean tankFull;
  boolean wellPumpRunning;
  unsigned long timerChecker = 0;

  Log("StartWellPump START @: " + String(millis()));
  do
  {
    tankFull = IsTankFull();
    
    if(tankFull==0)
    {
      digitalWrite(WellPumpControl, LOW);
      WELL_PUMP_STAT = 1;
    }

    delay(700);
    generator_running = GeneratorStatusCheck();
    
    wellPumpRunning = WellPumpCurrentClampCheck();

    timerChecker += 4000;
    
    Log("Well Pump has been working for: " + String(timerChecker) + "ms");
    
  }while((tankFull == 0) && (timerChecker < wellPumpingTime) && (generator_running)); // Add  && (wellPumpRunning) after testing
  //exit this loop when: Tank=FULL or PumpTime=ELAPSED or wellPumpRunning = IDLE or Generator=IDLE

  Log("Generator is " + String((generator_running)?"RUNNING":"NotRunning") + ". Well Pump is "+ String((wellPumpRunning)?"RUNNING":"NotRunning") + ". Tank is " + String((tankFull)?"FULL":"NotFull") + ". Timer = " + String(timerChecker));
   
  Log("StartWellPump END @: " + String(millis()));
}

/* ------------------------------------------------------------------
                        WELL PUMP START FUNCTION
---------------------------------------------------------------------
void StartWellPump(){
  
  digitalWrite(WellPumpControl, LOW);
  WELL_PUMP_STAT = 1;
  //DEBUG_PRINT("WELL_PUMP_STAT = ");
  //DEBUG_PRINT(WELL_PUMP_STAT);
  
} */

/* ------------------------------------------------------------------
                        WELL PUMP STOP FUNCTION
---------------------------------------------------------------------*/
void StopWellPump(){
  Log("StopWellPump START @: " + String(millis()));
  digitalWrite(WellPumpControl, HIGH);
  WELL_PUMP_STAT = 0;
  delay(1000);
  Log("StopWellPump END @: " + String(millis()));
}
