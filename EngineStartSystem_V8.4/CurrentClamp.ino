
boolean WellPumpCurrentClampCheck()
{
  Log("WellPumpCurrentClampCheck BEGIN");
  
  WellPumpIrmsNew = WellPumpCurrentClamp.calcIrms(1480);  // Calculate Irms only

  Log("Well Pump Current = " + String(WellPumpIrmsNew) + " A");
  
  if(WellPumpIrmsNew >= (WellPumpIrmsOld/2))
  {
        Log("WELL PUMP IS WORKING!");
        WellPumpIrmsOld = WellPumpIrmsNew;
        return 1; 
  }
 
  Log("WELL PUMP IS IDLE!");
  WellPumpIrmsOld = 0;
  return 0;
}
