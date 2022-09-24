/*!
 *@file  ambientLightInterrupt.ino
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
#define APDS9960_INT    2  // Needs to be an interrupt pin
#define LED_PIN         13 // LED for showing interrupt

// Constants
#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   10   // Low light level for interrupt

// Global variables
DFRobot_APDS9960 apds = DFRobot_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;
uint16_t threshold = 0;

void setup() {
  
  // Set LED as output
  Serial.println("color wave sensor boot......");
  pinMode(LED_PIN, OUTPUT);
  pinMode(APDS9960_INT, INPUT);
  Serial.begin(9600);
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  // Initialize APDS-9960 (configure I2C and initial values)
  if (!apds.init()) 
  {
		Serial.println(F("init fail"));
  } 
  // Set high and low interrupt thresholds
  if (!apds.setLightIntLowThreshold(LIGHT_INT_LOW)) 
  {
		Serial.println(F("write low threshold error"));
  }
  if (!apds.setLightIntHighThreshold(LIGHT_INT_HIGH)) 
  {
		Serial.println(F("write high threshold error"));
  }
  // Start running the APDS-9960 light sensor (no interrupts)
  if (!apds.enableLightSensor(false)) 
  {
		Serial.println(F("enable light sensor fail"));
  } 
  // Read high and low interrupt thresholds
  if ( !apds.getLightIntLowThreshold(threshold)) 
  {
		Serial.println(F("read low threshold error"));
  } 
  if ( !apds.getLightIntHighThreshold(threshold) ) 
  {
		Serial.println(F("read high threshold error"));
  } 
  // Enable interrupts
  if (!apds.setAmbientLightIntEnable(1)) 
  {
		Serial.println(F("enable interrupts "));
  }
  delay(500);
}

void loop() {
  
  // If interrupt occurs, print out the light levels
  if ( isr_flag == 1 ) {
    
    // Read the light levels (ambient, red, green, blue) and print
    if (  !apds.readAmbientLight(ambient_light) ||
          !apds.readRedLight(red_light) ||
          !apds.readGreenLight(green_light) ||
          !apds.readBlueLight(blue_light) ) {
      Serial.println("Error reading light values");
    } else {
      Serial.print("Interrupt! Ambient: ");
      Serial.print(ambient_light);
      Serial.print(" R: ");
      Serial.print(red_light);
      Serial.print(" G: ");
      Serial.print(green_light);
      Serial.print(" B: ");
      Serial.println(blue_light);
    }
    
    // Turn on LED for a half a second
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    
    // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
    isr_flag = 0;
    if ( !apds.clearAmbientLightInt() ) 
	{
		Serial.println("Error clearing interrupt");
    }
    
  }
}

void interruptRoutine() {
  isr_flag = 1;
}
