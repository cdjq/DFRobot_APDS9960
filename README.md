# DFRobot_APDS9960
- [中文版](./README_CN.md)

DFRobot's Color Wave Sensor，this is the SparkFun RGB and Gesture Sensor, a small breakout board with a built in <br>
APDS-9960 sensor that offers ambient light and color measuring, proximity detection, and touchless gesture sensing. <br>
With this RGB and Gesture Sensor you will be able to control a computer, microcontroller, robot, and more with a <br>
simple swipe of your hand!<br>
![Product Image](./resources/images/SEN0187.jpg)

## Product Link（https://www.dfrobot.com/product-1361.html)    
   SEN0187：RGB Color and Gesture Sensor For Arduino
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

Provide an Arduino library to drive RGB Color and Gesture Sensor

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods
```C++
   /**
    * @fn init
	* @brief Configures I2C communications and initializes registers to defaults
	* @return True if initialized successfully. False otherwise.
	*/
   bool init();
	
   /**
    * @fn getMode
	* @brief Reads and returns the contents of the ENABLE register
	* @return Contents of the ENABLE register. 0xFF if error.
	*/
  uint8_t getMode();
	
   /**
    * @fn setMode
	* @brief Enables or disables a feature in the APDS-9960
	* @param mode which feature to enable
	* @param enable ON (1) or OFF (0)
	* @return True if operation success. False otherwise.
	*/
   bool setMode(uint8_t mode, uint8_t enable);
    
   /**
    * @fn enablePower
	* @brief Starts the light (R/G/B/Ambient) sensor on the APDS-9960
	* @return True if sensor enabled correctly. False on error.
	*/
   bool enablePower();
	
   /**
    * @fn disablePower
	* @brief Ends the light sensor on the APDS-9960
	* @return True if sensor disabled correctly. False on error.
	*/
   bool disablePower();
	
   /**
    * @fn enableLightSensor
	* @brief Enable light sensors
    * @param interrupts true to enable hardware interrupt on high or low light
	* @return True if sensor disabled correctly. False on error.
	*/
   bool enableLightSensor(bool interrupts = false);
	
   /**
    * @fn disableLightSensor
	* @brief Disable light sensors
	* @return True if sensor disabled correctly. False on error.
	*/
   bool disableLightSensor();
	
   /**
    * @fn enableProximitySensor
	* @brief Starts the proximity sensor on the APDS-9960
	* @param interrupts true to enable hardware external interrupt on proximity
	* @return True if sensor enabled correctly. False on error.
	*/
   bool enableProximitySensor(bool interrupts = false);
	
   /**
    * @fn disableProximitySensor
	* @brief stop the proximity sensor on the APDS-9960
	* @param interrupts true to enable hardware external interrupt on proximity
	* @return True if sensor enabled correctly. False on error.
	*/
   bool disableProximitySensor();
	
   /**
    * @fn enableGestureSensor
	* @brief Starts the gesture recognition engine on the APDS-9960
	* @param interrupts true to enable hardware external interrupt on gesture
	* @return True if engine enabled correctly. False on error.
	*/
   bool enableGestureSensor(bool interrupts = true);
	
   /**
    * @fn disableGestureSensor
	* @brief Ends the gesture recognition engine on the APDS-9960
	* @return True if engine disabled correctly. False on error.
	*/
   bool disableGestureSensor();
    
   /**
    * @fn getLEDDrive
	* @brief Returns LED drive strength for proximity and ALS
	* @n Value    LED Current
	* @n   0        100 mA
	* @n   1         50 mA
	* @n   2         25 mA
	* @n   3         12.5 mA
	* @n 
	* @return the value of the LED drive strength. 0xFF on failure.
	*/
   uint8_t getLEDDrive();
	
   /**
    * @fn setLEDDrive
	* @brief Sets the LED drive strength for proximity and ALS
	* @n Value    LED Current
	* @n   0        100 mA
	* @n   1         50 mA
	* @n   2         25 mA
	* @n   3         12.5 mA
	* @n 
	* @param drive the value (0-3) for the LED drive strength
	* @return True if operation successful. False otherwise.
	*/
   bool setLEDDrive(uint8_t drive);
	
   /**
    * @fn getGestureLEDDrive
	* @brief Gets the drive current of the LED during gesture mode
	* @n Value    LED Current
	* @n   0        100 mA
	* @n   1         50 mA
	* @n   2         25 mA
	* @n   3         12.5 mA
	* @n 
	* @return the LED drive current value. 0xFF on error.
	*/
   uint8_t getGestureLEDDrive();
	
   /**
    * @fn setGestureLEDDrive
	* @brief Sets the LED drive current during gesture mode
	* @n Value    LED Current
	* @n   0        100 mA
	* @n   1         50 mA
	* @n   2         25 mA
	* @n   3         12.5 mA
	* @n 
	* @param drive the value for the LED drive current
	* @return True if operation successful. False otherwise.
	*/
   bool setGestureLEDDrive(uint8_t drive);
    
   /**
    * @fn getAmbientLightGain
	* @brief Returns receiver gain for the ambient light sensor (ALS)
	* @n Value    Gain
	* @n   0        1x
	* @n   1        4x
	* @n   2       16x
	* @n   3       64x
	* @n 
	* @return the value of the ALS gain. 0xFF on failure.
	*/
   uint8_t getAmbientLightGain();
	
   /**
    * @fn setAmbientLightGain
	* @brief Sets the receiver gain for the ambient light sensor (ALS)
	* @n Value    Gain
	* @n   0        1x
	* @n   1        4x
	* @n   2       16x
	* @n   3       64x
	* @n 
	* @param gain the value (0-3) for the gain
	* @return True if operation successful. False otherwise.
	*/
   bool setAmbientLightGain(uint8_t gain);
	
   /**
    * @fn getProximityGain
	* @brief Returns receiver gain for proximity detection
	* @n Value    Gain
	* @n   0       1x
	* @n   1       2x
	* @n   2       4x
	* @n   3       8x
	* @n 
	* @return the value of the proximity gain. 0xFF on failure.
	*/
   uint8_t getProximityGain();
	
   /**
    * @fn setProximityGain
	* @brief Sets the receiver gain for proximity detection
	* @n Value    Gain
	* @n   0       1x
	* @n   1       2x
	* @n   2       4x
	* @n   3       8x
	* @n 
	* @param gain the value (0-3) for the gain
	* @return True if operation successful. False otherwise.
	*/
   bool setProximityGain(uint8_t gain);
	
   /**
    * @fn getGestureGain
	* @brief Gets the gain of the photodiode during gesture mode
	* @n Value    Gain
	* @n   0       1x
	* @n   1       2x
	* @n   2       4x
	* @n   3       8x
	* @n 
	* @return the current photodiode gain. 0xFF on error.
	*/
   uint8_t getGestureGain();
	
   /**
    * @fn setGestureGain
	* @brief Sets the gain of the photodiode during gesture mode
	* @n Value    Gain
	* @n   0       1x
	* @n   1       2x
	* @n   2       4x
	* @n   3       8x
	* @n 
	* @param gain the value for the photodiode gain
	* @return True if operation successful. False otherwise.
	*/
   bool setGestureGain(uint8_t gain);
    
   /**
    * @fn getLightIntLowThreshold
	* @brief Gets the low threshold for ambient light interrupts
	* @param threshold current low threshold stored on the APDS-9960
	* @return True if operation successful. False otherwise.
	*/
   bool getLightIntLowThreshold(uint16_t &threshold);
	
   /**
    * @fn setLightIntLowThreshold
	* @brief Sets the low threshold for ambient light interrupts
	* @param threshold low threshold value for interrupt to trigger
	* @return True if operation successful. False otherwise.
	*/
   bool setLightIntLowThreshold(uint16_t threshold);
	
   /**
    * @fn getLightIntHighThreshold
	* @brief Gets the high threshold for ambient light interrupts
	* @param threshold current low threshold stored on the APDS-9960
	* @return True if operation successful. False otherwise.
	*/
   bool getLightIntHighThreshold(uint16_t &threshold);
	
   /**
    * @fn setLightIntHighThreshold
	* @brief Sets the high threshold for ambient light interrupts
	* @param threshold high threshold value for interrupt to trigger
	* @return True if operation successful. False otherwise.
	*/
   bool setLightIntHighThreshold(uint16_t threshold);
    
   /**
    * @fn getProximityIntLowThreshold
	* @brief Gets the low threshold for proximity interrupts
	* @param threshold current low threshold stored on the APDS-9960
	* @return True if operation successful. False otherwise.
	*/
   bool getProximityIntLowThreshold(uint8_t &threshold);
	
   /**
    * @fn setProximityIntLowThreshold
	* @brief Sets the low threshold for proximity interrupts
	* @param threshold low threshold value for interrupt to trigger
	* @return True if operation successful. False otherwise.
	*/
   bool setProximityIntLowThreshold(uint8_t threshold);
	
   /**
    * @fn getProximityIntHighThreshold
	* @brief Gets the high threshold for proximity interrupts
	* @param threshold current low threshold stored on the APDS-9960
	* @return True if operation successful. False otherwise.
	*/
   bool getProximityIntHighThreshold(uint8_t &threshold);
	
   /**
    * @fn setProximityIntHighThreshold
	* @brief Sets the high threshold for proximity interrupts
	* @param threshold high threshold value for interrupt to trigger
	* @return True if operation successful. False otherwise.
	*/
   bool setProximityIntHighThreshold(uint8_t threshold);
    
   /**
    * @fn getAmbientLightIntEnable
	* @brief Gets if ambient light interrupts are enabled or not
	* @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
	*/
   uint8_t getAmbientLightIntEnable();
	
   /**
    * @fn setAmbientLightIntEnable
	* @brief Turns ambient light interrupts on or off
	* @param enable 1 to enable interrupts, 0 to turn them off
	* @return True if operation successful. False otherwise.
	*/
   bool setAmbientLightIntEnable(uint8_t enable);
	
   /**
    * @fn getProximityIntEnable
	* @brief Gets if proximity interrupts are enabled or not
	* @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
	*/
   uint8_t getProximityIntEnable();
	
   /**
    * @fn setProximityIntEnable
	* @brief Turns proximity interrupts on or off
	* @param enable 1 to enable interrupts, 0 to turn them off
	* @return True if operation successful. False otherwise.
	*/
   bool setProximityIntEnable(uint8_t enable);
	
   /**
    * @fn getGestureIntEnable
	* @brief Gets if gesture interrupts are enabled or not
	* @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
	*/
   uint8_t getGestureIntEnable();
	
   /**
    * @fn setGestureIntEnable
	* @brief Turns gesture-related interrupts on or off
	* @param enable 1 to enable interrupts, 0 to turn them off
	* @return True if operation successful. False otherwise.
	*/
   bool setGestureIntEnable(uint8_t enable);
    
   /**
    * @fn clearAmbientLightInt
	* @brief Clears the ambient light interrupt
	* @return True if operation completed successfully. False otherwise.
	*/
   bool clearAmbientLightInt();
	
   /**
    * @fn clearProximityInt
	* @brief Clears the proximity interrupt
	* @return True if operation completed successfully. False otherwise.
	*/
   bool clearProximityInt();
    
   /**
    * @fn readAmbientLight
	* @brief Reads the ambient (clear) light level as a 16-bit value
	* @param val value of the light sensor.
	* @return True if operation successful. False otherwise.
	*/
   bool readAmbientLight(uint16_t &val);
	
   /**
    * @fn readRedLight
	* @brief Reads the red light level as a 16-bit value
	* @param val value of the light sensor.
	* @return True if operation successful. False otherwise.
	*/
   bool readRedLight(uint16_t &val);
	
   /**
    * @fn readGreenLight
	* @brief Reads the green light level as a 16-bit value
	* @param val value of the light sensor.
	* @return True if operation successful. False otherwise.
	*/
   bool readGreenLight(uint16_t &val);
	
   /**
    * @fn readBlueLight
	* 	@brief Reads the red light level as a 16-bit value
	* 	@param[out] val value of the light sensor.
	* 	@return True if operation successful. False otherwise.
	*/
   bool readBlueLight(uint16_t &val);
    
    /**
    * @fn readProximity
	* 	@brief Reads the proximity level as an 8-bit value
	* 	@param[out] val value of the proximity sensor.
	* 	@return True if operation successful. False otherwise.
	*/
   bool readProximity(uint8_t &val);
    
   /**
    * @fn isGestureAvailable
	* @brief Determines if there is a gesture available for reading
	* @return True if gesture available. False otherwise.
	*/
   bool isGestureAvailable();
	
   /**
    * @fn readGesture
	* @brief Processes a gesture event and returns best guessed gesture
	* @return Number corresponding to gesture. -1 on error.
	*/
   int readGesture();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
FireBeetle-ESP8266        |      √       |              |             | 
FireBeetle-ESP32        |      √       |              |             | 
Arduino MEGA2560        |      √       |              |             | 
Arduino Leonardo|      √       |              |             | 
Micro:bit        |      √       |              |             | 
FireBeetle-M0        |      √       |              |             | 
## History

- 2022/09/24 - Version 1.0.0 released.
## Credits

Written by fengli(li.feng@dfrobot.com), 2021.7.2 (Welcome to our [website](https://www.dfrobot.com/))





