#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

#define DriveMotor_Stop 0
#define DriveMotor_MovingLeft 1
#define DriveMotor_MovingRight 2
#define DriveMotor_PausedLeft 3
#define DriveMotor_PausedRight 4
#define DriveMotor_Error 5

uint8_t DriveMotor_State = DriveMotor_Stop;

Adafruit_DCMotor *DriveMotor;
Adafruit_DCMotor *ShakeMotor;
Adafruit_MotorShield AFMotorShield;
/*/////////////////////////
//
//
// work on time and states
//
//
*/////////////////////////
//moved back to blank space
byte a = 0;
int counter=0;
byte ended = false;
bool LeftSensor = false;
bool RightSensor = false;
const int LeftSensorPin = 6;
const int RightSensorPin = 5;
//end moved back to blank space

void Error();
void MovingLeft(bool LeftSensor, bool RightSensor);
void MovingRight(bool LeftSensor, bool RightSensor);
void PausedLeft(bool LeftSensor,bool RightSensor);
void PausedRight(bool LeftSensor, bool RightSensor);
void Stop();

void setup() {
    //AFMotorShield = Adafruit_MotorShield();

    DriveMotor = AFMotorShield.getMotor(1);
    ShakeMotor = AFMotorShield.getMotor(2);
    AFMotorShield.begin();
    DriveMotor->setSpeed(100);
    DriveMotor->run(FORWARD);
    ShakeMotor->setSpeed(254);
    ShakeMotor->run(FORWARD);
    Timer1.initialize();
    MFS.initialize(&Timer1);    // initialize multi-function shield library
/*
    MFS.write("Hi");
    delay(2000);
    MFS.write(-273);
    delay(2000);
    MFS.write(3.141, 3);  // display to 2 decimal places.
    delay(2000);
    */
    pinMode(LeftSensorPin, INPUT);
    pinMode(RightSensorPin, INPUT);

}



void loop() {
    a = MFS.getButton();
  LeftSensor = !digitalRead(LeftSensorPin);
  RightSensor = !digitalRead(RightSensorPin);
      // digitalWrite(3, HIGH);
/* from the man
    // put your main code here, to run repeatedly:
    if (counter == 100)
    {
        DriveMotor->setSpeed(255);
    }
    if (counter == 150)

    {

        DriveMotor->run(BACKWARD);
    }
    if (counter < 200)
 {
   MFS.write((int)counter);
   counter++;
 }
 else if (!ended)
 {
   ended = true;
   //MFS.write("End");
   //MFS.blinkDisplay(DIGIT_ALL, ON);
   // DriveMotor->run(RELEASE);
   error();
 }
*/
//error();
 delay(50);

if (!LeftSensor && !RightSensor)
{
    MFS.write(a);
  //Error();
  // MFS.write("BOTH");
}
/*
else if (LeftSensor){
  MFS.write("LEFT");
}
else if (RightSensor){
  MFS.write("RIGHT");
}
else if (!LeftSensor && !RightSensor){
  MFS.write("NONE");
}
*/
///
else
{
 switch(DriveMotor_State)
 {
  case DriveMotor_Stop:
    Stop();
    break; // leaving this says stop, removing it says prog ??????????????????
    // ???????????????????????????????????????????????????????
  case DriveMotor_MovingLeft:
  case DriveMotor_MovingRight:
  case DriveMotor_PausedLeft:
  case DriveMotor_PausedRight:
  case DriveMotor_Error:
  default:
    MFS.write("prog");
    ShakeMotor->run(RELEASE);
    DriveMotor->run(RELEASE);
  }

}

}

void Error() {
/*
      DriveMotor->setSpeed(255);

      MFS.write("1111");
      MFS.blinkDisplay(DIGIT_ALL, ON);
      DriveMotor->run(BACKWARD);
*/
      DriveMotor->setSpeed(0);
      ShakeMotor->setSpeed(0);
      Stop();
      MFS.write("ERR");
      MFS.blinkDisplay(DIGIT_ALL, ON);
}

void MovingLeft(bool LeftSensor, bool RightSensor){

}
void Stop(){
  MFS.write("STOP");
  DriveMotor->run(RELEASE);
  ShakeMotor->run(RELEASE);
}

void MovingRight(bool LeftSensor, bool RightSensor){

}


void PausedLeft(bool LeftSensor,bool RightSensor){

}


void PausedRight(bool LeftSensor, bool RightSensor){

}
