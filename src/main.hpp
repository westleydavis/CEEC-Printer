#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

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
    Display_Max,
    Display_ShakeMotor_On,
    Display_ShakeMotor_Off,
    Display_Stop,
    Display_Error,
    Display_Sensor_Test,
    Display_Pot_Test
};
display_type Display_State;// = Display_Stop;

Adafruit_DCMotor *DriveMotor;
Adafruit_DCMotor *ShakeMotor;
Adafruit_MotorShield AFMotorShield;

int counter=0;
byte ended = false;
bool LeftSensor = false;
bool RightSensor = false;
const int LeftSensorPin = 6;
const int RightSensorPin = 5;
const int PotPin = A0;
int PotVal = 0;
// int DriveMotorCycleTime = 10;
int DriveMotorSpeed = 0;
int DriveMotorSpeed_Prev = 0;
int ShakeMotorSpeed = 0;
int ShakeMotorSpeed_Prev = 0;
bool ShakeMotorRunning = false;
int long timeout = 500; //ms
unsigned long DisplayTime = 0;
unsigned long GoTime = 0;
bool Time_Transition = false;
float DispVal = 0;



void Error();
void MovingLeft(bool LeftSensor, bool RightSensor);
void MovingRight(bool LeftSensor, bool RightSensor);
void PausedLeft(bool LeftSensor,bool RightSensor);
void PausedRight(bool LeftSensor, bool RightSensor);
void Stop();
void Sensor_Test();
void Pot_Test();
void shakemotor();
