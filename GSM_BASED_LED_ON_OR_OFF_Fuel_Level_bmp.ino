
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

String textMessage;
String Presstemp="";

String lampState = "OFF";

const int relay = 13;

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);    //  Displays some basic information on this sensor from the un

void setup()
{
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(1000);
    digitalWrite(12, HIGH);
  
  Serial.begin(19200); 
  SIM900.begin(19200);

    if(!bmp.begin())
  {    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
   }
   displaySensorDetails();
  delay(2000);
  Serial.print("SIM900 ready...");

  SIM900.print("AT\r"); 
  delay(200);
  
  SIM900.print("AT+CMGF=1\r"); 
  delay(200);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");// Set module to send SMS data to serial out upon receipt 
  delay(200);
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
    delay(1000);
    digitalWrite(relay, HIGH);
    lampState = "on";
    Serial.println("Car Lock set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0)
  {
    // Turn off relay and save current state
    digitalWrite(12, LOW);
    delay(1000);
    digitalWrite(12, HIGH);
    
    lampState = "off"; 
    Serial.println("Car Lock set to OFF");
    textMessage = ""; 
  }
  
  if(textMessage.indexOf("STATE")>=0)
  {
    
      sensors_event_t event;
      bmp.getEvent(&event);
     if (event.pressure)
         { 
            float tyrepress = event.pressure;  //  Serial.println(" hPa");
            
            float temperature;
            bmp.getTemperature(&temperature);

             Presstemp = "Tyre Pressure is ";
             Presstemp += String(tyrepress);
             Presstemp += " hPa , ";
             Presstemp += "Tyre Temperature is ";
             Presstemp += String(temperature);
             Presstemp += " Degree Celcius";
         }
        else
        {
          Serial.println("Sensor error");
        }

        
    int a= analogRead(A1);
    a=map(a,0,1023,0,100);
    String fuel=String(a);
    
    String message = "CAR is " + lampState+", Fuel level is "+ fuel+" %"+ Presstemp;
    sendSMS(message);

    
    Serial.println("Lamp state resquest");
    textMessage = "";
    Presstemp="";
    fuel="";
  }
}  


void sendSMS(String message)

{
 
  SIM900.print("AT+CMGF=1\r"); 
  delay(500);
  SIM900.println("AT + CMGS = \"+917986534673\"");  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  delay(500);
  SIM900.println(message); 
  delay(500);
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
 
  delay(5000);  
}


void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
