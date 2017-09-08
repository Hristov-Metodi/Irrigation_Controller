/* ------------------------------------------------------------------
                    WELL PUMP CURRENT CLAMP FUNCTION
CONNECTIONS:
1. 5V   (red)
2. GND  (black)
3. A1   (white)
---------------------------------------------------------------------*/
boolean WellPumpCurrentClampCheck()
{
  Log("WellPumpCurrentClampCheck START @: " + String(millis()));

  if(wellPumpCurrentClampCounter < 5){        // Measurement delay by 5 check cycles
    //Log("wellPumpCurrentClampCounter = " + String(wellPumpCurrentClampCounter));
    
    delay(10);
    WellPumpIrmsNew = (WellPumpCurrentClamp.calcIrms(1480))/10;  // Calculate Irms only (1480)
    delay(10);
    Log("Well Pump Current = " + String(WellPumpIrmsNew) + " A");
    
    Log("WELL PUMP IS WORKING! @: " + String(millis()));
    wellPumpCurrentClampCounter ++;
    return 1;
  }
  
  delay(10);
  WellPumpIrmsNew = (WellPumpCurrentClamp.calcIrms(1480))/10;  // Calculate Irms only (1480)
  delay(10);
  Log("Well Pump Current = " + String(WellPumpIrmsNew) + " A");
  
  if(WellPumpIrmsNew >= (WellPumpIrmsOld/2))    // Detecst a 4X drop in the current 
  {
        Log("WELL PUMP IS WORKING! @: " + String(millis()));
        WellPumpIrmsOld = WellPumpIrmsNew;
        wellPumpCurrentClampCounter = 0;
        return 1; 
  }
  else
  {
    Log("WELL PUMP IS IDLE! @: " + String(millis()));
    WellPumpIrmsOld = 0;
    wellPumpCurrentClampCounter = 0;
    return 0;
  }
}
