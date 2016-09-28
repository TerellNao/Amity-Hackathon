#include <SoftwareSerial.h>

SoftwareSerial gsm(9,10);

//////////////////////////////////////
const int TIME_OUT = 5;
boolean configured = false, dataActivated = false;


////////////////////////////////////

 struct Location
{
  String longitude;
  String latitude;
};

///////////////////

void setup()
{
   Serial.begin(9600);
   delay(1000);
  Serial.println("Starting,...");
  gsm.begin(9600);
  
  gsmWrite("AT");
  gsmReadOut();
  gsmConfig();
  
 
}

void loop()
{
  
    gsmTriangulate();
    delay( 60000);
    
    if(Serial.available())
    {
      String command = Serial.readString();
      if(command == "triangulate\r\n")
        gsmTriangulate();
    }
  
}





//checkout
//1.gsm read to read into, must return value
//2. Serial.write
