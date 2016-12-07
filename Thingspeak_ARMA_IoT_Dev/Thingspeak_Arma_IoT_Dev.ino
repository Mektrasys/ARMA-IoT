/* this code is compatible with all esp12f connected to Arduino Uno through hardware serial
 *  This code requires the following libraries 1. ESP8266_Lib.h and 2. Sensor Related Librarie
 *  This code doesn't require Thingspeak Library
 *  This code is open source and was developed for ARMA IoT
 *  Developed by Mektrasys, Banglore India
 *  find ESP8266_lib.h on github
 *  www.mektrasys.com       www.github.com/Mektrasys/ARMA-IoT
 */

#include <ESP8266_Lib.h>

#define DEBUG true
#define SSID "Wi-Fi ID or SSID"  // your ssid   space and case matters
#define PASS "Wi-Fi password"    //pass word

//ThingSpeak details
unsigned long myChannelNumber = 161323;    //find it on thingspeak
const char * apiKey = "KBHJ89N5VTV40LFX";  //find it on thingspeak

//Thingspeak Field
float field1=0,field2=0,field3=0,field4=0,field5=0,field6=0,field7=0,field8=0;


void setup()
{
  
  Serial.begin(115200); // ESP baud rate
 
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point 
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
 
       //connect to the wifi
       boolean connected=false;
       for(int i=0;i<5;i++)
       {
           if(connectWiFi())
           {
               connected = true;
                   break;
             }
         }
         if (!connected){while(1);}
         delay(2000);
         
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  
}


void loop()
{
  
  data();
  thingspeak();
  
}


void data()                            //Assign all the 8 datas gathered to respective fields as assigned on thingspeak website 
{

/* Read Me
 *  This is just an example, field1 is assigned to temperature, 
 *  field2 to humidity 
 *  and the rest are left empty 
 *  on the site */

int temperature = analogRead(A2); // Reads the value at Analog 2 port
float humidity = analogRead(A3);   //Reads the value at Analog 3 port
humidity = humidity/100;


// Assign all the data to respective field in the end
// keep the default value of field zero if not in use

field1=temperature;
field2=humidity;
field3=0;
field4=0;
field5=0;
field6=0;
field7=0;
field8=0;

}


void thingspeak()
{
 // convert to string
  char buf[32];
 
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  Serial.println(cmd);
   
  if(Serial.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(field1);
  getStr +="&field2=";
  getStr += String(field2);
  getStr +="&field3=";
  getStr += String(field3);
  getStr +="&field4=";
  getStr += String(field4);
  getStr +="&field5=";
  getStr += String(field5);
  getStr +="&field6=";
  getStr += String(field6);
  getStr +="&field7=";
  getStr += String(field7);
  getStr +="&field8=";
  getStr += String(field8);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);

  if(Serial.find(">")){
    Serial.print(getStr);
  }
  else{
    Serial.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}



String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    Serial.print(command); // send the read character to the Serial
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}



boolean connectWiFi()
{
       String cmd="AT+CWJAP=\"";
       cmd+=SSID;
       cmd+="\",\"";
       cmd+=PASS;
       cmd+="\"";
       Serial.println(cmd);
       Serial.println(cmd);
       delay(3000);
       if(Serial.find("OK"))
     {
          Serial.println("OK, Connected to WiFi.");
     return true;
     }else
     {
           Serial.println("Can not connect to the WiFi.");
           return false;
     }
}
