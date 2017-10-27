//#include "StandardCplusplus.h"

#include "main.hpp"
// uint8_t DriveMotor_State = DriveMotor_Stop;
enum motor_type {
    DriveMotor_Stop,
    DriveMotor_MovingLeft,
    DriveMotor_MovingRight,
    DriveMotor_PausedLeft,
    DriveMotor_PausedRight,
    DriveMotor_Error,
    DriveMotor_Engaged
};
motor_type DriveMotor_State = DriveMotor_Stop;
enum display_type {
    Display_MotorSpeed,
    Display_Min,
    Display_ShakeMotor_On,
    Display_ShakeMotor_Off,
    Display_Stop,
    Display_Error,
    Display_Sensor_Test
};
display_type Display_State;// = Display_Stop;

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
//using namespace std;
//using namespace std;


int counter=0;
byte ended = false;
bool LeftSensor = false;
bool RightSensor = false;
const int LeftSensorPin = 6;
const int RightSensorPin = 5;
int DriveMotorCycleTime = 10;
int shakemotorspeed = 5;
bool ShakeMotorRunning = false;
int long timeout = 500; //ms
unsigned long DisplayTime = 0;
unsigned long GoTime = 0;
bool Time_Transition = false;

//end moved back to blank space

void Error();
void MovingLeft(bool LeftSensor, bool RightSensor);
void MovingRight(bool LeftSensor, bool RightSensor);
void PausedLeft(bool LeftSensor,bool RightSensor);
void PausedRight(bool LeftSensor, bool RightSensor);
void Stop();
void Sensor_Test();

void setup() {
    //AFMotorShield = Adafruit_MotorShield();

    DriveMotor = AFMotorShield.getMotor(3);
    ShakeMotor = AFMotorShield.getMotor(4);
    AFMotorShield.begin();
    DriveMotor->setSpeed(150);
    //DriveMotor->run(FORWARD);
    ShakeMotor->setSpeed(75);
    //ShakeMotor->run(FORWARD);
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
    //*
    delay(500);
    byte btn = MFS.getButton();
    delay(500);
    Display_State = Display_Stop;
    if((btn==BUTTON_1_PRESSED) | (btn==BUTTON_2_PRESSED) | (btn==BUTTON_3_PRESSED)){
        Display_State = Display_Sensor_Test;
    }
    if(btn==BUTTON_1_LONG_PRESSED){
        //Display_State = Display_Sensor_Test;
    }
    //*/


}



void loop() {
    if(ShakeMotorRunning){
        ShakeMotor->run(FORWARD);
    }
    else{
        ShakeMotor->run(RELEASE);
    }

    byte btn = MFS.getButton();
  LeftSensor = !digitalRead(LeftSensorPin);
  RightSensor = !digitalRead(RightSensorPin);
  //*
if (Display_State == Display_Sensor_Test){
    Sensor_Test();
}
else{
//*/
  if (!(LeftSensor && RightSensor))
  {
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

//error();
 delay(50);

}
*/

if (millis() < DisplayTime){
    Time_Transition = true;
 switch (Display_State){
     case Display_Min:
     MFS.write("LO",1);
     break;
     case Display_ShakeMotor_Off:
     MFS.write("OFF",1);
     break;
     case Display_ShakeMotor_On:
     MFS.write("ON",1);
     break;
     case Display_Stop:
     break;
     case Display_MotorSpeed:
     break;
     case Display_Error:
     break;
     case Display_Sensor_Test:
     break;
 }
}
else if (Time_Transition){
        Time_Transition = false;
        Display_State = Display_MotorSpeed;
        MFS.write(DriveMotorCycleTime);
}
else{
        switch (Display_State){
            case Display_Min:
            break;
            case Display_ShakeMotor_Off:
            break;
            case Display_ShakeMotor_On:
            break;
            case Display_MotorSpeed:
            MFS.write(DriveMotorCycleTime);
            break;
            case Display_Stop:
            MFS.write("STOP");
            break;
            case Display_Error:
            MFS.write("ERR");
            break;
            case Display_Sensor_Test:
            break;
        }
    }


/*if((btn==BUTTON_1_PRESSED)&&(btn==BUTTON_3_PRESSED)){
    Display_State = Display_Sensor_Test;
*/
//}
/*else{
    DriveMotor_State = DriveMotor_Error;
}
*/
//else{

    if(btn==BUTTON_1_PRESSED){
        if (Display_State == Display_Stop){
            DriveMotorCycleTime = 10;
            Display_State = Display_MotorSpeed;
            DriveMotor_State = DriveMotor_Engaged;
        }
        else if (DriveMotorCycleTime > 1){
        DriveMotorCycleTime--;
        Display_State = Display_MotorSpeed;
        }
        else{
            Display_State = Display_Min;
            DisplayTime = millis() + timeout;
        }
    }

    if(btn==BUTTON_2_PRESSED){
        if (DriveMotorCycleTime < 10){
            DriveMotorCycleTime++;
            Display_State = Display_MotorSpeed;
        }
        else if(DriveMotorCycleTime == 10){
            DriveMotor_State = DriveMotor_Stop;
            Display_State = Display_Stop;
        }
        else{
            DriveMotor_State = DriveMotor_Error;
        }
    }

    if((btn==BUTTON_3_PRESSED) && (Display_State!=Display_Stop)){
        ShakeMotorRunning = ShakeMotorRunning != true;
        DisplayTime = millis() + timeout;
        if (ShakeMotorRunning){
            Display_State = Display_ShakeMotor_On;
        }
        else{
            Display_State = Display_ShakeMotor_Off;
        }
    }




//else
//{
 switch(DriveMotor_State)
 {
  case DriveMotor_Stop:
    Stop();
    break;
  case DriveMotor_MovingLeft:
        DriveMotor->run(BACKWARD);
        GoTime = millis() + DriveMotorCycleTime*500;
        if (LeftSensor){
            DriveMotor_State = DriveMotor_PausedLeft;
            MFS.write("00");
            delay(50);
        }
        break;
  case DriveMotor_MovingRight:
        DriveMotor->run(FORWARD);
    GoTime = millis() + DriveMotorCycleTime*500;
    if (RightSensor){
        DriveMotor_State = DriveMotor_PausedRight;
        MFS.write("00",1);
        delay(50);
    }
    break;
  case DriveMotor_PausedLeft:
    DriveMotor->run(RELEASE);
  if (millis() > GoTime){
      DriveMotor_State = DriveMotor_MovingRight;
      DriveMotor->setSpeed(150);
      DriveMotor->run(FORWARD);
      MFS.write("00");
      delay(50);
    }
    break;
  case DriveMotor_PausedRight:
  DriveMotor->run(RELEASE);
      if (millis() > GoTime){
          DriveMotor_State = DriveMotor_MovingLeft;
          DriveMotor->setSpeed(150);
          DriveMotor->run(BACKWARD);
          MFS.write("00",1);
          delay(50);

        }
        break;
  case DriveMotor_Error:
  Error();
  break;
  case DriveMotor_Engaged:
  if (LeftSensor)
    DriveMotor_State = DriveMotor_MovingRight;
    else{
        DriveMotor_State = DriveMotor_MovingLeft;
    }
  break;
  default:
    MFS.write("prog");
    ShakeMotor->run(RELEASE);
    DriveMotor->run(RELEASE);
  }

//}
//} //end double press else
}
else{
    MFS.write("SENS");
    Stop();
    //Display_State = Display_Sensor_Test;
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
      //DriveMotor->setSpeed(0);
      //ShakeMotor->setSpeed(0);
      Stop();
      DriveMotor_State = DriveMotor_Error;
      Display_State = Display_Error;
      //MFS.write("ERR");
      MFS.blinkDisplay(DIGIT_ALL, ON);
}

void MovingLeft(bool LeftSensor, bool RightSensor){

}
void Stop(){
  //MFS.write("STOP");
  Display_State = Display_Stop;
  DriveMotor_State = DriveMotor_Stop;
  ShakeMotorRunning = false;
  DriveMotor->run(RELEASE);
  ShakeMotor->run(RELEASE);
}

void MovingRight(bool LeftSensor, bool RightSensor){

}


void PausedLeft(bool LeftSensor,bool RightSensor){

}


void PausedRight(bool LeftSensor, bool RightSensor){

}

void Sensor_Test(){
    if (LeftSensor && RightSensor){
        MFS.write("BOTH");
    }
    else if (LeftSensor){
        MFS.write("LEFT");
    }
    else if (RightSensor){
        MFS.write("RIGHT");
    }
    else {
        MFS.write("NONE");
    }

}
