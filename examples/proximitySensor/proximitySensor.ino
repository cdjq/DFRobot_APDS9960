/*!
 *@file proximitySensor.ino
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

// Global Variables
DFRobot_APDS9960 colwave = DFRobot_APDS9960();
uint8_t proximity_data = 0;

void setup() {
  
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println("color wave sensor boot........");
  
  // Initialize ColorWaveSensor (configure I2C and initial values)
  if ( colwave.init() ) 
  {
		Serial.println(F("ColorWaveSensor initialization complete"));
  } 
  else 
  {
		Serial.println(F("Something went wrong during ColorWaveSensor init!"));
  }
  
  // Adjust the Proximity sensor gain
  if ( !colwave.setProximityGain(PGAIN_2X) ) 
  {
		Serial.println(F("Something went wrong trying to set PGAIN"));
  }
  
  // Start running the ColorWaveSensor proximity sensor (no interrupts)
  if ( colwave.enableProximitySensor(false) ) 
  {
		Serial.println(F("Proximity sensor is now running"));
  } 
  else 
  {
		Serial.println(F("Something went wrong during sensor init!"));
  }
}

void loop() {
  
  // Read the proximity value
  if ( !colwave.readProximity(proximity_data) ) 
  {
		Serial.println("Error reading proximity value");
  } 
  else 
  {
		Serial.print("Proximity: ");
		Serial.println(proximity_data);
  }
  
  // Wait 250 ms before next reading
  delay(250);
}