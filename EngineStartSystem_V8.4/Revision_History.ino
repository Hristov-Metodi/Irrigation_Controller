/* ------------------------------------------------------------------
.
.
.
Version 8.3:
 - Revision History added
 - General formatting eddition
 - ARDUINO RESET @ CRANCKING CHECK proposal NOTE
 - unsigned long duration = (endTankFillUpCycleMillis - startTankFillUpCycleMillis); // / (1000UL * 60UL);
 - chokeCloseAngle from 54 to 60
 - unsigned long duration = (millis() - startTankFillUpCycleMillis) / (1000UL * 60UL);
 - @: "+ String(millis()) added to all functions
 
 Version8.4
 - addition of the Current Clamp (YHDC SCT-013-000)
   REF: https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino?redirected=true
 - addition of a SPI Light Sensor (TSL2561) - get the reading at the same time as the analogues ensor,
   however do NOT base the logic on the TSL2561's reading yet
   REF: https://learn.sparkfun.com/tutorials/tsl2561-luminosity-sensor-hookup-guide
 - I2CLightSensorCheck TESTING: I2CLightSensorCheck() called before every dayLightLevelCheck() function.
 - WellPumpCurrentClampCheck TESTING: WellPumpCurrentClampCheck() called after every GeneratorStatusCheck
 - Current clamp sensor problems - high initial value
 - Current clamp sensor problem fixed - delay of 5 measirement cycles before actually comparing values (TESTED)
 - Well Pump pumping cycles duration calculations (ms and min) (TESTED)
 - TANK level (water volume in liters) estimation calculation added
 - dayLightLevelCheck level increased to 685 (EXPERIMENTAL VALUE -  Sun through a light cloud)

  Verison 8.5
 - Addition of the 1" 1/4 Tank Valve
 - Addition of Temperature Sensor(s) DS18B20
   (https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806)
   See the tutorial on how to obtain the sensor's addresses:
   http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
---------------------------------------------------------------------*/
