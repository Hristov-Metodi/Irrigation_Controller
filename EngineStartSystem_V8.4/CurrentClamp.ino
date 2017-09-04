
boolean WellPumpCurrentClampCheck()
{
  Log("WellPumpCurrentClampCheck START @: " + String(millis()));

  delay(100);
  WellPumpIrmsNew = (WellPumpCurrentClamp.calcIrms(500))/10;  // Calculate Irms only (1480)
  delay(100);
  
  Log("Well Pump Current = " + String(WellPumpIrmsNew) + " A");
  
  if(WellPumpIrmsNew >= (WellPumpIrmsOld/4))    // Detecst a 4X drop in the current 
  {
        Log("WELL PUMP IS WORKING! @: " + String(millis()));
        WellPumpIrmsOld = WellPumpIrmsNew;
        return 1; 
  }
  else
  {
    Log("WELL PUMP IS IDLE! @: " + String(millis()));
    WellPumpIrmsOld = 0;
    return 0;
  }
 

}
