//#include <string>
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
    Display_Min,
    Display_ShakeMotor_On,
    Display_ShakeMotor_Off,
    Display_Stop,
    Display_Error,
    Display_Sensor_Test
};
