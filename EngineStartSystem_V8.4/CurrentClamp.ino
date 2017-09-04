
boolean WellPumpCurrentClampCheck()
{
  Log("WellPumpCurrentClampCheck BEGIN");

  delay(100);
  WellPumpIrmsNew = (WellPumpCurrentClamp.calcIrms(100))/10;  // Calculate Irms only (1480)
  delay(50);
  
  Log("Well Pump Current = " + String(WellPumpIrmsNew) + " A");
  
  if(WellPumpIrmsNew >= (WellPumpIrmsOld/4))    // Detecst a 4X drop in the current 
  {
        Log("WELL PUMP IS WORKING!");
        WellPumpIrmsOld = WellPumpIrmsNew;
        return 1; 
  }
  else
  {
    Log("WELL PUMP IS IDLE!");
    WellPumpIrmsOld = 0;
    return 0;
  }
 

}
