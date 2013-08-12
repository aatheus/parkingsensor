#include <TinyWireS.h>

const int led = 1;
const int trigger=3;
const int echo = 4;

uint8_t ID = 2;
int count = 0;
unsigned int distance = 30003;

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

void sendInt(unsigned int l) {
   unsigned char byteArray[3];
 
    // convert from an unsigned int to a 2-byte array. Int in Uno is 16 bits
    //We are big-endian because Jeff wanted to.
    byteArray[0] = (unsigned int)((l >> 8) & 0xFF) ; //High order bits
    byteArray[1] = (unsigned int)(l & 0xFF) ; //Low order bits
//    byteArray[2] = (int)((l >> 8) & 0XFF);
//    byteArray[3] = (int)((l & 0XFF));
 
 for (int i = 0; i < 2; i++) {
   //byte b = byteArray[i];
   TinyWireS.send(byteArray[i]);
 }
}

void sendUptime() {
 unsigned int uptime = 32765;
 sendInt(uptime);
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
  sendInt(distance);
}


void sendPong() {
 unsigned int pong = 42424;
 sendInt(pong); 
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
 
 //Respond to bus scan request
 else if (cmd == 4) {
   sendPong();
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
