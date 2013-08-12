#include <TinyWireS.h>

const int led = 3;
const int trigger=3;
const int echo = 4;

uint8_t ID = 2;
int count = 0;
unsigned long distance = 42;

void blink() {
 digitalWrite(led,HIGH);
 delay(250);
 digitalWrite(led,LOW);
 delay(250); 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(echo,INPUT);
  
  for (int i=0; i < 3; i++) {
   blink(); 
  }
  
  TinyWireS.begin(ID);
  TinyWireS.onReceive(parseRequest);
}

void sendLong(unsigned long l) {
   unsigned char byteArray[3];
 
    // convert from an unsigned long int to a 4-byte array
    byteArray[0] = (int)((l >> 24) & 0xFF) ;
    byteArray[1] = (int)((l >> 16) & 0xFF) ;
    byteArray[2] = (int)((l >> 8) & 0XFF);
    byteArray[3] = (int)((l & 0XFF));
 
 for (int i = 0; i < 4; i++) {
   //byte b = byteArray[i];
   TinyWireS.send(byteArray[i]);
 }
}

void sendUptime() {
 unsigned long uptime = 32765;
 sendLong(uptime);

}

void getDistance() {
 //Force trigger pin low
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
 
 //Read distance
 distance = (pulseIn(echo,HIGH) / 2) / 29.1;
 
}

void sendDistance() {
  sendLong(distance);
}


void parseRequest(uint8_t howMany) { 
  
 int cmd = TinyWireS.receive();
 blink();
 
 //Send uptime
 if (cmd == 1) {
  sendUptime(); 
 }
 
 //Get distance
 else if (cmd == 2) {
   getDistance();
 }
 
 else if (cmd == 3) {
   sendDistance();
 }
 
 //Invalid command!
 else {
   for (int i = 1; i <= 5; i++) {
    blink();
    delay(50); 
   }
 }

}

void loop() {

// blink();
 delay(100); 
}
