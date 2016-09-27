#include <SoftwareSerial.h>

SoftwareSerial gsm(9,10);

//////////////////////////////////////
const int TIME_OUT = 5;
boolean configured = false, dataActivated = false;

////////////////////////////////////
void setup()
{
   Serial.begin(9600);
  Serial.println("Starting,...");
  gsm.begin(9600);
  
}

void loop()
{
  gsmWrite("AT");
  delay(500);
  Serial.println(gsmRead());
 
  
}


void gsmWrite(char input[])
{
  //Writes input to the gsm device
  
  if(!gsm.available())
  {
    gsm.write(input);
    gsm.write("\r\n");
    delay(10);
   // Serial.print("Writing to GSM:");
   // Serial.println(input);
  }
}

void waitForResponse()
{
  //Waits for gsm device to respond with data.
  //Has a time out value = 1000 milliseconds x TIME_OUT (const)
  if(!gsm.available())
  {
    int count = 0;
    while(!gsm.available())
    {
      delay(999); count++;
      if(count>=TIME_OUT)
        break;
    }
  }
}

String gsmRead()
{
  //Reads and returns data from gsm device if data is available
  waitForResponse();
  String data;
  if(gsm.available())
  {
    while(gsm.available())
    {
      data = gsm.readString();
      delay(10); 
    }
  }
  return data;
}

String gsmReadOut()
{ //Reads and returns data from gsm device if data is available
  //Also prints out data to Serial Monitor
  String data = gsmRead();
  Serial.println(data);
  return data;
}

boolean errorCheck(String readOutput)
{
  //Checks for word "ERROR" in readOutput. if there is error, retries last command
    // for TIME_OUT times. if there is error after total retry times, exits with error message; 
   int count;
    boolean errorCheck = readOutput.find("ERROR");
  if(!errorCheck)
  {
    return false; //No error
  }

  else if(errorCheck)
  {
    if(count>TIMEOUT)
    {
      Serial.print("Error Occured ");
      Serial.print(TIME_OUT);
      Serial.println(" times. Stopping Retries.");
      return true; //Error

    }
    Serial.println("Error Occured.Retrying Last Command...");
    gsmWrite("A/");
    count ++;
    errorCheck();
  }
}

void gsmConfig()
{
  //Configures APN of gsm Sim
  gsmWrite("AT\r\n");
  errorCheck(gsmReadOut());
  
  gsmWrite("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
  errorCheck(gsmReadOut());
  
  gsmWrite("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\r\n");
  if(!errorCheck(gsmReadOut()));
  {
    configured = true;
  }
  
  gsmWrite("AT+SAPBR=1,1\r\n");
  errorCheck(gsmReadOut());
  
  gsmWrite("AT+SAPBR=2,1\r\n");
  errorCheck(gsmReadOut());
  
  gsmWrite("AT+SAPBR=0,1\r\n");
  errorCheck(gsmReadOut());
  
}

void gsmDataActivate()
{
  //Activates Data Connection

  if(dataActivated==true) //data already on
    return;

  if(configured==false) //gsm sim not configured
  {
    Serial.println("GSM not configured.\n Configuring...")
    gsmConfig();
  }

  gsmWrite("AT+SAPBR=1,1");
  boolean check = errorCheck(gsmReadOut());
  if(!check)
    dataActivated = true;
  else
    dataActivated = false;  
}

void gsmDataDeactivate()
{
  //Deactivates data connection

  if(dataActivated==false) //data already off
    return;

 if(configured==false) //gsm sim not configured  {
    Serial.println("GSM not configured.\n Configuring...")
    gsmConfig();
  }

  gsmWrite("AT+SAPBR=0,1");
  boolean check = errorCheck(gsmReadOut());
  if(check)
    dataActivated = true;
  else
    dataActivated = false;
}

String gsmTriangulate()
{
  gsmWrite("AT+CIPGSMLOC=1,1\r\n");
  String data = gsmReadOut();
 if( errorCheck(data)) //if there is error
 {
   data = -1;
   return data;
 }
 
 else
  return data;
}





//checkout
//1.gsm read to read into, must return value
//2. Serial.write
