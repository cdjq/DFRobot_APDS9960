/*!
 *@file proximityInterrupt.ino
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
#define PROX_INT_HIGH   50 // Proximity level for interrupt
#define PROX_INT_LOW    0  // No far interrupt

// Global variables
DFRobot_APDS9960 apds = DFRobot_APDS9960();
uint8_t proximity_data = 0;
int isr_flag = 0;

void setup() {
  
  // Set LED as output
  Serial.println("color wave sensor boot.......");
  pinMode(LED_PIN, OUTPUT);
  pinMode(APDS9960_INT, INPUT);
  Serial.begin(9600);
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( !apds.init() ) 
  {
		Serial.println(F("init fail"));
  } 
  
  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) 
  {
		Serial.println(F("set PGAIN error"));
  }
  
  // Set proximity interrupt thresholds
  if (!apds.setProximityIntLowThreshold(PROX_INT_LOW)) 
  {
		Serial.println(F("write low threshold error"));
  }
  if ( !apds.setProximityIntHighThreshold(PROX_INT_HIGH) ) 
  {
    Serial.println(F("write  high threshold error"));
  }
  // Start running the APDS-9960 proximity sensor (interrupts)
  if ( !apds.enableProximitySensor(true) ) 
  {
		Serial.println(F("enable proximity sensor error"));
  } 
}

void loop() {
  
  // If interrupt occurs, print out the proximity level
  if ( isr_flag == 1 ) {
  
    // Read proximity level and print it out
    if ( !apds.readProximity(proximity_data) ) 
	{
		Serial.println("read proximity value error");
    } 
    // Turn on LED for a half a second
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
    isr_flag = 0;
    if ( !apds.clearProximityInt() ) 
	{
		Serial.println("clearing interrupt error");
    }
	else 
	{
		Serial.print("Proximity detected! Level: ");
		Serial.println(proximity_data);
    }
    
  }
}

void interruptRoutine() {
  isr_flag = 1;
}
