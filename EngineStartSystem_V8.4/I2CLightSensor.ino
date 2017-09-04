
void I2CLightSensorCheck(){

  Log("I2CLightSensorCheck START @: " + String(millis()));
  
  // Wait integration timer (ms) between measurements before retrieving the result
  delay(ms);
  
  // Once integration is complete, we'll retrieve the data.
  
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.
  // Retrieve the data from the device:

  unsigned int data0, data1;
  
  if (I2CLightSensor.getData(data0,data1))
  {
    // To calculate lux, pass all your settings and readings to the getLux() function.
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:
    good = I2CLightSensor.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
    Log("I2C Light Sensor: " + String(lux) + " lux");
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.
    Log("TSL2561 ERROR @: " + String(millis()));
  }

  Log("I2CLightSensorCheck END @: " + String(millis()));
}
