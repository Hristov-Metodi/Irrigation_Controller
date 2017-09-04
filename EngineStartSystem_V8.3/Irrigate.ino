/* ------------------------------------------------------------------
              MANUEL IRRIGATION START SEQUENCE FUNCTION
---------------------------------------------------------------------*/
void Irrigate(){

  Log("Irrigate START @:" + String(millis()));
  
  boolean tankEmpty;
 
  do
  {
    tankEmpty = IsTankEmpty();
    
    digitalWrite(IrrigationPumpControl, LOW); // Turns ON the irrigation pump repaly
    IRRIGATION_PUMP_STAT = 1;
    
    delay(2000);
    
  }while(tankEmpty == 0);

  digitalWrite(IrrigationPumpControl, HIGH);  // Turns OFF the irrigation pump relay
  IRRIGATION_PUMP_STAT = 0;
  Log("Irrigate END @: " + String(millis()));
}

/* ------------------------------------------------------------------
            AUTONOMOUS IRRIGATION START SEQUENCE FUNCTION
---------------------------------------------------------------------*/
void StartAutoIrrigate(){

  boolean generator_running;
  boolean tankEmpty;
  unsigned long timerChecker = 0;

  Log("Auto Irrigate START @: " + String(millis()));

  do
  {
    tankEmpty = IsTankEmpty();
    
    if(tankEmpty == 0)
    {
      digitalWrite(IrrigationPumpControl, LOW); // Turns ON the irrigation pump repaly
      IRRIGATION_PUMP_STAT = 1;
      Log("Irrigating...");
    }

    delay(900);
    generator_running = GeneratorStatusCheck();
    timerChecker += 4000;
    Log(String(timerChecker));
  
  }while((tankEmpty == 0) && (timerChecker < irrigationTime) && generator_running); //exit this loop when: Tank=FULL or PumpTime=ELAPSED or Generator=IDLE
 
  Log("Tank is " + String((tankEmpty)?"EMPTY":"NotEmpty") + ". Timer = " + String(timerChecker) + ". GeneratorRunning = " + String(generator_running));

  Log("StartStartAutoIrrigate END @: " + String(millis()));
}

/* ------------------------------------------------------------------
                   IRRIGATION PUMP STOP FUNCTION
---------------------------------------------------------------------*/

void StopIrrigationPump(){
  Log("StopIrrigationPump BEGIN @: "+ String(millis()));
  digitalWrite(IrrigationPumpControl, HIGH);
  IRRIGATION_PUMP_STAT = 0;
  delay(1000);
  Log("StopIrrigationPump END @: "+ String(millis()));
}
