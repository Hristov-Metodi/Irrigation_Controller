/*enum{
  StateMachinePrefix_State0Name=0,
  StateMachinePrefix_State1Name,
};

unsigned char SwitchStateName = IDLE_STATE; // Waiting state

void StateMachineInitializationFunction(void)
{
  SwitchStateName = FIRST_STATE;
}

void StateMachineFunction()
{
  unsigned char prevState=0xFF;
  static unsigned long timeOut = 0;

  if((timeOut ==0) || (timeOut < millis()))
  {
    switch(SwitchStateName)
    {
      case IDLE_STATE:
      break;
    
      case StateMachinePrefix_State0Name:
      //do...
      timeOut = millis() + 3000;          // wait for the fuel to reach the carborator
      SwitchStateName = StateMachinePrefix_State1Name;
      break;

      case END_OF_STATEMACHINE_STATE:
      break;
      
      default:    
      break;
    }
  
    if(prevState != SwitchStateName)
    {
    Log("STATEMACHINE_NAME current state is: " + String(stateGenStarting));
    prevState = SwitchStateName;
    }
  }
}

*/
