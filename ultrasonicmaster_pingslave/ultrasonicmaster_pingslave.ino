#include <Wire.h>

const int led = 13;
uint8_t ID = 2;

void blink() {
 digitalWrite(led,HIGH);
 delay(250);
 digitalWrite(led,LOW);
 delay(250); 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  Serial.println("Up!");
  Wire.begin();
  delay(2000); //Slave startup time
}

void getSlaveUptime(int ID) {
  
  Serial.print("Pinging slave ");
  Serial.print(ID);
  Serial.println(" to get uptime...");
  
  Wire.beginTransmission(ID);
  //Wire.write(buf, uptimeStr.length() + 1);
  Wire.write(1);
  Wire.endTransmission();
  
  //Wait for data from slave
  Wire.requestFrom(ID,4);    // request 8 bytes from slave device
  delay(100);
  
  //char buf[Wire.available() + 1];
  uint8_t ptr = 0;
  unsigned long result;
  unsigned char buf[4];
  
  while (Wire.available()) {
     unsigned char c = Wire.read();
     Serial.print(c);
     buf[ptr] = c;
     ptr++;
     //result.concat(c);
  }
  //result = atol(buf);

  result =         (unsigned long)( (buf[0] << 24) 
                   + (unsigned long)(buf[1] << 16) 
                   + (unsigned long)(buf[2] << 8) 
                   + (unsigned long)(buf[3] ) );



  Serial.print("Got last char. Got " );
  Serial.print(ptr);
  Serial.print(" chars. Result is ");
  Serial.print(result);
  Serial.println("");
  
}

void getSlaveDistance() {
  
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  //String uptimeStr = String(millis(), DEC);
  //byte buf[uptimeStr.length() + 1];
 
  //uptimeStr.getBytes(buf, uptimeStr.length() + 1);
  
  getSlaveUptime(ID);
  blink();
  delay(1000);
  
}
