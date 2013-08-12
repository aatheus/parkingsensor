#include <Wire.h>

const int led = 13;
//uint8_t ID = 2;

void blink() {
 digitalWrite(led,HIGH);
 delay(250);
 digitalWrite(led,LOW);
 delay(250); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Up!");
  Wire.begin();
  delay(2000); //Slave startup time
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  String uptimeStr = String(millis(), DEC);
  byte buf[uptimeStr.length() + 1];
 
  uptimeStr.getBytes(buf, uptimeStr.length() + 1);
  
  Wire.beginTransmission(ID);
  Wire.write(buf, uptimeStr.length() + 1);
  Wire.endTransmission();
  
  Serial.print("Sent ");
  Serial.println(uptimeStr);
  
  blink();
  delay(1000);
  
}
