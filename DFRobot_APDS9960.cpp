/*!
 * @file DFRobot_APDS9960.cpp
 * @brief color wave sensor
 * @details Header file for DFRobot's color wave sensors
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-9-24
 * @https://github.com/DFRobot/DFRobot_APDS9960
 */
 
#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_APDS9960.h"
 

DFRobot_APDS9960::DFRobot_APDS9960()
{
    gesture_ud_delta_ = 0;
    gesture_lr_delta_ = 0;
    gesture_ud_count_ = 0;
    gesture_lr_count_ = 0;
    gesture_near_count_ = 0;
    gesture_far_count_ = 0; 
    gesture_state_ = 0;
    gesture_motion_ = eDirNone;
}
 

DFRobot_APDS9960::~DFRobot_APDS9960()
{

}


bool DFRobot_APDS9960::init()
{
    uint8_t id;

    ///< Initialize I2C 
    Wire.begin();
     
    ///< Read ID register and check against known values for APDS-9960 
    if( !wireReadDataByte(APDS9960_ID, id) ) {
        return false;
    }
    if( !(id == APDS9960_ID_1 || id == APDS9960_ID_2) ) {
        return false;
    }
     
    ///< Set ENABLE register to 0 (disable all features) 
    if( !setMode(ALL, OFF) ) {
        return false;
    }
    
    ///< Set default values for ambient light and proximity registers 
    if( !wireWriteDataByte(APDS9960_ATIME, DEFAULT_ATIME) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_WTIME, DEFAULT_WTIME) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_PPULSE, DEFAULT_PROX_PPULSE) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG1, DEFAULT_CONFIG1) ) {
        return false;
    }
    if( !setLEDDrive(DEFAULT_LDRIVE) ) {
        return false;
    }
    if( !setProximityGain(DEFAULT_PGAIN) ) {
        return false;
    }
    if( !setAmbientLightGain(DEFAULT_AGAIN) ) {
        return false;
    }
    if( !setProxIntLowThresh(DEFAULT_PILT) ) {
        return false;
    }
    if( !setProxIntHighThresh(DEFAULT_PIHT) ) {
        return false;
    }
    if( !setLightIntLowThreshold(DEFAULT_AILT) ) {
        return false;
    }
    if( !setLightIntHighThreshold(DEFAULT_AIHT) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_PERS, DEFAULT_PERS) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG2, DEFAULT_CONFIG2) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG3, DEFAULT_CONFIG3) ) {
        return false;
    }
    
    ///< Set default values for gesture sense registers 
    if( !setGestureEnterThresh(DEFAULT_GPENTH) ) {
        return false;
    }
    if( !setGestureExitThresh(DEFAULT_GEXTH) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GCONF1, DEFAULT_GCONF1) ) {
        return false;
    }
    if( !setGestureGain(DEFAULT_GGAIN) ) {
        return false;
    }
    if( !setGestureLEDDrive(DEFAULT_GLDRIVE) ) {
        return false;
    }
    if( !setGestureWaitTime(DEFAULT_GWTIME) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_U, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_D, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_L, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_R, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GPULSE, DEFAULT_GPULSE) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_GCONF3, DEFAULT_GCONF3) ) {
        return false;
    }
    if( !setGestureIntEnable(DEFAULT_GIEN) ) {
        return false;
    }
    
#if 0
    ///< Gesture config register dump 
    uint8_t reg;
    uint8_t val;
  
    for(reg = 0x80; reg <= 0xAF; reg++) {
        if( (reg != 0x82) && \
            (reg != 0x8A) && \
            (reg != 0x91) && \
            (reg != 0xA8) && \
            (reg != 0xAC) && \
            (reg != 0xAD) )
        {
            wireReadDataByte(reg, val);
            Serial.print(reg, HEX);
            Serial.print(": 0x");
            Serial.println(val, HEX);
        }
    }

    for(reg = 0xE4; reg <= 0xE7; reg++) {
        wireReadDataByte(reg, val);
        Serial.print(reg, HEX);
        Serial.print(": 0x");
        Serial.println(val, HEX);
    }
#endif

    return true;
}


uint8_t DFRobot_APDS9960::getMode()
{
    uint8_t enable_value;
    
    ///< Read current ENABLE register
    if( !wireReadDataByte(APDS9960_ENABLE, enable_value) ) {
        return ERROR;
    }
    
    return enable_value;
}


bool DFRobot_APDS9960::setMode(uint8_t mode, uint8_t enable)
{
    uint8_t reg_val;

    ///< Read current ENABLE register 
    reg_val = getMode();
    if( reg_val == ERROR ) {
        return false;
    }
    
    ///< Change bit(s) in ENABLE register 
    enable = enable & 0x01;
    if( mode >= 0 && mode <= 6 ) {
        if (enable) {
            reg_val |= (1 << mode);
        } else {
            reg_val &= ~(1 << mode);
        }
    } else if( mode == ALL ) {
        if (enable) {
            reg_val = 0x7F;
        } else {
            reg_val = 0x00;
        }
    }
        
    ///< Write value back to ENABLE register 
    if( !wireWriteDataByte(APDS9960_ENABLE, reg_val) ) {
        return false;
    }
        
    return true;
}


bool DFRobot_APDS9960::enableLightSensor(bool interrupts)
{
    
    ///< Set default gain, interrupts, enable power, and enable sensor 
    if( !setAmbientLightGain(DEFAULT_AGAIN) ) {
        return false;
    }
    if( interrupts ) {
        if( !setAmbientLightIntEnable(1) ) {
            return false;
        }
    } else {
        if( !setAmbientLightIntEnable(0) ) {
            return false;
        }
    }
    if( !enablePower() ){
        return false;
    }
    if( !setMode(AMBIENT_LIGHT, 1) ) {
        return false;
    }
    
    return true;

}


bool DFRobot_APDS9960::disableLightSensor()
{
    if( !setAmbientLightIntEnable(0) ) {
        return false;
    }
    if( !setMode(AMBIENT_LIGHT, 0) ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::enableProximitySensor(bool interrupts)
{
    ///< Set default gain, LED, interrupts, enable power, and enable sensor 
    if( !setProximityGain(DEFAULT_PGAIN) ) {
        return false;
    }
    if( !setLEDDrive(DEFAULT_LDRIVE) ) {
        return false;
    }
    if( interrupts ) {
        if( !setProximityIntEnable(1) ) {
            return false;
        }
    } else {
        if( !setProximityIntEnable(0) ) {
            return false;
        }
    }
    if( !enablePower() ){
        return false;
    }
    if( !setMode(PROXIMITY, 1) ) {
        return false;
    }
    
    return true;
}
    bool disableProximitySensor();


bool DFRobot_APDS9960::enableGestureSensor(bool interrupts)
{
    
    ///< 	 Enable gesture mode
    ///<   Set ENABLE to 0 (power off)
    ///<   Set WTIME to 0xFF
    ///<   Set AUX to LED_BOOST_300
    ///<   Enable PON, WEN, PEN, GEN in ENABLE 
    resetGestureParameters();
    if( !wireWriteDataByte(APDS9960_WTIME, 0xFF) ) {
        return false;
    }
    if( !wireWriteDataByte(APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE) ) {
        return false;
    }
    if( !setLEDBoost(LED_BOOST_300) ) {
        return false;
    }
    if( interrupts ) {
        if( !setGestureIntEnable(1) ) {
            return false;
        }
    } else {
        if( !setGestureIntEnable(0) ) {
            return false;
        }
    }
    if( !setGestureMode(1) ) {
        return false;
    }
    if( !enablePower() ){
        return false;
    }
    if( !setMode(WAIT, 1) ) {
        return false;
    }
    if( !setMode(PROXIMITY, 1) ) {
        return false;
    }
    if( !setMode(GESTURE, 1) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::disableGestureSensor()
{
    resetGestureParameters();
    if( !setGestureIntEnable(0) ) {
        return false;
    }
    if( !setGestureMode(0) ) {
        return false;
    }
    if( !setMode(GESTURE, 0) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::isGestureAvailable()
{
    uint8_t val;
    
    ///< Read value from GSTATUS register 
    if( !wireReadDataByte(APDS9960_GSTATUS, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out GVALID bit 
    val &= APDS9960_GVALID;
    
    ///< Return true/false based on GVALID bit
    if( val == 1) {
        return true;
    } else {
        return false;
    }
}


int DFRobot_APDS9960::readGesture()
{
    uint8_t fifo_level = 0;
    uint8_t bytes_read = 0;
    uint8_t fifo_data[128];
    uint8_t gstatus;
    int motion;
    int i;
    
    ///< Make sure that power and gesture is on and data is valid 
    if( !isGestureAvailable() || !(getMode() & 0b01000001) ) {
        return eDirNone;
    }
    
    ///< Keep looping as long as gesture data is valid 
    while(1) {
    
        ///< Wait some time to collect next batch of FIFO data
        delay(FIFO_PAUSE_TIME);
        
        ///< Get the contents of the STATUS register. Is data still valid?
        if( !wireReadDataByte(APDS9960_GSTATUS, gstatus) ) {
            return ERROR;
        }
        
        ///< If we have valid data, read in FIFO 
        if( (gstatus & APDS9960_GVALID) == APDS9960_GVALID ) {
        
            ///< Read the current FIFO level
            if( !wireReadDataByte(APDS9960_GFLVL, fifo_level) ) {
                return ERROR;
            }

#if DEBUG
            Serial.print("FIFO Level: ");
            Serial.println(fifo_level);
#endif

            ///< If there's stuff in the FIFO, read it into our data block 
            if( fifo_level > 0) {
                bytes_read = wireReadDataBlock(  APDS9960_GFIFO_U, 
                                                (uint8_t*)fifo_data, 
                                                (fifo_level * 4) );
                if( bytes_read == -1 ) {
                    return ERROR;
                }
#if DEBUG
                Serial.print("FIFO Dump: ");
                for ( i = 0; i < bytes_read; i++ ) {
                    Serial.print(fifo_data[i]);
                    Serial.print(" ");
                }
                Serial.println();
#endif

                ///< If at least 1 set of data, sort the data into U/D/L/R 
                if( bytes_read >= 4 ) {
                    for( i = 0; i < bytes_read; i += 4 ) {
                        gesture_data_.u_data[gesture_data_.index] = \
                                                            fifo_data[i + 0];
                        gesture_data_.d_data[gesture_data_.index] = \
                                                            fifo_data[i + 1];
                        gesture_data_.l_data[gesture_data_.index] = \
                                                            fifo_data[i + 2];
                        gesture_data_.r_data[gesture_data_.index] = \
                                                            fifo_data[i + 3];
                        gesture_data_.index++;
                        gesture_data_.total_gestures++;
                    }
                    
#if DEBUG
                Serial.print("Up Data: ");
                for ( i = 0; i < gesture_data_.total_gestures; i++ ) {
                    Serial.print(gesture_data_.u_data[i]);
                    Serial.print(" ");
                }
                Serial.println();
#endif

                    ///< Filter and process gesture data. Decode near/far state 
                    if( processGestureData() ) {
                        if( decodeGesture() ) {
                            //***TODO: U-Turn Gestures
#if DEBUG
                            ///<Serial.println(gesture_motion_);
#endif
                        }
                    }
                    
                    ///< Reset data 
                    gesture_data_.index = 0;
                    gesture_data_.total_gestures = 0;
                }
            }
        } else {
    
            ///< Determine best guessed gesture and clean up
            delay(FIFO_PAUSE_TIME);
            decodeGesture();
            motion = gesture_motion_;
#if DEBUG
            Serial.print("END: ");
            Serial.println(gesture_motion_);
#endif
            resetGestureParameters();
            return motion;
        }
    }
}


bool DFRobot_APDS9960::enablePower()
{
    if( !setMode(POWER, 1) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::disablePower()
{
    if( !setMode(POWER, 0) ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::readAmbientLight(uint16_t &val)
{
    uint8_t val_byte;
    val = 0;
    
    ///< Read value from clear channel, low byte register 
    if( !wireReadDataByte(APDS9960_CDATAL, val_byte) ) {
        return false;
    }
    val = val_byte;
    
    ///< Read value from clear channel, high byte register 
    if( !wireReadDataByte(APDS9960_CDATAH, val_byte) ) {
        return false;
    }
    val = val + ((uint16_t)val_byte << 8);
    
    return true;
}


bool DFRobot_APDS9960::readRedLight(uint16_t &val)
{
    uint8_t val_byte;
    val = 0;
    
    ///< Read value from clear channel, low byte register 
    if( !wireReadDataByte(APDS9960_RDATAL, val_byte) ) {
        return false;
    }
    val = val_byte;
    
    ///< Read value from clear channel, high byte register
    if( !wireReadDataByte(APDS9960_RDATAH, val_byte) ) {
        return false;
    }
    val = val + ((uint16_t)val_byte << 8);
    
    return true;
}
 

bool DFRobot_APDS9960::readGreenLight(uint16_t &val)
{
    uint8_t val_byte;
    val = 0;
    
    ///< Read value from clear channel, low byte register 
    if( !wireReadDataByte(APDS9960_GDATAL, val_byte) ) {
        return false;
    }
    val = val_byte;
    
    ///< Read value from clear channel, high byte register 
    if( !wireReadDataByte(APDS9960_GDATAH, val_byte) ) {
        return false;
    }
    val = val + ((uint16_t)val_byte << 8);
    
    return true;
}


bool DFRobot_APDS9960::readBlueLight(uint16_t &val)
{
    uint8_t val_byte;
    val = 0;
    
    ///< Read value from clear channel, low byte register 
    if( !wireReadDataByte(APDS9960_BDATAL, val_byte) ) {
        return false;
    }
    val = val_byte;
    
    ///< Read value from clear channel, high byte register
    if( !wireReadDataByte(APDS9960_BDATAH, val_byte) ) {
        return false;
    }
    val = val + ((uint16_t)val_byte << 8);
    
    return true;
}

bool DFRobot_APDS9960::readProximity(uint8_t &val)
{
    val = 0;
    
    ///< Read value from proximity data register
    if( !wireReadDataByte(APDS9960_PDATA, val) ) {
        return false;
    }
    
    return true;
}

void DFRobot_APDS9960::resetGestureParameters()
{
    gesture_data_.index = 0;
    gesture_data_.total_gestures = 0;
    
    gesture_ud_delta_ = 0;
    gesture_lr_delta_ = 0;
    
    gesture_ud_count_ = 0;
    gesture_lr_count_ = 0;
    
    gesture_near_count_ = 0;
    gesture_far_count_ = 0;
    
    gesture_state_ = 0;
    gesture_motion_ = eDirNone;
}


bool DFRobot_APDS9960::processGestureData()
{
    uint8_t u_first = 0;
    uint8_t d_first = 0;
    uint8_t l_first = 0;
    uint8_t r_first = 0;
    uint8_t u_last = 0;
    uint8_t d_last = 0;
    uint8_t l_last = 0;
    uint8_t r_last = 0;
    int ud_ratio_first;
    int lr_ratio_first;
    int ud_ratio_last;
    int lr_ratio_last;
    int ud_delta;
    int lr_delta;
    int i;

    ///< If we have less than 4 total gestures, that's not enough
    if( gesture_data_.total_gestures <= 4 ) {
        return false;
    }
    
    ///< Check to make sure our data isn't out of bounds
    if( (gesture_data_.total_gestures <= 32) && \
        (gesture_data_.total_gestures > 0) ) {
        
        ///< Find the first value in U/D/L/R above the threshold 
        for( i = 0; i < gesture_data_.total_gestures; i++ ) {
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_first = gesture_data_.u_data[i];
                d_first = gesture_data_.d_data[i];
                l_first = gesture_data_.l_data[i];
                r_first = gesture_data_.r_data[i];
                break;
            }
        }
        
        ///< If one of the _first values is 0, then there is no good data
        if( (u_first == 0) || (d_first == 0) || \
            (l_first == 0) || (r_first == 0) ) {
            
            return false;
        }
        ///< Find the last value in U/D/L/R above the threshold
        for( i = gesture_data_.total_gestures - 1; i >= 0; i-- ) {
#if DEBUG
            Serial.print(F("Finding last: "));
            Serial.print(F("U:"));
            Serial.print(gesture_data_.u_data[i]);
            Serial.print(F(" D:"));
            Serial.print(gesture_data_.d_data[i]);
            Serial.print(F(" L:"));
            Serial.print(gesture_data_.l_data[i]);
            Serial.print(F(" R:"));
            Serial.println(gesture_data_.r_data[i]);
#endif
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_last = gesture_data_.u_data[i];
                d_last = gesture_data_.d_data[i];
                l_last = gesture_data_.l_data[i];
                r_last = gesture_data_.r_data[i];
                break;
            }
        }
    }
    
    ///< Calculate the first vs. last ratio of up/down and left/right
    ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
    lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
    ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
    lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);
       
#if DEBUG
    Serial.print(F("Last Values: "));
    Serial.print(F("U:"));
    Serial.print(u_last);
    Serial.print(F(" D:"));
    Serial.print(d_last);
    Serial.print(F(" L:"));
    Serial.print(l_last);
    Serial.print(F(" R:"));
    Serial.println(r_last);

    Serial.print(F("Ratios: "));
    Serial.print(F("UD Fi: "));
    Serial.print(ud_ratio_first);
    Serial.print(F(" UD La: "));
    Serial.print(ud_ratio_last);
    Serial.print(F(" LR Fi: "));
    Serial.print(lr_ratio_first);
    Serial.print(F(" LR La: "));
    Serial.println(lr_ratio_last);
#endif
       
    ///< Determine the difference between the first and last ratios
    ud_delta = ud_ratio_last - ud_ratio_first;
    lr_delta = lr_ratio_last - lr_ratio_first;
    
#if DEBUG
    Serial.print("Deltas: ");
    Serial.print("UD: ");
    Serial.print(ud_delta);
    Serial.print(" LR: ");
    Serial.println(lr_delta);
#endif

    ///< Accumulate the UD and LR delta values
    gesture_ud_delta_ += ud_delta;
    gesture_lr_delta_ += lr_delta;
    
#if DEBUG
    Serial.print("Accumulations: ");
    Serial.print("UD: ");
    Serial.print(gesture_ud_delta_);
    Serial.print(" LR: ");
    Serial.println(gesture_lr_delta_);
#endif
    
    ///< Determine U/D gesture
    if( gesture_ud_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = 1;
    } else if( gesture_ud_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = -1;
    } else {
        gesture_ud_count_ = 0;
    }
    
    ///< Determine L/R gesture 
    if( gesture_lr_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = 1;
    } else if( gesture_lr_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = -1;
    } else {
        gesture_lr_count_ = 0;
    }
    
    ///< Determine Near/Far gesture 
    if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 0) ) {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
            
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            } else if( (ud_delta != 0) || (lr_delta != 0) ) {
                gesture_far_count_++;
            }
            
            if( (gesture_near_count_ >= 10) && (gesture_far_count_ >= 2) ) {
                if( (ud_delta == 0) && (lr_delta == 0) ) {
                    gesture_state_ = eNearState;
                } else if( (ud_delta != 0) && (lr_delta != 0) ) {
                    gesture_state_ = eFarState;
                }
                return true;
            }
        }
    } else {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
                
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            }
            
            if( gesture_near_count_ >= 10 ) {
                gesture_ud_count_ = 0;
                gesture_lr_count_ = 0;
                gesture_ud_delta_ = 0;
                gesture_lr_delta_ = 0;
            }
        }
    }
    
#if DEBUG
    Serial.print("UD_CT: ");
    Serial.print(gesture_ud_count_);
    Serial.print(" LR_CT: ");
    Serial.print(gesture_lr_count_);
    Serial.print(" NEAR_CT: ");
    Serial.print(gesture_near_count_);
    Serial.print(" FAR_CT: ");
    Serial.println(gesture_far_count_);
    Serial.println("----------");
#endif
    
    return false;
}


bool DFRobot_APDS9960::decodeGesture()
{
    ///< Return if near or far event is detected
    if( gesture_state_ == eNearState ) {
        gesture_motion_ = eDirNear;
        return true;
    } else if ( gesture_state_ == eFarState ) {
        gesture_motion_ = eDirFar;
        return true;
    }
    
    ///< Determine swipe direction
    if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = eDirRight;//DIR_UP;
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ =eDirLeft; ///<DIR_DOWN;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 1) ) {
        gesture_motion_ = eDirDown;//DIR_RIGHT;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == -1) ) {
        gesture_motion_ = eDirUp;//DIR_LEFT;
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ =eDirRight; ///<DIR_UP;
        } else {
            gesture_motion_ = eDirDown;//DIR_RIGHT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = eDirLeft;//DIR_DOWN;
        } else {
            gesture_motion_ = eDirUp;//DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ =eDirRight; ///<DIR_UP;
        } else {
            gesture_motion_ =eDirUp;// DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = eDirLeft;//DIR_DOWN;
        } else {
            gesture_motion_ = eDirDown;//DIR_RIGHT;
        }
    } else {
        return false;
    }
    
    return true;
}

uint8_t DFRobot_APDS9960::getProxIntLowThresh()
{
    uint8_t val;
    
    ///< Read value from PILT register
    if( !wireReadDataByte(APDS9960_PILT, val) ) {
        val = 0;
    }
    
    return val;
}


bool DFRobot_APDS9960::setProxIntLowThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getProxIntHighThresh()
{
    uint8_t val;
    
    ///< Read value from PIHT register
    if( !wireReadDataByte(APDS9960_PIHT, val) ) {
        val = 0;
    }
    
    return val;
}


bool DFRobot_APDS9960::setProxIntHighThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getLEDDrive()
{
    uint8_t val;
    
    ///< Read value from CONTROL register
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out LED drive bits
    val = (val >> 6) & 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    ///< Read value from CONTROL register
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value
    drive &= 0b00000011;
    drive = drive << 6;
    val &= 0b00111111;
    val |= drive;
    
    ///< Write register value back into CONTROL register
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getProximityGain()
{
    uint8_t val;
    
    ///< Read value from CONTROL register
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out PDRIVE bits 
    val = (val >> 2) & 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setProximityGain(uint8_t drive)
{
    uint8_t val;
    
    ///< Read value from CONTROL register 
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    drive &= 0b00000011;
    drive = drive << 2;
    val &= 0b11110011;
    val |= drive;
    
    ///< Write register value back into CONTROL register
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getAmbientLightGain()
{
    uint8_t val;
    
    ///< Read value from CONTROL register
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out ADRIVE bits 
    val &= 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setAmbientLightGain(uint8_t drive)
{
    uint8_t val;
    
    ///< Read value from CONTROL register
    if( !wireReadDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value
    drive &= 0b00000011;
    val &= 0b11111100;
    val |= drive;
    
    ///< Write register value back into CONTROL register
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getLEDBoost()
{
    uint8_t val;
    
    ///< Read value from CONFIG2 register 
    if( !wireReadDataByte(APDS9960_CONFIG2, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out LED_BOOST bits 
    val = (val >> 4) & 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setLEDBoost(uint8_t boost)
{
    uint8_t val;
    
    ///< Read value from CONFIG2 register 
    if( !wireReadDataByte(APDS9960_CONFIG2, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    boost &= 0b00000011;
    boost = boost << 4;
    val &= 0b11001111;
    val |= boost;
    
    ///< Write register value back into CONFIG2 register 
    if( !wireWriteDataByte(APDS9960_CONFIG2, val) ) {
        return false;
    }
    
    return true;
}    
   

uint8_t DFRobot_APDS9960::getProxGainCompEnable()
{
    uint8_t val;
    
    ///< Read value from CONFIG3 register 
    if( !wireReadDataByte(APDS9960_CONFIG3, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out PCMP bits 
    val = (val >> 5) & 0b00000001;
    
    return val;
}


 bool DFRobot_APDS9960::setProxGainCompEnable(uint8_t enable)
{
    uint8_t val;
    
    ///< Read value from CONFIG3 register 
    if( !wireReadDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    
    ///< Write register value back into CONFIG3 register 
    if( !wireWriteDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getProxPhotoMask()
{
    uint8_t val;
    
    ///< Read value from CONFIG3 register 
    if( !wireReadDataByte(APDS9960_CONFIG3, val) ) {
        return ERROR;
    }
    
    ///< Mask out photodiode enable mask bits 
    val &= 0b00001111;
    
    return val;
}


bool DFRobot_APDS9960::setProxPhotoMask(uint8_t mask)
{
    uint8_t val;
    
    ///< Read value from CONFIG3 register 
    if( !wireReadDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    mask &= 0b00001111;
    val &= 0b11110000;
    val |= mask;
    
    ///< Write register value back into CONFIG3 register 
    if( !wireWriteDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getGestureEnterThresh()
{
    uint8_t val;
    
    ///< Read value from GPENTH register 
    if( !wireReadDataByte(APDS9960_GPENTH, val) ) {
        val = 0;
    }
    
    return val;
}


bool DFRobot_APDS9960::setGestureEnterThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_GPENTH, threshold) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getGestureExitThresh()
{
    uint8_t val;
    
    ///< Read value from GEXTH register 
    if( !wireReadDataByte(APDS9960_GEXTH, val) ) {
        val = 0;
    }
    
    return val;
}


bool DFRobot_APDS9960::setGestureExitThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_GEXTH, threshold) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getGestureGain()
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out GGAIN bits 
    val = (val >> 5) & 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setGestureGain(uint8_t gain)
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value
    gain &= 0b00000011;
    gain = gain << 5;
    val &= 0b10011111;
    val |= gain;
    
    ///< Write register value back into GCONF2 register 
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getGestureLEDDrive()
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out GLDRIVE bits 
    val = (val >> 3) & 0b00000011;
    
    return val;
}


bool DFRobot_APDS9960::setGestureLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    drive &= 0b00000011;
    drive = drive << 3;
    val &= 0b11100111;
    val |= drive;
    
    ///< Write register value back into GCONF2 register 
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


uint8_t DFRobot_APDS9960::getGestureWaitTime()
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return ERROR;
    }
    
    ///< Mask out GWTIME bits 
    val &= 0b00000111;
    
    return val;
}


bool DFRobot_APDS9960::setGestureWaitTime(uint8_t time)
{
    uint8_t val;
    
    ///< Read value from GCONF2 register 
    if( !wireReadDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    time &= 0b00000111;
    val &= 0b11111000;
    val |= time;
    
    ///< Write register value back into GCONF2 register 
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::getLightIntLowThreshold(uint16_t &threshold)
{
    uint8_t val_byte;
    threshold = 0;
    
    ///< Read value from ambient light low threshold, low byte register 
    if( !wireReadDataByte(APDS9960_AILTL, val_byte) ) {
        return false;
    }
    threshold = val_byte;
    
    ///< Read value from ambient light low threshold, high byte register 
    if( !wireReadDataByte(APDS9960_AILTH, val_byte) ) {
        return false;
    }
    threshold = threshold + ((uint16_t)val_byte << 8);
    
    return true;
}


bool DFRobot_APDS9960::setLightIntLowThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    ///< Break 16-bit threshold into 2 8-bit values 
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    ///< Write low byte 
    if( !wireWriteDataByte(APDS9960_AILTL, val_low) ) {
        return false;
    }
    
    ///< Write high byte 
    if( !wireWriteDataByte(APDS9960_AILTH, val_high) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::getLightIntHighThreshold(uint16_t &threshold)
{
    uint8_t val_byte;
    threshold = 0;
    
    ///< Read value from ambient light high threshold, low byte register 
    if( !wireReadDataByte(APDS9960_AIHTL, val_byte) ) {
        return false;
    }
    threshold = val_byte;
    
    ///< Read value from ambient light high threshold, high byte register 
    if( !wireReadDataByte(APDS9960_AIHTH, val_byte) ) {
        return false;
    }
    threshold = threshold + ((uint16_t)val_byte << 8);
    
    return true;
}


bool DFRobot_APDS9960::setLightIntHighThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    ///< Break 16-bit threshold into 2 8-bit values 
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    ///< Write low byte 
    if( !wireWriteDataByte(APDS9960_AIHTL, val_low) ) {
        return false;
    }
    
    ///< Write high byte 
    if( !wireWriteDataByte(APDS9960_AIHTH, val_high) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::getProximityIntLowThreshold(uint8_t &threshold)
{
    threshold = 0;
    
    ///< Read value from proximity low threshold register 
    if( !wireReadDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::setProximityIntLowThreshold(uint8_t threshold)
{
    
    ///< Write threshold value to register 
    if( !wireWriteDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}
    

bool DFRobot_APDS9960::getProximityIntHighThreshold(uint8_t &threshold)
{
    threshold = 0;
    
    ///< Read value from proximity low threshold register 
    if( !wireReadDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}


bool DFRobot_APDS9960::setProximityIntHighThreshold(uint8_t threshold)
{
    
    ///< Write threshold value to register 
    if( !wireWriteDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}

uint8_t DFRobot_APDS9960::getAmbientLightIntEnable()
{
    uint8_t val;
    
    ///< Read value from ENABLE register 
    if( !wireReadDataByte(APDS9960_ENABLE, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out AIEN bit 
    val = (val >> 4) & 0b00000001;
    
    return val;
}

bool DFRobot_APDS9960::setAmbientLightIntEnable(uint8_t enable)
{
    uint8_t val;
    
    ///< Read value from ENABLE register 
    if( !wireReadDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    enable &= 0b00000001;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    
    ///< Write register value back into ENABLE register 
    if( !wireWriteDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    return true;
}

uint8_t DFRobot_APDS9960::getProximityIntEnable()
{
    uint8_t val;
    
    ///< Read value from ENABLE register 
    if( !wireReadDataByte(APDS9960_ENABLE, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out PIEN bit 
    val = (val >> 5) & 0b00000001;
    
    return val;
}

bool DFRobot_APDS9960::setProximityIntEnable(uint8_t enable)
{
    uint8_t val;
    
    ///< Read value from ENABLE register 
    if( !wireReadDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    
    ///< Write register value back into ENABLE register 
    if( !wireWriteDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    return true;
}

uint8_t DFRobot_APDS9960::getGestureIntEnable()
{
    uint8_t val;
    
    ///< Read value from GCONF4 register 
    if( !wireReadDataByte(APDS9960_GCONF4, val) ) {
        return ERROR;
    }
    
    ///< Shift and mask out GIEN bit 
    val = (val >> 1) & 0b00000001;
    
    return val;
}


bool DFRobot_APDS9960::setGestureIntEnable(uint8_t enable)
{
    uint8_t val;
    
    ///< Read value from GCONF4 register 
    if( !wireReadDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    enable &= 0b00000001;
    enable = enable << 1;
    val &= 0b11111101;
    val |= enable;
    
    ///< Write register value back into GCONF4 register 
    if( !wireWriteDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::clearAmbientLightInt()
{
    uint8_t throwaway;
    if( !wireReadDataByte(APDS9960_AICLEAR, throwaway) ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::clearProximityInt()
{
    uint8_t throwaway;
    if( !wireReadDataByte(APDS9960_PICLEAR, throwaway) ) {
        return false;
    }
    
    return true;
}

uint8_t DFRobot_APDS9960::getGestureMode()
{
    uint8_t val;
    
    ///< Read value from GCONF4 register 
    if( !wireReadDataByte(APDS9960_GCONF4, val) ) {
        return ERROR;
    }
    
    ///< Mask out GMODE bit 
    val &= 0b00000001;
    
    return val;
}

bool DFRobot_APDS9960::setGestureMode(uint8_t mode)
{
    uint8_t val;
    
    ///< Read value from GCONF4 register 
    if( !wireReadDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    ///< Set bits in register to given value 
    mode &= 0b00000001;
    val &= 0b11111110;
    val |= mode;
    
    ///< Write register value back into GCONF4 register 
    if( !wireWriteDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::wireWriteByte(uint8_t val)
{
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    
    return true;
}

bool DFRobot_APDS9960::wireWriteDataByte(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }

    return true;
}

bool DFRobot_APDS9960::wireWriteDataBlock(  uint8_t reg, 
                                        uint8_t *val, 
                                        unsigned int len)
{
    unsigned int i;

    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    for(i = 0; i < len; i++) {
        Wire.beginTransmission(val[i]);
    }
    if( Wire.endTransmission() != 0 ) {
        return false;
    }

    return true;
}

bool DFRobot_APDS9960::wireReadDataByte(uint8_t reg, uint8_t &val)
{
    
    ///< Indicate which register we want to read from 
    if (!wireWriteByte(reg)) {
        return false;
    }
    
    ///< Read from register 
    Wire.requestFrom(APDS9960_I2C_ADDR, 1);
    while (Wire.available()) {
        val = Wire.read();
    }

    return true;
}

int DFRobot_APDS9960::wireReadDataBlock(   uint8_t reg, 
                                        uint8_t *val, 
                                        unsigned int len)
{
    unsigned char i = 0;
    
    ///< Indicate which register we want to read from 
    if (!wireWriteByte(reg)) {
        return -1;
    }
    
    ///< Read block data 
    Wire.requestFrom(APDS9960_I2C_ADDR, len);
    while (Wire.available()) {
        if (i >= len) {
            return -1;
        }
        val[i] = Wire.read();
        i++;
    }

    return i;
}