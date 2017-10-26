//#include <string>
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
