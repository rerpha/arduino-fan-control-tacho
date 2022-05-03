#include <EEPROM.h>

#define FAN_PIN 3
#define FS_ADDR 0x01
int fanSpeed;
const byte interruptPin = 2;
volatile int pulseCount=0;
int rpm=0;
unsigned long lastmillis = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
      pinMode(interruptPin, INPUT_PULLUP);


  EEPROM.get(FS_ADDR, fanSpeed);
  if(fanSpeed < 1) fanSpeed = 255;
  analogWrite(FAN_PIN, fanSpeed);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, HIGH);
  Serial.begin(9600);

}
char rx_byte = 0;
String input = "";

void loop() {

 if (millis() - lastmillis == 1000) {
  detachInterrupt(digitalPinToInterrupt(interruptPin));
   rpm = pulseCount/2 * 60;  /* Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use rpmcount * 30.*/
 
 Serial.print("RPM =\t"); //print the word "RPM" and tab.
 Serial.print(rpm); // print the rpm value.
 Serial.print("\t Hz=\t"); //print the word "Hz".
 Serial.println(pulseCount/2); /*print revolutions per second or Hz. And print new line or enter.*/
 
 pulseCount = 0; // Restart the RPM counter
 lastmillis = millis(); // Uptade lasmillis
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
 //enable interrupt
  
 }
  
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
  
    // check if a number was received
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      input.concat(rx_byte);
      
    }
    else if (rx_byte == '\n') {
      Serial.print("Received: ");
      Serial.println(input);
      if(input.toInt() < 256) {
        fanSpeed = input.toInt();
        EEPROM.put(FS_ADDR, fanSpeed);
      } else {
        Serial.println("Invalid Number");
      }
      input = "";
    }
    else {
      Serial.println("Not a number.");
    }
  } // end: if (Serial.available() > 0)
  analogWrite(FAN_PIN, fanSpeed);

}

void blink() {
  pulseCount++;
}
