
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

String textMessage;

String lampState = "HIGH";

const int relay = 13;

void setup()
{
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  
  Serial.begin(19200); 
  SIM900.begin(19200);

  delay(2000);
  Serial.print("SIM900 ready...");

  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");// Set module to send SMS data to serial out upon receipt 
  delay(100);
}

void loop(){
  if(SIM900.available()>0)
  {
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0)
  
  {
    // Turn on relay and save current state
    digitalWrite(relay, LOW);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0)
  {
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  
  if(textMessage.indexOf("STATE")>=0)
  {
    int a= analogRead(A0);
    String fuel=String(a);
    
    String message = "Lamp is " + lampState+", Fuel level is "+ fuel+" %";
    sendSMS(message);

    
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  


void sendSMS(String message)

{
 
  SIM900.print("AT+CMGF=1\r"); 
  delay(500);
  SIM900.println("AT + CMGS = \"9988218772\"");  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  delay(500);
  SIM900.println(message); 
  delay(500);
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
 
  delay(5000);  
}
