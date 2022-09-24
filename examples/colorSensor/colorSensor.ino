/*!
 *@file colorSensor.ino
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
DFRobot_APDS9960 apds = DFRobot_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

void setup() {
  Serial.begin(9600);
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( !apds.init() ) 
  {
		Serial.println(F("init fail"));
  } 
  // Start running the APDS-9960 light sensor (no interrupts)
  if ( !apds.enableLightSensor(false) ) 
  {
		Serial.println(F("enable light sensor fail"));
  } 
  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
  
  // Read the light levels (ambient, red, green, blue)
  if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
    Serial.println("Error reading light values");
  } else {
    Serial.print("Ambient: ");
    Serial.print(ambient_light);
    Serial.print(" Red: ");
    Serial.print(red_light);
    Serial.print(" Green: ");
    Serial.print(green_light);
    Serial.print(" Blue: ");
    Serial.println(blue_light);
  }
  delay(1000);
}
