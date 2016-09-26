

#include <SoftwareSerial.h>

SoftwareSerial gsm(9,10);

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial.println("Starting,...");
  gsm.begin(9600);
  // reserve 200 bytes for the inputString:
//  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  delay(50);
  
  if(Serial.available())
  {
    while(Serial.available())
    {
      gsm.write(Serial.read());
    }
  }
  
  if(gsm.available())
  {
    while(gsm.available())
    {
      Serial.write(gsm.read());
    }
  }
  

}


