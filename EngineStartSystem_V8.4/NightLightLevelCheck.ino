/* ------------------------------------------------------------------
                     NIGHT LIGHT LEVEL CHECK FUNCTION
---------------------------------------------------------------------*/

void nightLightLevelCheck(){
  
  Log("nightLightLevelCheck START @: " + String(millis()));

  ChokeServo.detach();                // cuts off power to the servo
  
  int photocellReading;       // the analog reading from the analog resistor divider

  do
  {
    photocellReading = analogRead(A0);                // Read the analogue pin

    Log("Light level = " + String(photocellReading));

    if(photocellReading > 110)    // TESTED AT NIGHT!!!
    {
      Log("Too bright. Waiting for night (10 min)... @: " + String(millis()));
      delay(10UL*60UL*1000UL); // waits for 10 minutes before checking again
    }
  } while (photocellReading > 110);
  
  Log("nightLightLevelCheck END @: " + String(millis()));
}


/*
void nightLightLevelCheck(){
  
  Log("nightLightLevelCheck START");

  ChokeServo.detach();                // cuts off power to the servo
  
  int photocellReading;       // the analog reading from the analog resistor divider
  float Res = 10.0;           // Resistance in the circuit of sensor 1 (KOhms)
  float lux = 0.0;

  do
  {
    photocellReading = analogRead(A0);                // Read the analogue pin
    float Vout = photocellReading * (3.3 / 1024.0);   // calculate the voltage
    lux =500 / (Res * ((3.3-Vout) / Vout));           // calculate the Lux
    if(lux > 50.0)
    {
      Log("Waiting sunset for another 60minutes");
      delay(60UL*60UL*1000UL); // waits for 1 hour
    }
  } while (lux > 50.0);

  Log("nightLightLevelCheck END");
}
*/
