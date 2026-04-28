#ifndef TOFSensors_h
#define TOFSensors_h

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define PCAADDR 0x70

Adafruit_VL53L0X leftSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X rightSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X frontRightSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X frontLeftSensor = Adafruit_VL53L0X();

extern const uint8_t leftAddr;
extern const uint8_t rightAddr;
extern const uint8_t frontLeftAddr;
extern const uint8_t frontRightAddr;

uint16_t left;
uint16_t right;
uint16_t frontLeft;
uint16_t frontRight;

void pcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void BeginTof(uint8_t buget){
  pcaselect(leftAddr);
  leftSensor.begin();
  leftSensor.startRangeContinuous(buget);

  pcaselect(rightAddr);
  rightSensor.begin();
  rightSensor.startRangeContinuous(buget);

  pcaselect(frontLeftAddr);
  frontLeftSensor.begin();
  frontLeftSensor.startRangeContinuous(buget);

  pcaselect(frontRightAddr);
  frontRightSensor.begin();
  frontRightSensor.startRangeContinuous(buget);
}

void ReadTofSensors(){
  pcaselect(leftAddr);
  left = leftSensor.readRangeResult();

  pcaselect(rightAddr);
  right = rightSensor.readRangeResult();
  
  pcaselect(frontLeftAddr);
  frontLeft = frontLeftSensor.readRangeResult();  
  
  pcaselect(frontRightAddr);
  frontRight = frontRightSensor.readRangeResult();
}

void PrintTofSensors(){
  Serial.print("Left: ");
  Serial.print(left);
  Serial.print("\t");
  
  Serial.print("Right: ");
  Serial.print(right);
  Serial.print("\t");

  Serial.print("Front left: ");
  Serial.print(frontLeft);
  Serial.print("\t");
  
  Serial.print("Front right: ");
  Serial.print(frontRight);
  Serial.print("\n");
}

#endif