/* ------------------------------------------------------------------
                        USE ARDUINO MEGA R3 2560
---------------------------------------------------------------------*/

#include <Servo.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <EmonLib.h>                   // Current Clamp library (YHDC SCT-013-000)
#include <SparkFunTSL2561.h>           // Light sensor library (TSL2561)
#include <Wire.h>
#include <DallasTemperature.h>

Servo ChokeServo;                      // Engine Choke Servo
EnergyMonitor WellPumpCurrentClamp;    // Create an instance
SFE_TSL2561 I2CLightSensor;            

const double SoftwareVersion    = 8.5;
const int SDCardSelect          = 4;   // SPI chip select selecting the SD Card (CONNECTED TO PIN 4 PWM)
const int ChokeServoPin         = 10;  // Choke Servo control pin (CONNECTED TO PIN 11 PWM)
const int CrankControl          = 32;  // crank up the engine (NORMALLY OPEN POSITION OF THE RELAY)(CONNECTED TO PIN 32)
const int EngineCutOffControl   = 30;  // ENGINE CUT OFF (NORMALLY OPEN POSITION OF THE RELAY) (CONNECTED TO PIN 30)
// http://www.remotemonitoringsystems.ca/projects/generator/generator_project.php
const int WellPumpControl       = 28;  // WELL PUMP rellay control pin (NORMALLY OPEN POSITION OF THE RELAY) (CONNECTED TO PIN 28)
const int IrrigationPumpControl = 26;  // IRRIGATION PUMP rellay control pin (NORMALLY OPEN POSITION OF THE RELAY) (CONNECTED TO PIN 26)
const int TankValve             = 24;  // TANK VALVE rellay control (CONNECTED TO PIN 24)
const int FuelValveRelay           = 22;  // control of the fuel valve relay (CONNECTED TO PIN 22)
const int VoltageMeasurement       = 34;  // 12V Generator ON voltage measurement (CONNECTED TO PIN 34)
const int FloatSensorPowerSupply   = 36;  // float sensor boar power supply control (CONNECTED TO PIN 36)
const int BottomFloatSensor_1      = 48;  // bottom float sensor 1 (CONNECTED TO PIN 48)
const int BottomFloatSensor_2      = 46;  // bottom float sensor 2 (CONNECTED TO PIN 46)
//const int BottomFloatSensor_2      = 44; // bottom float sensor 2 (CONNECTED TO PIN 44)
const int TopFloatSensor_1         = 42;  // top float sensor 1 (CONNECTED TO PIN 42)
const int TopFloatSensor_2         = 40;  // top float sensor 2 (CONNECTED TO PIN 40)
//const int TopFloatSensor_3         = 38; // top float sensor 2 (CONNECTED TO PIN 38)
const int ONE_WIRE_BUS             = 2;   // OneWire Pin (Temperature Sensor(s)) (CONNECTED TO PIN 2) 

// Servo
int ChokeServoPos;                        // variable to store the CHOKE SERVO position (angle)

// Current Clamp (YHDC SCT-013-000)
float WellPumpIrmsOld = 0;               // old Irms values of the Well Pump
float WellPumpIrmsNew = 0;               // new Irms values of the Well Pump

// Light sensor (TSL2561)
boolean gain;                             // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;                          // Integration ("shutter") time in milliseconds

// EEPROM
unsigned int reset_counter;
unsigned int cranck_reset_check;

const unsigned long wellRecoveryTime        = (60UL*60UL*1000UL); // 1 hour in ms [the time needed for the well to recover 200 liters of water]
const unsigned long wellPumpingTime         = (4UL*60UL*1000UL);  // 4 min in ms  [the time the pump needs to get the 200 liters of well water to the tank]
const unsigned long irrigationTime          = (30UL*60UL*1000UL); // 30 min of irrigation per cycle (3 cycles)
const unsigned long generatorCoolDownTime   = (30UL*60UL*1000UL); // 30 min cool down for the generator (keep the temperature in the shaft at a reasonable level)
const unsigned int crankTime                = 2500;    // 2.5 second in ms [the time it takes for the generator to start when cranked]
const unsigned int openChokeTime            = 500;     // 500 ms [the time the engine will run with an OPEN CHOKE after CRANCKING]
const unsigned int chokeOpenAngle           = 104;     // 104 degrees - the Choke Servo's OPENED position angle
const unsigned int chokeCloseAngle          = 63;      //  63 degrees - the Choke Servo's CLOSED position angle

//const unsigned long fullDay          = 86400000; // 24 hours in ms

const unsigned int numberOfCycles   = 35;     // 160 liters per cycle (number of cycles to fill the water tank [4000 l / 200 l = 20 cycles])
unsigned int cycleCounter           = 0;      // The WellPump counter of the cycles (manipulated by StartWellPump to jump to numberOfCycles if Tank IS FULL)!!!

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the unique addresses of your 1-Wire temp sensors.
DeviceAddress insideThermometer  = { 0x28, 0x94, 0xE2, 0xDF, 0x02, 0x00, 0x00, 0xFE };
DeviceAddress outsideThermometer = { 0x28, 0x6B, 0xDF, 0xDF, 0x02, 0x00, 0x00, 0xC0 };

boolean FUEL_VALVE_STAT       = 0;
boolean CHOKE_STAT            = 0;
boolean CRANK_CONTROL_STAT    = 0;
boolean ENGINE_CUT_OFF_STAT   = 0;
boolean WELL_PUMP_STAT        = 0;
boolean IRRIGATION_PUMP_STAT  = 0;
boolean IGN_STAT              = 0;
boolean IGN_MEASUREMENT_STAT  = 0;
boolean TANK_VALVE_STAT       = 0;

void setup() {
  
  //reset_counter = 0;
  //EEPROM.put(0, reset_counter);         // Zeroes the reset_counter
  //cranck_reset_check = 0;
  //EEPROM.put(10, cranck_reset_check);   // Zeroes the cranck_reset_check
  //cycleCounter = 0;                     
  //EEPROM.put(20, cycleCounter);         // Zeroes the cycleCounter
  
  EEPROM.get(0,  reset_counter);
  EEPROM.get(10, cranck_reset_check);
  EEPROM.put(0,  ++reset_counter);
  
  LogInit("LOG_1.txt", 9600);
  Log("SoftwareVersion = " + String(SoftwareVersion));
  Log("reset_counter = " + String(reset_counter));
  Log("cranck_reset_check=" + String(cranck_reset_check));

  WellPumpCurrentClamp.current(1, 111.1);         // Current: input pin (PIN A1), calibration

  I2CLightSensor.begin();

  // The light sensor has a default integration time of 402ms,
  // and a default gain of low (1X).
  // If you would like to change either of these, you can
  // do so using the setTiming() command.
  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 0;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):
  I2CLightSensor.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  I2CLightSensor.setPowerUp();
  
  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.

  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(insideThermometer, 10);
  sensors.setResolution(outsideThermometer, 10);
  
  pinMode (CrankControl, OUTPUT);                 // CrankControl           - PIN 32
  pinMode (EngineCutOffControl, OUTPUT);          // EngineCutOffControl    - PIN 30
  pinMode (WellPumpControl, OUTPUT);              // WellPumpControl        - PIN 28
  pinMode (IrrigationPumpControl, OUTPUT);        // IrrigationPumpControl  - PIN 26
  pinMode (TankValve, OUTPUT);                    // TankValve              - PIN 24
  pinMode (FuelValveRelay, OUTPUT);               // FuelValveRelay         - PIN 22
  pinMode (VoltageMeasurement, INPUT);            // VoltageMeasurement     - PIN 34
  pinMode (FloatSensorPowerSupply, OUTPUT);       // FloatSensorPowerSupply - PIN 36
  pinMode (BottomFloatSensor_1, INPUT);           // BottomFloatSensor_1    - PIN 48
  pinMode (BottomFloatSensor_2, INPUT);           // BottomFloatSensor_2    - PIN 46
  pinMode (TopFloatSensor_1, INPUT);              // TopFloatSensor_1       - PIN 42
  pinMode (TopFloatSensor_2, INPUT);              // TopFloatSensor_2       - PIN 40
  
  pinMode (A2, OUTPUT);
  pinMode (A3, OUTPUT);
  pinMode (A4, OUTPUT);
  pinMode (A5, OUTPUT);
  pinMode (A6, OUTPUT);
  pinMode (A7, OUTPUT);
  pinMode (A8, OUTPUT);
  pinMode (A9, OUTPUT);
  pinMode (A10, OUTPUT);
  pinMode (A11, OUTPUT);
  pinMode (A12, OUTPUT);
  pinMode (A13, OUTPUT);
  pinMode (A14, OUTPUT);
  pinMode (A15, OUTPUT);
  
  digitalWrite(CrankControl, HIGH);              // makes sure cranking is OFF
  digitalWrite(EngineCutOffControl, HIGH);       // makes sure EngineCutOffControl is ON!
  digitalWrite(WellPumpControl, HIGH);           // makes sure WellPump relay is OFF
  digitalWrite(IrrigationPumpControl, HIGH);     // makes sure IrrigationPump relay is OFF
  digitalWrite(TankValve, HIGH);                 // makes sure TankValve relay is OFF
  digitalWrite(FuelValveRelay, HIGH);            // makes sure FuelValveRelay relay is OFF

  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A6, LOW);
  digitalWrite(A7, LOW);
  digitalWrite(A8, LOW);
  digitalWrite(A9, LOW);
  digitalWrite(A10, LOW);
  digitalWrite(A11, LOW);
  digitalWrite(A12, LOW);
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  digitalWrite(A15, LOW);
}

void loop() {
  /* ------------------------------------------------------------------
                       ARDUINO RESET @ CRANCKING CHECK
  ---------------------------------------------------------------------*/
  EEPROM.get(10, cranck_reset_check);
  
  if(cranck_reset_check != 0){

    Log("cranck_reset_check = " + String(cranck_reset_check));
    Log("Arduino RESTARTED @ Crancking due to insifficienrt battery power!");
        
    dayLightLevelCheck();     // wait for sufficient amount of sunlight
    
    delay(120UL*60UL*1000UL); // expose solar pannel to 2 hour of sufficient sinlight before ignition retry
    
    cranck_reset_check = 0;
    EEPROM.put(10, cranck_reset_check);  // Store cranck_reset_check in EEPROM
    
    Log("cranck_reset_check = " + String(cranck_reset_check));
  /* ------------------------------------------------------------------
                                NEW IDEA:
                                
  Start at 60 minutes waiting time. Double the time every subsequent
  restart while crancking. 2nd time 2*60 minutes 3rd time 3*60 minutes.

  How: Implement a CrankcResetCouner into the EEPROM
  ---------------------------------------------------------------------*/
    
  }
  
  /* ------------------------------------------------------------------
                       FILLING UP THE 4000 LITER TANK
  ---------------------------------------------------------------------*/

  boolean generator_running = 0;
  int start_retry_cnt = 0;
  
  EEPROM.get(20, cycleCounter);
  
  while((IsTankFull() == false) && (cycleCounter < numberOfCycles)){

    unsigned long startTankFillUpCycleMillis = millis();

    Log("\nSTART OF TANK FILLING_UP CYCLE: " + String(cycleCounter) + " @: "+ String(millis()));
    
    dayLightLevelCheck();

    StopGenerator(); // <------ REMOVE WHEN YOU GET TO A STABLE SW/HW!
    
    for(start_retry_cnt = 1; (start_retry_cnt <= 3) && (generator_running == 0); start_retry_cnt++){

      Log("\nIgnition attempt = " + String(start_retry_cnt));
      
      StartGenerator();
      
      generator_running = GeneratorStatusCheck();
      
      if(generator_running == 0){

        Log("Ignition FAILED!");
        
        StopGenerator();

        Log("Wait 1 hour before retrying...");
        
        delay(60UL*60UL*1000UL); // Wait for 1 hour
      }
    }

    if( generator_running != 0)
    {
      Log("Generator RUNNING!");
      
      StartWellPump();
      // WAITING FOR wellPumpingTime is taken care of in StartWellPumpV2 (1 pumping cycle ~ 200 liters of water into the tank)
      StopWellPump();
      StopGenerator();
      
      generator_running = GeneratorStatusCheck(); // Checks if the generator has really been turned OFF
    }
    else
    {
      Log("Generator FAILED to start 3 times in a roll. WAITING FOR ASSISTANCE!");
      Finish();
    }
   
    Log("Starting wellRecoveryTime timer...");
    delay(wellRecoveryTime);
    Log("wellRecoveryTime timer has elapsed.");

    unsigned long duration = (millis() - startTankFillUpCycleMillis) / (1000UL * 60UL);
    Log("END OF TANK FILLING_UP CYCLE: " + String(cycleCounter) + " @: "+ String(millis()) + " (" + String(duration) + " minutes)");
    
    //unsigned long duration = (endTankFillUpCycleMillis - startTankFillUpCycleMillis) / (1000 * 60);
    //Log("FILLING_UP CYCLE DURATION = " + String(duration) + " minutes");

    cycleCounter ++;                // First set the Cycle Counter to cycleCounter ++, so the counter will be correct if the program is stoped during wellRecoveryTime (by me)
    EEPROM.put(20, cycleCounter);
    
  }

  cycleCounter = 0;                     // Zeroes the cycleCounter after the set numberOfCycles has passes
  EEPROM.put(20, cycleCounter);         // Stores the zeroes EEPROM_Cycle_Counter to EEPROM

  
 /* ------------------------------------------------------------------
         AUTONOMOUS IRRIGATION OF THE FOREST (10 LITERS PER TREE)
  ---------------------------------------------------------------------
  
  for(int irCycleCounter = 1; irCycleCounter <= 3; irCycleCounter++){

    Log("\nSTART OF AUTONOMOUS IRRIGATION CYCLE: " + String(irCycleCounter) + " @ " + String(millis()));
    
    nightLightLevelCheck();

    StopGenerator(); // <------ REMOVE WHEN YOU GET TO A STABLE SW/HW!
    
    for(start_retry_cnt = 1; (start_retry_cnt <= 3) && (generator_running == 0); start_retry_cnt++){

      Log("\nIgnition attempt = " + String(start_retry_cnt));
      
      StartGenerator();
      
      generator_running = GeneratorStatusCheck();
      
      if(generator_running == 0){

        Log("Ignition FAILED!");
        
        StopGenerator();

        Log("Wait 1 hour before retrying...");
        
        delay(60UL*60UL*1000UL); // Wait for 1 hour
      }
    }

    if( generator_running != 0)
    {
      Log("Generator RUNNING!");

      OpenTankValve();
      
      StartAutoIrrigate();
      // The irrigationTime is taken care of in the StartAutoIrrigate function (Wait for 30 min)
      StopIrrigationPump();

      CloseTankValve();
      
      StopGenerator();
      
      generator_running = GeneratorStatusCheck(); // Checks if the generator has really been turned OFF
    }
    else
    {
      Log("Generator failed to start 3 times in a roll. WAITING FOR ASSISTANCE!");
      Finish();
    }

    Log("Starting generatorCoolDownTime timer...");
    delay(generatorCoolDownTime);
    Log("generatorCoolDownTime timer has elapsed");

    Log("END OF IRRIGATION CYCLE:  " + String(irCycleCounter)  + " @ " + String(millis()));
  }

*/

/* ------------------------------------------------------------------
            MANUEL IRRIGATION OF THE FOREST (10 LITERS PER TREE)
  ---------------------------------------------------------------------

  StopGenerator();
  generator_running = GeneratorStatusCheck();
  
  StartGenerator();
  
  generator_running = GeneratorStatusCheck();
  if(generator_running == 0){
      
    StopGenerator();
      
    Finish();
  }

  OpenTankValve();
  
  Irrigate();
  // Pump all the water out of the tank
  StopIrrigationPump();

  CloseTankValve();
  
  delay(1000);
    
  StopGenerator();

---------------------------------------------------------------------*/
 

  Log("THE END OF THE FULL IRRIGATION CYCLE HAS BEEN REACHED SUCCESSFULLY. Waiting for RESET...(if infinite Irrigation LOOP is OFF)");
  Finish();   // Comment to make it an infinite Irrigation LOOP!

  /* ------------------------------------------------------------------
    WAIT FOR THE NEXT CYCLE TO BEGIN (EVERY OTHER DAY OF THE WEEK)

    M 10 l/tree
    T 0  l/tree
    W 10 l/tree
    T 0  l/tree
    F 10 l/tree
    S 0  l/tree
    S 10 l/tree
  -------------
  ALL 40 l/tree per week
  ---------------------------------------------------------------------*/
}

void Finish(void)
{
    Log("Finish!");
    LogClose();
    while(1);
}
