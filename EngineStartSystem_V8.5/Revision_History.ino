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

 Version 8.4
 - addition of the Current Clamp (YHDC SCT-013-000)
   REF: https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino?redirected=true
 - addition of a SPI Light Sensor (TSL2561) - get the reading at the same time as the analogues ensor,
   however do NOT base the logic on the TSL2561's reading yet
   REF: https://learn.sparkfun.com/tutorials/tsl2561-luminosity-sensor-hookup-guide
 
 Verison 8.5
 - Addition of the 1" 1/4 Tank Valve
 - Addition of Temperature Sensor(s) DS18B20
   (https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806)
   See the tutorial on how to obtain the sensor's addresses:
   http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
 ---------------------------------------------------------------------*/
