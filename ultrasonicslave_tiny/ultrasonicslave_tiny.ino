#include <TinyWireS.h>
#include <EEPROM.h>

const int led = 3;
const int trigger=3;
const int echo = 4;
const int button = 1;

uint8_t ID;
unsigned int distance = 30003;

void getID() {
 ID = EEPROM.read(0);
 if ((ID < 1) || (ID > 127)) {
  ID = 1;
  saveID(ID);
 } 
}

void saveID(uint8_t ID) {
   EEPROM.write(0,ID); 
}

void blink(uint8_t d) {
 digitalWrite(led,HIGH);
 delay(d);
 digitalWrite(led,LOW);
 delay(d); 
}

void blinkID() {
  digitalWrite(led,LOW);
  delay(250);
  
 for (int i = 1; i <= ID; i++) {
  blink(200);
 } 
}

void checkForProgrammingMode() {
 //Check input pin for button to see if we need to go into programming mode
 //button will normally be low
 long duration = pulseIn(button,HIGH,2000000);
 delay(3000);

 //We need a good solid press of at least 2000 uS but less than 2 seconds.
 if (duration > 0) {
  enterProgrammingMode();
 } 
 
 //Button was not pressed. Continue normally
 else {}
}

void enterProgrammingMode() {

  //Init programming mode. Set ID to 0
  bool endProgramming = false;
  ID = 0;

 //Loop until we get another long press
 while (endProgramming == false) {
  //Set LED high
  digitalWrite(led,HIGH);
  
  //Attempt to read the button
  long duration = pulseIn(button,HIGH,5000000);
  //delay(500);
  
  if (duration >= 1000000) {
   endProgramming = true; 

  }
  else if ((duration >= 2000) && (duration < 1000000)) {
    ID++;
    saveID(ID);
    blinkID();
    delay(500);
  }
 } 
 
 //Done programming. Exit and continue
 saveID(ID);
 blinkID();
 
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
  digitalWrite(trigger,LOW);

 
 //Read distance
 long duration = pulseIn(echo,HIGH,1000000);
 distance = (duration / 2) / 29.1;
 
 //Blink once
 blink(50);
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
   //for (int i = 1; i <= 5; i++) {
    //blink(100);
    //delay(50); 
   //}
 }
 
  //blink(100);

}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(button,INPUT);
  
  //LED high for 1 second
  digitalWrite(led,HIGH);
  delay(500);
  digitalWrite(led,LOW);
  delay(500);
  
  //Set ID from EEPROM, or override to be 1 if no ID set
  getID();
  
  //Blink ID, then wait 2 seconds for input
  blinkID();
  checkForProgrammingMode();
  
  
  TinyWireS.begin(ID);
  TinyWireS.onReceive(parseRequest);
}

void loop() {

 //blink(100);
 delay(100); 
}
