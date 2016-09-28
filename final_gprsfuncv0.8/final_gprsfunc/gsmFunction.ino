void gsmWrite(char input[])
{
  //Writes input to the gsm device
  int count = 0;
  while(gsm.available())
  { 
    delay(500);
    count++;
    if(count>=TIME_OUT)
      break;
  }

  gsm.write(input);
  gsm.write("\r\n");
  gsm.flush();
  Serial.print("Writing to GSM:");
  Serial.println(input);
  delay(2);
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
      delay(1000);
      count++;
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
  int errorCheckValue = readOutput.indexOf("ERROR");

  if(errorCheckValue==-1)
  {
    return false; //No error
  }

  else //error occured
  {
    for(int i=0; i<TIME_OUT; i++)
    {
      Serial.println("Error Occured.Retrying Last Command...");
      gsmWrite("A/");
      readOutput = gsmReadOut();
      errorCheckValue = readOutput.indexOf("ERROR");
      if(errorCheckValue==-1)
      {
        return false; //No error
      }
    }

    Serial.print("Can't fix error. Retried ");
    Serial.print(TIME_OUT);
    Serial.println(" times. Stopping Retries. Exiting\n***********************************.");
    return true;
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
  
 // Serial.print("gsm activate");
  //gsmWrite("AT+SAPBR=1,1\r\n");
//  errorCheck(gsmReadOut());
  gsmDataActivate();
  
  delay(1000);
  gsmWrite("AT+SAPBR=2,1\r\n");
  errorCheck(gsmReadOut());

//  gsmWrite("AT+SAPBR=0,1\r\n");
//  errorCheck(gsmReadOut());
  gsmDataDeactivate();
}

void gsmDataActivate()
{
  //Activates Data Connection

  if(dataActivated==true) //data already on
    return;

  if(configured==false) //gsm sim not configured
  {
    Serial.println("GSM not configured.\n Configuring...");
    gsmConfig();
  }

  gsmWrite("AT+SAPBR=1,1");
  boolean check = errorCheck(gsmReadOut());
  gsmWrite("AT+SAPBR=2,1\r\n");
  errorCheck(gsmReadOut());
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

  if(configured==false) //gsm sim not configured
  {
    Serial.println("GSM not configured.\n Configuring...");
    gsmConfig();
  }

  gsmWrite("AT+SAPBR=0,1");
  boolean check = errorCheck(gsmReadOut());
  if(!check)
    dataActivated = false;
  else
    dataActivated = true;
}

String gsmTriangulate()
{
  if(dataActivated==false)
      gsmDataActivate();
    
  gsmWrite("AT+CIPGSMLOC=1,1\r\n");
  String data = gsmReadOut();
  if( errorCheck(data)) //if there is error
  {
    return data;
  }

  else
    return data;
}

String gsmLocationLongitude(String locationData)
{
  int firstCommaIndex = locationData.indexOf(',');
  int secondCommaIndex = locationData.indexOf(',', firstCommaIndex + 1);
  int thirdCommaIndex = locationData.indexOf(',',secondCommaIndex + 1);
  
  if(firstCommaIndex == -1)
  {
     Serial.println("Error. Location not found. Returning...");  //triangulation failed
     return;
  }

  String longitude;

  //Longitude
   longitude = locationData.substring(firstCommaIndex+1, 
                        secondCommaIndex);
  
  return longitude;

}


String gsmLocationLatitude(String locationData)
{
  int firstCommaIndex = locationData.indexOf(',');
  int secondCommaIndex = locationData.indexOf(',', firstCommaIndex + 1);
  int thirdCommaIndex = locationData.indexOf(',',secondCommaIndex + 1);
  
  if(firstCommaIndex == -1)
  {
     Serial.println("Error. Location not found. Returning...");    //triangulation failed
     return;
  }

  String latitude;

  latitude = locationData.substring(secondCommaIndex+1, 
                        thirdCommaIndex);
  
  return latitude;

}
