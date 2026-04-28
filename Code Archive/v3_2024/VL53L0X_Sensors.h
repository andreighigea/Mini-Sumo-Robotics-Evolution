#include <sys/_stdint.h>
#ifndef VL53L0X_Sensors_h
#define VL53L0X_Sensors_h

#include "Settings.h"
#include <Wire.h>
#include <Adafruit_VL53L0X.h>  // Include the Adafruit library

#define ERROR1 8191
#define ERROR2 65535
#define NUM_HISTORY 5

uint16_t distanceHistory[NUM_HISTORY] = {0};
int historyIndex = 0;

using namespace TOFSensorsSettings;
using namespace Teo;

uint16_t left;
uint16_t frontLeft;
uint16_t frontRight;
uint16_t right;
uint16_t back;

Adafruit_VL53L0X leftSensor     = Adafruit_VL53L0X();
Adafruit_VL53L0X frontLeftSensor  = Adafruit_VL53L0X();
Adafruit_VL53L0X frontRightSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X rightSensor      = Adafruit_VL53L0X();
Adafruit_VL53L0X backSensor       = Adafruit_VL53L0X();

void InitTOF(){
  bool initializedCorectly = true;

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(ALL_OFF_REG);
  Wire.endTransmission();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(LEFT_REG);
  Wire.endTransmission();

  for (int i = 0; i < 1; i++){
    if(leftSensor.begin(0x30)){
      leftSensor.startRangeContinuous(30);
      break;
    }
    else if (i == 4){
      initializedCorectly = false;
    }
  }

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(FRONT_LEFT_REG);
  Wire.endTransmission();

  for (int i = 0; i < 1; i++){
    if(frontLeftSensor.begin(0x31)){
      frontLeftSensor.startRangeContinuous(30);
      break;
    }
    else if (i == 4){
      initializedCorectly = false;
    }
  }

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(FRONT_RIGHT_REG);
  Wire.endTransmission();

  for (int i = 0; i < 1; i++){
    if(frontRightSensor.begin(0x32)){
      frontRightSensor.startRangeContinuous(30);
      break;
    }
    else if (i == 4){
      initializedCorectly = false;
    }
  }

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(RIGHT_REG);
  Wire.endTransmission();

  for (int i = 0; i < 1; i++){
    if(rightSensor.begin(0x33)){
      rightSensor.startRangeContinuous(30);
      break;
    }
    else if (i == 4){
      initializedCorectly = false;
    }
  }

  if (useBackSensor){
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(BACK_REG);
    Wire.endTransmission();

    for (int i = 0; i < 1; i++){
      if(backSensor.begin(0x34)){
        backSensor.startRangeContinuous(22);
        break;
      }
      else if (i == 4){
        initializedCorectly = false;
      }
    }
  }
}
uint16_t test;
void ReadTOF(){
  left = leftSensor.readRangeResult();
  frontLeft = frontLeftSensor.readRangeResult();
  frontRight = frontRightSensor.readRangeResult();
  test = rightSensor.readRangeResult();
  distanceHistory[historyIndex] = test;
  historyIndex = (historyIndex + 1) % NUM_HISTORY;

  // Check if previous 5 are not error values
  bool allValid = true;
  if (test < 39){
    allValid = false;
  }
  else if (test < 80){
    for (int i = 0; i < NUM_HISTORY; i++) {
      if (distanceHistory[i] == ERROR1 || distanceHistory[i] == ERROR2) {
        allValid = false;
        break;
      }
    }
  }
  

  // If current reading < 80 and previous 5 were valid → set to 80
  if (allValid) {
    right = test;
  }

  if (useBackSensor && touchedLine){
    back = leftSensor.readRangeResult();
  }
  else {
    back = 3000;
  }
}

#endif