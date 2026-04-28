#include "esp32-hal-gpio.h"
#include "Arduino.h"
#ifndef ESP32S3_h
#define ESP32S3_h

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define EXTRA_1 1
#define EXTRA_2 13

#define START_MODULE 3

#define LEFT_LINE 10
#define RIGHT_LINE 4

#define LEFT_MOTOR_A 8
#define LEFT_MOTOR_B 9
#define LEFT_MOTOR_FB 7

#define RIGHT_MOTOR_A 5
#define RIGHT_MOTOR_B 6
#define RIGHT_MOTOR_FB 2

#define TOF_ADDRESS 0x30

#define SDA 12
#define SCL 11

class MyDelay{
public:
  MyDelay(){
    _start = 0;
    _end = 0;
  }

  void StartTimer(int duration){
    _start = (int)millis();
    _end = _start + duration;
  }

  bool IsOver(){
    return (int)millis() > _end;
  }

  void Interrupt(){
    _end = (int)millis();
  }

private:
  int _start;
  int _end;
};

MyDelay lineDelay = MyDelay();
MyDelay backDelay = MyDelay();
MyDelay sideDelay = MyDelay();

const uint8_t DIP_SW1 = TX;
const uint8_t DIP_SW2 = RX;

uint16_t left;
uint16_t frontLeft;
uint16_t frontRight;
uint16_t right;
uint16_t back;

uint64_t IrCommand = 0;
uint8_t DipCommand;

uint16_t lineLeft;
uint16_t lineRight;
String lineDet = "A";

uint8_t strategy = 0;

Adafruit_VL53L0X leftSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X frontLeftSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X frontRightSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X rightSensor = Adafruit_VL53L0X();
//Adafruit_VL53L0X backSensor = Adafruit_VL53L0X();

void setup() {
Serial.begin(9600);
Wire.setPins(SDA,SCL);
//Wire.setClock(400000);
Wire.begin();
bool initializedCorectly = false;

pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(9, OUTPUT);
pinMode(8, OUTPUT);

if ((bool)digitalRead(START_MODULE)){
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(8, 0);
  analogWrite(9, 100);
}

Serial.println("test");
  initializedCorectly = false;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(ALL_OFF_REG);
  Wire.endTransmission();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(LEFT_REG);
  Wire.endTransmission();

  if(!leftSensor.begin(0x30)){
    initializedCorectly = false;
  }
  leftSensor.startRangeContinuous(22);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(FRONT_LEFT_REG);
  Wire.endTransmission();

  if(!frontLeftSensor.begin(0x31)){
    initializedCorectly = false;
  }
  frontLeftSensor.startRangeContinuous(22);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(FRONT_RIGHT_REG);
  Wire.endTransmission();

  if(!frontRightSensor.begin(0x32)){
    initializedCorectly = false;
  }
  frontRightSensor.startRangeContinuous(22);

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(RIGHT_REG);
  Wire.endTransmission();

  if(!rightSensor.begin(0x33)){
    initializedCorectly = false;
  }
  rightSensor.startRangeContinuous(22);
  /*
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(BACK_REG);
  Wire.endTransmission();

  if(!backSensor.begin(0x34)){
    initializedCorectly = false;
  }
  backSensor.startRangeContinuous(22);
  */

pinMode(10, INPUT);
pinMode(4, INPUT);

pinMode(DIP_SW1, INPUT_PULLDOWN);
pinMode(DIP_SW2, INPUT_PULLDOWN);

pinMode(3, INPUT);
}

void loop() {

left = leftSensor.readRangeResult();
frontLeft = frontLeftSensor.readRangeResult();
frontRight = frontRightSensor.readRangeResult();
right = rightSensor.readRangeResult();
back = 3000;

lineLeft = analogRead(LEFT_LINE);
lineRight = analogRead(RIGHT_LINE);


Serial.print("left: ");
Serial.print(left);
Serial.print("\tfront left: ");
Serial.print(frontLeft);
Serial.print("\tfront right: ");
Serial.print(frontRight);
Serial.print("\tright: ");
Serial.print(right);
Serial.print("\tback: ");
Serial.print(back);

Serial.println("");

/*
Serial.print(lineLeft);
Serial.print("\t");
Serial.print(lineRight);
Serial.println("");
*/

if ((bool)digitalRead(START_MODULE)){
  if ((lineLeft <= 300 || lineRight <= 300) /*&& lineDelay.IsOver() && backDelay.IsOver() && sideDelay.IsOver()*/){
    if (lineLeft <= 300 && lineRight <= 300){
      analogWrite(5, 0);
      analogWrite(6, 75);
      analogWrite(8, 0);
      analogWrite(9, 100);
      delay(500);
      lineDet = "A";
    }
    else if (lineLeft <= 300){
      analogWrite(5, 0);
      analogWrite(6, 100);
      analogWrite(8, 0);
      analogWrite(9, 75);
      delay(400);
      lineDet = "L";
    }
    else{
      analogWrite(5, 0);
      analogWrite(6, 75);
      analogWrite(8, 0);
      analogWrite(9, 100);
      delay(400);
      lineDet = "R";
    }
  }/*
  else if (back <= 300 && backDelay.IsOver()){
    if (!lineDelay.IsOver()){
      if (back <= 100){
        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(8, 0);
        analogWrite(9, 255);
        backDelay.StartTimer(300);
      }
      else if (lineDet == "L"){
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(8, 0);
        analogWrite(9, 200);
        backDelay.StartTimer(300);
      }
      else if (lineDet == "R"){
        analogWrite(5, 0);
        analogWrite(6, 200);
        analogWrite(8, 0);
        analogWrite(9, 0);
        backDelay.StartTimer(300);
      }
      else if (lineDet = "A"){
        analogWrite(5, 0);
        analogWrite(6, 200);
        analogWrite(8, 0);
        analogWrite(9, 150);
        backDelay.StartTimer(300);
      }
    }
    else{
      if (back <= 100){
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(8, 200);
        analogWrite(9, 0);
        backDelay.StartTimer(300);
      }
      else{
        analogWrite(5, 75);
        analogWrite(6, 0);
        analogWrite(8, 200);
        analogWrite(9, 0);
        backDelay.StartTimer(300);
      }
    }
    lineDelay.Interrupt();
    sideDelay.Interrupt();
  }*//*
  else if ((left <= 350 || right <= 350) && sideDelay.IsOver()){
    if (!lineDelay.IsOver()){
      if (lineDet == "L" && right <= 350){
        if (right <= 150){
          analogWrite(5, 0);
          analogWrite(6, 200);
          analogWrite(8, 0);
          analogWrite(9, 0);
          sideDelay.StartTimer(300);
        }
        else{
          analogWrite(5, 0);
          analogWrite(6, 0);
          analogWrite(8, 0);
          analogWrite(9, 200);
          sideDelay.StartTimer(300);
        }
      }
      else if (lineDet = "R"  && left <= 350){
        if (left <= 150){
          analogWrite(5, 0);
          analogWrite(6, 0);
          analogWrite(8, 0);
          analogWrite(9, 200);
          sideDelay.StartTimer(300);
        }
        else{
          analogWrite(5, 0);
          analogWrite(6, 200);
          analogWrite(8, 0);
          analogWrite(9, 0);
          sideDelay.StartTimer(300);
        }
      }
    }
    else{
      if (left <= 350){
        if (left <= 150){
          analogWrite(5, 0);
          analogWrite(6, 0);
          analogWrite(8, 200);
          analogWrite(9, 0);
          sideDelay.StartTimer(300);
        }
        else{
          analogWrite(5, 0);
          analogWrite(6, 150);
          analogWrite(8, 150);
          analogWrite(9, 0);
          sideDelay.StartTimer(300);
        }
      }
      else{
        if (right <= 150){
          analogWrite(5, 200);
          analogWrite(6, 0);
          analogWrite(8, 0);
          analogWrite(9, 0);
          sideDelay.StartTimer(300);
        }
        else{
          analogWrite(5, 150);
          analogWrite(6, 0);
          analogWrite(8, 0);
          analogWrite(9, 150);
          sideDelay.StartTimer(300);
        }
      }
    }
    backDelay.Interrupt();
    lineDelay.Interrupt();
  }
  else if (lineDelay.IsOver()){
    if (frontLeft <= 350 && frontRight <= 350){
      if (abs(frontLeft - frontRight) < 50){
        if (frontLeft <= 100 || frontRight <= 100){
          analogWrite(5, 200);
          analogWrite(6, 0);
          analogWrite(8, 200);
          analogWrite(9, 0);
        }
        else{
          analogWrite(5, 150);
          analogWrite(6, 0);
          analogWrite(8, 150);
          analogWrite(9, 0);
        }
      }
      else if (frontLeft - frontRight > 50){
        analogWrite(5, 100);
        analogWrite(6, 0);
        analogWrite(8, 150);
        analogWrite(9, 0);
      }
      else{
        analogWrite(5, 150);
        analogWrite(6, 0);
        analogWrite(8, 100);
        analogWrite(9, 0);
      }
    }
    else if (frontLeft <= 350){
      analogWrite(5, 75);
      analogWrite(6, 0);
      analogWrite(8, 150);
      analogWrite(9, 0);
    }
    else if (frontRight <= 350){
      analogWrite(5, 150);
      analogWrite(6, 0);
      analogWrite(8, 75);
      analogWrite(9, 0);
    }
    else{
      analogWrite(5, 75);
      analogWrite(6, 0);
      analogWrite(8, 75);
      analogWrite(9, 0);
    }

    backDelay.Interrupt();
    sideDelay.Interrupt();
  }
  */
  else if (frontRight <= 300 && frontLeft <= 300){
    analogWrite(5, 255);
    analogWrite(6, 0);
    analogWrite(8, 255);
    analogWrite(9, 0);
  }
  else{
    analogWrite(5, 75);
    analogWrite(6, 0);
    analogWrite(8, 75);
    analogWrite(9, 0);
  }
  
}
else{
  analogWrite(5, 255);
  analogWrite(6, 255);
  analogWrite(8, 255);
  analogWrite(9, 255);
  /*
  uint8_t buffer[9];

  Wire.requestFrom(I2C_ADDRESS, sizeof(buffer));  // Request 9 bytes

  if (Wire.available() == 9) {
    // Read buffer
    for (int i = 0; i < 9; i++) {
        buffer[i] = Wire.read();
    }

    // Check if IR code has changed
    if (buffer[0] == 0x00) {
        //Serial.println("No IR Code Change");
    } else {
        // Reconstruct IR code from buffer
        IrCommand = 0;
        for (int i = 1; i < 9; i++) {
            IrCommand |= ((uint64_t)buffer[i] << ((i - 1) * 8));
        }

        //Serial.print("New IR Code: 0x");
        //Serial.println(IrCommand, HEX);
    }
    
  }
  */
  
}

//Serial.println("ESP32 S3");
}
#endif