#define HWSERIAL Serial1    //Create Hardware Serial Port
int armPinL =19;            //Define Pin 19 to read left swtich value
int armPinR = 18;           // Define Pin 18 to read right switch value
int blconnect = 11;         // Define pin 11 for bluetooth connection state

int LED0 = 3;               // Define pwm pins for block O LED Pattern
int LED1 = 4;
int LED2 = 6;
int LED3 = 9;
int LED4 = 10;
int LED5 = 16;
int LED6 = 17;

double i =855;                 // Initialize loop counter for lights

int armLeft = 0;                //Define pins to hold read states
int armRight = 0;

bool recentlybreathing=false;   // Define pin for keeping recent sequence data
int pi = 3.14159265;            // Define Pi constant
bool blconnectState = false;    // Define connection state

//This function sets up or refreshes the LED display
void LEDSetup(){
  pinMode(LED0,OUTPUT);     
  analogWrite(LED0, 0);
  pinMode(LED1, OUTPUT);
  analogWrite(LED1,0);
  pinMode(LED2, OUTPUT);
  analogWrite(LED2,0);
  pinMode(LED3, OUTPUT);
  analogWrite(LED3,0);
  pinMode(LED4, OUTPUT);
  analogWrite(LED4,0);
  pinMode(LED5, OUTPUT);
  analogWrite(LED5,0);
  pinMode(LED6, OUTPUT);
  analogWrite(LED6,0);
}

void setup() {
  pinMode(armPinL,INPUT);    //Setup arming Pin
  pinMode(armPinR,INPUT);
  pinMode(blconnect, INPUT);  //Set up Bluetooth state pin for input

  LEDSetup();                 // Set up initial state for LEDs
  
  HWSERIAL.setRX(0);        //Setup and start hardware serial communications (RX = Pin 0, TX = Pin 1)
  HWSERIAL.setTX(1);
  HWSERIAL.begin(9600);
  Serial.begin(9600);
}

void loop() {
  //Take readings once for use throughout loop
  armLeft = digitalRead(armPinL);
  armRight = digitalRead(armPinR);
  blconnectState = digitalRead(blconnect);

  if(armLeft){                //Marsa command sent based on left switch state
    HWSERIAL.write(0x46);
  }
  else{
    HWSERIAL.write(0xc5);
  }

  if(armRight){               //Strata command send based on right switch state
    HWSERIAL.write(0x9e);
  }
  else{
    HWSERIAL.write(0x71);
  }
  delay(100);
  
  //If bluetooth is not paired, run through the circular block O logo on LEDs
  if(!blconnectState){

    //If the LED mode has returned from "breathing" pattern, resed LEDs
    if(recentlybreathing)
    {
      LEDSetup();
      recentlybreathing = false;
    }

    //set up i value for use in the block O loop
    if(i<=0){
        i=700;                // Reset i
      }
      else{
        i-=25;                  // Decrement i
      }
      
      //Block O Slow fading Lights
      if(i>=600){
        analogWrite(LED0, i-600);
      }
      if(i<=755&&i>=500){
        analogWrite(LED1, i-500);
      }
      if(i<=655&&i>=400){
        analogWrite(LED2, i-400);
      }
      if(i<=555&&i>=300){
        analogWrite(LED3, i-300);
      }
      if(i<=455&&i>=200){
        analogWrite(LED4, i-200);
      }
      if(i<=355&&i>=100){
        analogWrite(LED5, i-100);
      }
      if(i<=255&&i>=0){
        analogWrite(LED6, i);
      }
      if(i<155){
        analogWrite(LED0, i+100);
      }
      if(i<55){
        analogWrite(LED1, i+200);
      }
      
  }

  //if bluetooth is connected, start flipflop cycle or sleep cycle
  else if(blconnectState){

    //run block O flipflop if both switches are off
    if(armLeft==0&&armRight==0){

      //reset LEDs if we just came from the breathing state
      if(recentlybreathing)
      {
        LEDSetup();
        recentlybreathing = false;
      }

      //LED flipflop pattern
      if(i<=100){
        analogWrite(LED0,0);
        analogWrite(LED1,0);
        analogWrite(LED2,0);
        analogWrite(LED3,255);
        analogWrite(LED4,255);
        analogWrite(LED5,255);
        analogWrite(LED6,255);
        i+=25;
        
      }
      else {
        analogWrite(LED0,255);
        analogWrite(LED1,255);
        analogWrite(LED2,255);
        analogWrite(LED3,0);
        analogWrite(LED4,0);
        analogWrite(LED5,0);
        analogWrite(LED6,0);
        i+=25;
        if(i>200){
          i=0;   
        }
      }
    }

    //run "breathing" pattern on appropriate side of Block O for swtich
    else{
      if(i>pi/4.0){
        i= -pi/4.0;
      }
      else{
        i+=pi/40.0;
      }

      //this section uses mapping with trig to create a proper look with fading lights
      analogWrite(LED0,map(tan(abs(i)*armLeft),0,1,0,255));
      analogWrite(LED1,map(tan(abs(i)*armLeft),0,1,0,255));
      analogWrite(LED2,map(tan(abs(i)*armLeft),0,1,0,255));
      analogWrite(LED3,map(tan(abs(i)*armRight),0,1,0,255));
      analogWrite(LED4,map(tan(abs(i)*armRight),0,1,0,255));
      analogWrite(LED5,map(tan(abs(i)*armRight),0,1,0,255));
      analogWrite(LED6,map(tan(abs(i)*armRight),0,1,0,255));
      recentlybreathing = true;
  
    }
  }
}

