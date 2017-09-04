/* ------------------------------------------------------------------
                      TEMPERATURE SENSOR DS18B20
---------------------------------------------------------------------*/
void readTemperatureSensors()
{ 
  float tempC = 0;
  
  sensors.requestTemperatures();

  tempC = sensors.getTempC(insideThermometer);
  if (tempC == -127.00) {
    Log("ERROR getting inside temperature");
  } else {
    Log("Inside temperature is " + String(tempC) + " degrees C");
  }

  tempC = sensors.getTempC(outsideThermometer);
  if (tempC == -127.00) {
    Log("ERROR getting outside temperature");
  } else {
    Log("Outside temperature is " + String(tempC) + " degrees C");
  }
}
