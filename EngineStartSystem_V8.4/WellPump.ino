/* ------------------------------------------------------------------
                        WELL PUMP START FUNCTION V2
---------------------------------------------------------------------*/
void StartWellPump(){
  boolean generator_running;
  boolean tankFull;
  boolean wellPumpRunning;
  unsigned long timerChecker = 0;

  Log("StartWellPump BEGIN @: " + millis());  
  do
  {
    tankFull = IsTankFull();
    
    if(tankFull==0)
    {
      digitalWrite(WellPumpControl, LOW);
      WELL_PUMP_STAT = 1;
    }

    delay(900);
    generator_running = GeneratorStatusCheck();
    timerChecker += 4000;

    wellPumpRunning = WellPumpCurrentClampCheck();
    
    Log("Well Pump has been working for: " + String(timerChecker) + "ms");
    
  }while((tankFull == 0) && (wellPumpRunning) && (timerChecker < wellPumpingTime) && (generator_running));
  //exit this loop when: Tank=FULL or PumpTime=ELAPSED or wellPumpRunning = IDLE or Generator=IDLE

  Log("Tank is " + String((tankFull)?"FULL":"NotFull") + ". Timer = " + String(timerChecker) + ". GeneratorRunning = " + String(generator_running));
   
  Log("StartWellPump END @: " + millis());
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
  Log("StopWellPump BEGIN");
  digitalWrite(WellPumpControl, HIGH);
  WELL_PUMP_STAT = 0;
  delay(1000);
  Log("StopWellPump END");
}
