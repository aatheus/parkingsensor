#include <Wire.h>

const bool debug = false;

const int led = 13;
uint8_t ID = 2;

void blink() {
 digitalWrite(led,HIGH);
 delay(250);
 digitalWrite(led,LOW);
 delay(250); 
}

unsigned int receiveInt() {
 unsigned int result;
 
  uint8_t ptr = 0;
  unsigned char buf[2];
  
  while (Wire.available()) {
     unsigned char c = Wire.read();
     
     if (debug) {
       Serial.print(c);
     }
     
     buf[ptr] = c;
     ptr++;
     //result.concat(c);
  }
  int x = Wire.read();
  //result = atol(buf);

  result =         (unsigned int)( (buf[0] << 8)) 
                   + (unsigned int)(buf[1]); 
                  // + (unsigned long)(buf[2] << 8) 
                  // + (unsigned long)(buf[3] ) );



if(debug) {
  Serial.print("\nGot last char. Got " );
  Serial.print(ptr);
  Serial.print(" chars. Result is ");
  Serial.print(result);
  Serial.println("\n");
}

 return result; 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  Serial.println("Up!");
  Wire.begin();
  delay(1000); //Slave startup time
  
  //Scan the bus to get all slaves
  scanBus();
  delay(2000);
}

//We need to find out on startup how many slaves are alive. Ping them and store
//the ones that respond in an array. We will truncate it and create a smaller
//array afterward
void scanBus() {
  uint8_t buf[256];
  uint8_t ptr = -1;
  
  Serial.println("Scanning bus for slaves...");
  
  unsigned long startTime = millis();
  
  for (int ID=0; ID < 128; ID++) {
   Wire.beginTransmission(ID);
   Wire.write(4);
   Wire.endTransmission(); 
   
   //Wait for data from slave
   Wire.requestFrom(ID,2);    // request 8 bytes from slave device
   delay(100);
   unsigned int response = receiveInt();
   
   
   if(response == 42424) {
    ptr++;
    buf[ptr] = ID;  
    
    /*Serial.print("Slave ");
    Serial.print(ID);
    Serial.println("is alive!\n");*/
    Serial.print("*");
    Serial.print(ID);
    Serial.print("*\n");
    }
   
   else {
    /*Serial.print("\nSlave ");
    Serial.print(ID);
    Serial.print(" did not respond! Result was ");
    Serial.print(response); */
    if(debug) {
      Serial.print(ID);
      Serial.print("\n");
    }
   }
     
   delay(100);
   }
   
   unsigned long endTime = millis();
   
   String str = "Done! Found ";
   str.concat((ptr + 1));
   str.concat(" slave(s) (took ");
   str.concat(endTime - startTime);
   str.concat(" ms)");
   Serial.println(str);
  
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
  Wire.requestFrom(ID,2);    // request 8 bytes from slave device
  delay(30);
  unsigned int uptime = receiveInt();
  Serial.print("Slave ");
  Serial.print(ID);
  Serial.print(" uptime is ");
  Serial.println(uptime); 
  
}

void getSlaveDistance(int ID) {
  Serial.print("Pinging slave ");
  Serial.print(ID);
  Serial.println(" to get distance...");
  
   Wire.beginTransmission(ID);
  //Wire.write(buf, uptimeStr.length() + 1);
  Wire.write(3);
  Wire.endTransmission();
  
  //Wait for data from slave
  Wire.requestFrom(ID,2);    // request 8 bytes from slave device
  delay(100);
  unsigned int distance = receiveInt();
  Serial.print("Slave ");
  Serial.print(ID);
  Serial.print(" distance is ");
  Serial.println(distance); 
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  //String uptimeStr = String(millis(), DEC);
  //byte buf[uptimeStr.length() + 1];
 
  //uptimeStr.getBytes(buf, uptimeStr.length() + 1);
  
  getSlaveUptime(ID);
  delay(100);
  getSlaveDistance(ID);
  delay(100);
  
  blink();
  delay(1000);
  
}
