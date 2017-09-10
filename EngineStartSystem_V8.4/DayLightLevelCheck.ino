/* ------------------------------------------------------------------
                       DAY LIGHT LEVEL CHECK FUNCTION
CONNECTIONS:
1. 3.3V (yellow)
2. A0   (green)
3. 1K Ohm Pull Down resistor between GND and A0
---------------------------------------------------------------------*/

void dayLightLevelCheck(){

  Log("dayLightLevelCheck START @: " + String(millis()));

  ChokeServo.detach();                // cuts off power to the servo
  
  int photocellReading;       // the analog reading from the analog resistor divider

  do
  {
    photocellReading = analogRead(A0);                // Read the analogue pin
    
    Log("Light level = " + String(photocellReading));
    
    if(photocellReading < 665)    // Sun through a light cloud
    {
      Log("Too dark. Waiting for sun (10 min)... @: " + String(millis()));
      delay(10UL*60UL*1000UL); // waits for 10 minutes before checking again
    }
  } while (photocellReading <= 665);
  
  Log("dayLightLevelCheck END @: " + String(millis()));
}

