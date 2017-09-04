boolean IsTankFull(void)
{
    boolean tankIsNotFull;
    digitalWrite(FloatSensorPowerSupply, HIGH);  // Turns ON the measurement circuit power supply
    delay(100);
    tankIsNotFull = (digitalRead(TopFloatSensor_1) && digitalRead(TopFloatSensor_2));
    digitalWrite(FloatSensorPowerSupply, LOW);  // Turns OFF the measurement circuit power supply
    return !tankIsNotFull;
}


boolean IsTankEmpty(void)
{
    boolean tankIsNotEmpty;
    digitalWrite(FloatSensorPowerSupply, HIGH);  // Turns ON the measurement circuit power supply
    delay(100);
    tankIsNotEmpty = digitalRead(BottomFloatSensor_1) && digitalRead(BottomFloatSensor_2);
    digitalWrite(FloatSensorPowerSupply, LOW);  // Turns OFF the measurement circuit power 
    return !tankIsNotEmpty;
}
