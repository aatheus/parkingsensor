#include <Wire.h>

const int trigger = 2;
const int echo = 3;
const int led = 8;
int ID = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Wire.begin(ID);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Up!");

}

/*void receiveEvent(int howMany) {
  Serial.print("Master howMany: ");
  Serial.println(howMany);
  
  long val = 0;
   while (Wire.available() > 1) {
    uint8_t x = Wire.read();
    Serial.print("Raw was ");
    Serial.println(x);
    val & x;
    val << (Wire.available() + 1);
  }
  int x = Wire.read();
//  str += x;
  
   Serial.print("From master: [");
   Serial.print(val);
   Serial.println("]"); 
}
*/

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
 
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
  delay(100);
  
  Serial.print("Master howMany ");
  Serial.println(howMany);
  
  String str = "";
  
  while(Wire.available() > 1) // loop through all
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
    str.concat(c);
  }
  byte x = Wire.read();    // receive byte as an integer
  //Serial.println(x);  
  Serial.print("[");
  Serial.print(str); 
  Serial.println("]");
  
  char buf[str.length() + 1];
  str.toCharArray(buf, str.length() + 1);
  long lint = atol(buf);
  Serial.print("Converted to long: ");
  Serial.println(lint);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //blink(1);
  delay(100);

}
