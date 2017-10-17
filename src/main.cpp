#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

Adafruit_DCMotor *myMotor;
Adafruit_MotorShield AFMS;

void setup() {
    //AFMS = Adafruit_MotorShield();

    myMotor = AFMS.getMotor(1);
    AFMS.begin();
    myMotor->setSpeed(150);
    myMotor->run(FORWARD);
    Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library

  MFS.write("Hi");
  delay(2000);
  MFS.write(-273);
  delay(2000);
  MFS.write(3.141, 2);  // display to 2 decimal places.
  delay(2000);

}

int counter=0;
byte ended = false;

void loop() {

    // put your main code here, to run repeatedly:
    if (counter == 100)
    {
        myMotor->setSpeed(255);
    }
    if (counter == 150)

    {

        myMotor->run(BACKWARD);
    }
    if (counter < 200)
 {
   MFS.write((int)counter);
   counter++;
 }
 else if (!ended)
 {
   ended = true;
   MFS.write("End");
   MFS.blinkDisplay(DIGIT_ALL, ON);
 }
 delay(50);
}
