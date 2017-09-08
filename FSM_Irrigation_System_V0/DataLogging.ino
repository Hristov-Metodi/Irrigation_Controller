/* ------------------------------------------------------------------
                     DATA LOGGING FUNCTIONS
---------------------------------------------------------------------*/
#define SD_CARD_SELECT  (4)
static File LogFile;

int LogInit(String filename, int serial_baudrate)
{
    boolean res;
    Serial.begin(serial_baudrate);                //initiates the serial communication
    
    res = SD.begin(SD_CARD_SELECT);
    if(res != 0)
    {
        Serial.println("\n\nSDcard initialized @: " + String(millis()));
        LogFile = SD.open(filename, FILE_WRITE);
        if(LogFile)
        {
            Serial.println("LOGfile opened");
            return 0;//success
        }
        else
        {
            Serial.println("\nERROR: Failed to open LOGfile\n");
            return -1;
        }
    }
    else
    {
         Serial.println("\nERROR: Failed to init SDcard\n");
        return -2;//failed
    }
    
    return -3;//failed
}

void Log(String dataString)
{
  Serial.println(dataString);

  if(LogFile)
  {
    LogFile.println(dataString);
    LogFile.flush(); //flush data to the SD card
  } 
}

void LogClose(void)
{
    if(LogFile)
    {
        LogFile.close();
    }
}
