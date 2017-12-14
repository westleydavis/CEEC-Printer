#include "main.hpp"

void setup() {
    DriveMotor = AFMotorShield.getMotor(3);
    ShakeMotor = AFMotorShield.getMotor(4);
    AFMotorShield.begin();
    DriveMotor->setSpeed(255);
    ShakeMotor->setSpeed(75);
    Timer1.initialize();
    MFS.initialize(&Timer1);    // initialize multi-function shield library
    pinMode(LeftSensorPin, INPUT);
    pinMode(RightSensorPin, INPUT);
    delay(500);
    byte btn = MFS.getButton();
    delay(500);
    Display_State = Display_Stop;
    if((btn==BUTTON_1_PRESSED) | (btn==BUTTON_2_PRESSED) | (btn==BUTTON_3_PRESSED)){
        Display_State = Display_Sensor_Test;
    }
    if(btn==BUTTON_1_PRESSED){
        Display_State = Display_Pot_Test;
    }
}
void loop() {
    shakemotor();
    Pot_Test();
    byte btn = MFS.getButton();
    LeftSensor = !digitalRead(LeftSensorPin);
    RightSensor = !digitalRead(RightSensorPin);

    if (Display_State == Display_Sensor_Test){
      Sensor_Test();
      return;
    }
    if (LeftSensor && RightSensor){
        MFS.write("SENS");
        Stop();
        return;
    }

    if (millis() < DisplayTime){
      Time_Transition = true;
      switch (Display_State){
        case Display_Max:
        MFS.write("hi",1);
        break;
        case Display_ShakeMotor_Off:
        MFS.write("OFF",1);
        break;
        case Display_ShakeMotor_On:
        MFS.write("ON",1);
        ShakeMotor->setSpeed(100);
        break;
        case Display_Stop:
        break;
        case Display_MotorSpeed:
        break;
        case Display_Error:
        break;
        case Display_Sensor_Test:
        break;
        case Display_Pot_Test:
        break;
      }
    }
    else if (Time_Transition){
      Time_Transition = false;
      Display_State = Display_MotorSpeed;
      MFS.write(DriveMotorSpeed);
    }
    else{
      switch (Display_State){
        case Display_Max:
        break;
        case Display_ShakeMotor_Off:
        break;
        case Display_ShakeMotor_On:
        break;
        case Display_MotorSpeed:
        DispVal = float(DriveMotorSpeed)+float(ShakeMotorSpeed)/100.0;
        MFS.write(DispVal);
        break;
        case Display_Stop:
        MFS.write("STOP");
        //MFS.write("1"+67);
        break;
        case Display_Error:
        MFS.write("ERR");
        break;
        case Display_Sensor_Test:
        break;
        case Display_Pot_Test:
        break;
      }
    }
    if(btn==BUTTON_1_PRESSED){
      if (Display_State == Display_Stop){
      }
      else if (DriveMotorSpeed > 0){
        DriveMotorSpeed--;
        Display_State = Display_MotorSpeed;
      }
      if (DriveMotorSpeed == 0){
        Display_State = Display_Stop;
        DriveMotor_State = DriveMotor_Stop;
      }
    }
    if(btn==BUTTON_2_PRESSED){
      if (DriveMotorSpeed < 10){
        DriveMotorSpeed++;
        Display_State = Display_MotorSpeed;
        DriveMotor_State = DriveMotor_Engaged;
      }
      else if(DriveMotorSpeed == 10){
        Display_State = Display_Max;
        DisplayTime = millis() + timeout;
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
    switch(DriveMotor_State){
      case DriveMotor_Stop:
      Stop();
      break;
      case DriveMotor_MovingLeft:
      DriveMotor->setSpeed(DriveMotorSpeed*10+155);
      DriveMotor->run(BACKWARD);
      GoTime = millis() + (11-DriveMotorSpeed)*250;
      if (LeftSensor){
        DriveMotor_State = DriveMotor_PausedLeft;
        MFS.write("00");
        delay(50);
      }
      break;
      case DriveMotor_MovingRight:
      DriveMotor->setSpeed(DriveMotorSpeed*10+155);
      DriveMotor->run(FORWARD);
      GoTime = millis() + (11-DriveMotorSpeed)*250;
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
        DriveMotor->setSpeed(255);
        DriveMotor->run(FORWARD);
        MFS.write("00");
        delay(50);
      }
      break;
      case DriveMotor_PausedRight:
      DriveMotor->run(RELEASE);
      if (millis() > GoTime){
        DriveMotor_State = DriveMotor_MovingLeft;
        DriveMotor->setSpeed(255);
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
}
void Error() {
      Stop();
      DriveMotor_State = DriveMotor_Error;
      Display_State = Display_Error;
      MFS.blinkDisplay(DIGIT_ALL, ON);
}
void MovingLeft(bool LeftSensor, bool RightSensor){
}
void Stop(){
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
void shakemotor(){
  if(ShakeMotorRunning){
    ShakeMotor->run(FORWARD);
    ShakeMotor->setSpeed(ShakeMotorSpeed*15);

  }
  else{
    ShakeMotor->run(RELEASE);

  }
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
void Pot_Test(){
    //if (!ShakeMotorRunning){
    PotVal = analogRead(PotPin);
    ShakeMotorSpeed = 11-((((PotVal - 470) * 5 - 140) / 12) + 1);
    if (ShakeMotorSpeed > 10){
      ShakeMotorSpeed = 10;
    }
    if (ShakeMotorSpeed < 1){
      ShakeMotorSpeed = 1;
    }
  //}
}
