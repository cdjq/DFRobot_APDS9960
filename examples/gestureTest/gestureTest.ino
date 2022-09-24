/*!
 *@file gestureTest.ino
 *@brief DFRobot's Color Wave Sensor,DFRobot's Color Wave Sensor，this is the SparkFun RGB and Gesture Sensor, a small breakout board with a 
 *@n built in APDS-9960 sensor that offers ambient light and color measuring, proximity detection, 
 *@n and touchless gesture sensing. With this RGB and Gesture Sensor you will be able to control a computer,
 *@n microcontroller, robot, and more with a simple swipe of your hand!
 *@copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version     V1.0
 *@date        2022-09-24
 *@https://github.com/DFRobot/DFRobot_APDS9960
 */

#include <Wire.h>
#include <DFRobot_APDS9960.h>

// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Global Variables
DFRobot_APDS9960 apds = DFRobot_APDS9960();
int isr_flag = 0;

void setup() {
  Serial.begin(9600);
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  if (apds.init()) 
  {
		Serial.println(F("init successful"));
  } 
  else 
  {
		Serial.println(F("init fail"));
  }
  if (apds.enableGestureSensor(true)) 
  {
		Serial.println(F("enable gesture sensor successful"));
  } 
  else 
  {
		Serial.println(F("enable gesture sensor fail"));
  }
}

void loop() {
  if(isr_flag == 1) 
  {
    handleGesture();
    if(digitalRead(APDS9960_INT) == 0)
	{
      apds.init();
      apds.enableGestureSensor(true);
    }
    isr_flag = 0;
  }
}

void interruptRoutine() 
{
  isr_flag = 1;
}

void handleGesture() 
{
    if ( apds.isGestureAvailable() ) 
	{
    switch ( apds.readGesture() ) {
      case eDirUp:
        Serial.println("UP");
        break;
      case eDirDown:
        Serial.println("DOWN");
        break;
      case eDirLeft:
        Serial.println("LEFT");
        break;
      case eDirRight:
        Serial.println("RIGHT");
        break;
      case eDirNear:
        Serial.println("NEAR");
        break;
      case eDirFar:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}
