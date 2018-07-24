#include <EEPROM.h>
#define HWSERIAL Serial1      //Define Serial Port1 (RX = Pin 0, TX = Pin 1)
int transInt=0;           //Define transmission character
int oldTransInt = 0;      //Define copy variable
void setup() {                
  pinMode(17,OUTPUT);         //Pin to Strata
  digitalWrite(17, HIGH);
  pinMode(16,OUTPUT);         //Pin to Marsa
  digitalWrite(16, HIGH);
  pinMode(11,OUTPUT);         //LED Varification on Board
  digitalWrite(11, LOW);
  HWSERIAL.begin(9600);       //Set up hardware serial connection
  transInt=EEPROM.read(0);   //In case of microcontroller restart, take previous value for transChar
}

void loop() {

  // Checks and reads available seral data
  if(HWSERIAL.available() > 0){ // Checks whether data is comming from the serial port
    oldTransInt = transInt;   // Save past value of transChar
    transInt = HWSERIAL.read(); // Reads the data from the serial port
  }

  //  Recieving power on Strata transmission
  if (transInt == 1) {
    digitalWrite(17, HIGH);     //Power Strata on
    digitalWrite(16, LOW);      //Power Marsa off
    digitalWrite(11,HIGH);      //Turn on test LED
  }

  // Recieving power on Marsa transmission
  else if(transInt == 2) {
    digitalWrite(17, LOW);      //Power Strata off
    digitalWrite(16, HIGH);     //Power Marsa on
    digitalWrite(11,HIGH);      //Turn on test LED
  }

  // Recieving power on Marsa and Strata transmission
  else if(transInt == 3) {
    digitalWrite(17, HIGH);     //Power Strata on
    digitalWrite(16, HIGH);     //Power Marsa on
    digitalWrite(11,HIGH);      //Turn on test LED
  }

  else if(transInt == 0){
    digitalWrite(17, LOW);      //Power Strata off
    digitalWrite(16, LOW);      //Power Marsa off
    digitalWrite(11, LOW);      //Turn off test LED
  }

  // If new transmission value, save to non-volitile memory.
  if(oldTransInt != transInt){
    EEPROM.write(0, transInt);
  }
}
