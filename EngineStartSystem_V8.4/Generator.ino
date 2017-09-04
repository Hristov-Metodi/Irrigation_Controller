/* ------------------------------------------------------------------
                        GENERATOR START FUNCTION
---------------------------------------------------------------------*/
#include <Servo.h>

void StartGenerator(){


  Log("StartGenerator BEGIN");
  
  digitalWrite(FuelValveRelay, LOW);    // Turns ON the fuel line
  FUEL_VALVE_STAT = 1;
  
  delay (3000);    // wait a second for the system to stabilize

  ChokeServoPos = chokeOpenAngle;     // the Choke Servo's OPENED position
  ChokeServo.attach(ChokeServoPin);   // attaches the servo on pin 3 to the servo object
  delay(15);
  ChokeServo.write(ChokeServoPos);    // sets the servo position to OPEN
  delay(1000);                        // waits for it to get to the position
  ChokeServo.detach();                // cuts off power to the servo
  
  CHOKE_STAT = 1;
  delay (1000);

  cranck_reset_check = 1;
  EEPROM.put(10, cranck_reset_check);  // Store cranck_reset_check in EEPROM
  Log("cranck_reset_check = " + String(cranck_reset_check));
  delay(100);
  
  digitalWrite(CrankControl, LOW);    // start CRANCKING
  CRANK_CONTROL_STAT = 1;
  
  delay(crankTime);                   // defines CRANKING DURATION
  
  digitalWrite(CrankControl, HIGH);   // stop CRANKING
  CRANK_CONTROL_STAT = 0;
  
  delay (openChokeTime);       // defines the duration the engine will run with an onpen choke
  
  ChokeServoPos = chokeCloseAngle;   // the Choke Servo's CLOSE position angle = 100
  ChokeServo.attach(ChokeServoPin);  // attaches the servo on pin 3 to the servo object
  delay(15);  
  ChokeServo.write(ChokeServoPos); // sets the servo position to CLOSE
  delay(1000); // waits for it to get to the position
  ChokeServo.detach();
  CHOKE_STAT = 0;

  cranck_reset_check = 0;
  EEPROM.put(10, cranck_reset_check);  // Store cranck_reset_check in EEPROM
  Log("cranck_reset_check = " + String(cranck_reset_check));
  
  delay (1000);

  Log("StartGenerator END");
}

/* ------------------------------------------------------------------
                    GENERATOR STATUS CHECK FUNCTION
---------------------------------------------------------------------*/
boolean GeneratorStatusCheck()
{
	int meascycles; // number of IGN VOLTAGE measureents (3)
	int counter = 0;
	boolean value;	
  
  Log("GeneratorStatusCheck BEGIN");
	
	for(meascycles = 0 ; meascycles < 300; meascycles++)
	{
		value = digitalRead(VoltageMeasurement);
    
		if(value == 0)
		{
		  counter++;
		}
		delay(10);
	}

	if(counter >= 150)
	{
        IGN_STAT = 1;
        Log("GENERATOR IS WORKING!");
        return 1;
        
	}
 
  IGN_STAT = 0;
  Log("GENERATOR IS IDLE!");
  return 0;
}

/* ------------------------------------------------------------------
                        GENERATOR STOP FUNCTION
---------------------------------------------------------------------*/
#define STOP_GENERATOR_TIMEOUT  (10*1000) //10 sec
void StopGenerator()
{
  boolean gen_running;
  unsigned int time_elapsed=0;
  Log("StopGenerator BEGIN");

  delay (100); // wait 100ms for the system to stabilize
  do
  {
    digitalWrite(FuelValveRelay, HIGH);    // Turns OFF the fuel line
    FUEL_VALVE_STAT = 0;
    digitalWrite(EngineCutOffControl, LOW);  // turns the engine OFF
    ENGINE_CUT_OFF_STAT = 1;
    gen_running = GeneratorStatusCheck(); //this function takes 3000ms
    time_elapsed += 3000;
  }while(gen_running && (time_elapsed < STOP_GENERATOR_TIMEOUT));

  digitalWrite(EngineCutOffControl, HIGH);  // returns the relay to its default position
  ENGINE_CUT_OFF_STAT = 0;
  IGN_STAT = 0;
  Log("GeneratorRunning = " + String(gen_running) + ". ElapsedTime = " + String(time_elapsed));
  Log("StopGenerator END");
}
