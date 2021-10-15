// Include the Arduino Stepper Library
#include <Stepper.h>
#include <LiquidCrystal.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


// Number of steps per output rotation
const int stepsPerRevolution = 200;
// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


int pirPin = 4;         //pir motion detection               
int pirStat = 0;        //init pir status zero
int busy = 13;          //df player busy pin
int Lswitch = 12;       //limit switch        
static const uint8_t PIN_MP3_RX = 2; // Connects to module's RX 
static const uint8_t PIN_MP3_TX = 3; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;



void setup()
{
   pinMode(pirPin, INPUT);  //pir
   pinMode(busy, INPUT);    //busy_status
   pinMode(Lswitch, INPUT); //limit switch
   pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
   pinMode(11, OUTPUT);
   Serial.begin(9600);
   
   softwareSerial.begin(9600);

   if (player.begin(softwareSerial)) 
   {
   Serial.println("PLAYER_OK_Connection established");
   player.volume(30);
   }else {
    Serial.println("Connecting to DFPlayer Mini failed!");
   } 
  
  // set the speed at 60 rpm:
  myStepper.setSpeed(40);
  
  // stepper setup
  digitalWrite(Lswitch, HIGH); 
  while (digitalRead(Lswitch) == HIGH) 
  {
    myStepper.step(-5);
    Serial.println("on way to orgin_point"); 
    }
   stopstep();
   Serial.println("orgin set");
   myStepper.setSpeed(60);

}

void stopstep()
{
  Serial.println("stop movement");
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}




void loop() 
{
  
pirStat = digitalRead(pirPin);
 if (pirStat == LOW)   //detect motion (version_change)
 {
    Serial.println("motion_detected");
    
    player.play(1); //playing song
    myStepper.setSpeed(60);
    myStepper.step(60)
    
  
     while(digitalRead(busy)==0)
      { 
         
        Serial.println("playing_ongoing");
        myStepper.setSpeed(60);
        myStepper.step(60);
        myStepper.setSpeed(30);
        myStepper.step(-60);
        
         
         
      }
      myStepper.step(-60)
 }
pirStat = HIGH; //(version_change)
Serial.println("next iteration");
}
