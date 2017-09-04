/* ------------------------------------------------------------------
                       DAY LIGHT LEVEL CHECK FUNCTION
---------------------------------------------------------------------*/

void dayLightLevelCheck(){

  Log("dayLightLevelCheck START @: " + String(millis()));

  ChokeServo.detach();                // cuts off power to the servo
  
  int photocellReading;       // the analog reading from the analog resistor divider

  do
  {
    photocellReading = analogRead(A0);                // Read the analogue pin
    
    Log("Light level = " + String(photocellReading));
    
    if(photocellReading < 630)    // BASED ON SD CARD DATA (~ 120 Lux of light)
    {
      Log("Too dark. Waiting for sun (10 min)... @: " + String(millis()));
      delay(10UL*60UL*1000UL); // waits for 10 minutes before checking again
    }
  } while (photocellReading <= 630);
  
  Log("dayLightLevelCheck END @: " + String(millis()));
}

