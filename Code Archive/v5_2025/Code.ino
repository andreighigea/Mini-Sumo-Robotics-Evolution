#define DECODE_RC5
#define DECODE_SONY
#define DECODE_NEC
#include <IRremote.hpp>

#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <SparkFun_VL53L5CX_Library.h> 

#include "esp_task_wdt.h"

Adafruit_VL53L0X backSensor    = Adafruit_VL53L0X();

#define CLOSE_DIST 550
#define VERY_CLOSE_DIST 250
#define EXTREMLY_CLOSE_DISTANCE 85

enum TOF_DISTANCE {
  FAR = 0,
  CLOSE,
  VERY_CLOSE,
  EXTREMLY_CLOSE
};

enum TOF_DISTANCE frontLeftTOF[4];
enum TOF_DISTANCE frontRightTOF[4];
enum TOF_DISTANCE leftTOF[4];
enum TOF_DISTANCE rightTOF[4];

enum TOF_DISTANCE frontLeftStatus;
enum TOF_DISTANCE frontRightStatus;
enum TOF_DISTANCE leftStatus;
enum TOF_DISTANCE rightStatus;

SparkFun_VL53L5CX TOF_FRONT_LEFT;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM

SparkFun_VL53L5CX TOF_RIGHT;
VL53L5CX_ResultsData measurementData1; // Result data class structure, 1356 byes of RAM

SparkFun_VL53L5CX TOF_LEFT;
VL53L5CX_ResultsData measurementData2; // Result data class structure, 1356 byes of RAM

SparkFun_VL53L5CX TOF_FRONT_RIGHT;
VL53L5CX_ResultsData measurementData3; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output


#include "EventManager.h"
EventManager events;
#include "MyDelay.h"
#include "MyTOF.h"

#include "driver/mcpwm.h"
#include "driver/adc.h"
#include "driver/i2c.h"


#define REMOTE_BLUE 0x24
#define REMOTE_RED 0x25
#define REMOTE_YELLOW 0x27
#define REMOTE_GREEN 0x26

namespace PinDefinitions {
  // IR sensors
  const uint8_t IR_FRONT = 15;
  const uint8_t IR_RIGHT = 39;
  const uint8_t IR_FRONT_RIGHT = 5;
  const uint8_t IR_FRONT_LEFT = 6;
  const uint8_t IR_LEFT = 7;

  // Line Sensors
  const uint8_t LINE_LEFT = 4;
  const uint8_t LINE_RIGHT = 1;

  // I2C
  const uint8_t SCL0 = 18;
  const uint8_t SDA0 = 17;
  const uint8_t SCL1 = 2;
  const uint8_t SDA1 = 43;

  // Motors
  const uint8_t MOTOR_LA = 3;
  const uint8_t MOTOR_LB = 42;
  const uint8_t MOTOR_LF = 44;
  const uint8_t MOTOR_RA = 47;
  const uint8_t MOTOR_RB = 21;
  const uint8_t MOTOR_RF = 13;

  // Servos
  const uint8_t SERVO_RIGHT = 11;
  const uint8_t SERVO_LEFT = 48;

  // Remote control
  const uint8_t START = 38;
  const uint8_t IR_RECEIVER = 40;

  // Dip
  const uint8_t DIPA1 = 46;
  const uint8_t DIPA2 = 45;

  // LEDs
  const uint8_t LED_RED = 8;
  const uint8_t LED_BLUE = 9;
  const uint8_t LED_GREEN = 38;

  // Misc
  const uint8_t LOGIC_BATTERY_PD = 12;
  const uint8_t MOTOR_BATTERY_PD = 14;
  const uint8_t SHUT_BACK = 10;
  const uint8_t SHUT_LEFT = 16;
  const uint8_t SHUT_RIGHT = 41;
}

namespace Setup{
  enum RemoteType{
    MODULE,
    NANOLABS,
    JSUMO
  };
  enum RemoteType remoteType = NANOLABS;
  
  //enum SensorType LeftSensorType = VL53L5CX;
  //enum SensorType RightSensorType = VL53L5CX;
  //enum SensorType FrontRightSensorType = VL53L5CX;
  //enum SensorType FrontLeftSensorType = VL53L5CX;
}
using namespace PinDefinitions;
using namespace Setup;

bool start = false;
bool stopped = false;
bool progmode = false;
int startCommand;
int stopCommand;

MyDelay sideDelay = MyDelay();
MyDelay frondelay = MyDelay();
MyDelay lineDelay = MyDelay();


#include "Flags.h"

void HandleRemote(){
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    if (remoteType == NANOLABS){
      if (IrReceiver.decodedIRData.protocol == RC5){
        Serial.print("Address: 0x");
        Serial.print(IrReceiver.decodedIRData.address,HEX);
        Serial.print("\tCommand: 0x");
        Serial.println(IrReceiver.decodedIRData.command, HEX);

        if (IrReceiver.decodedIRData.address == 0xB && progmode){
          progmode = false;
          startCommand = IrReceiver.decodedIRData.command + 1;
          stopCommand = IrReceiver.decodedIRData.command;
          for (int i = 0; i < 3; i++){
            digitalWrite(LED_BLUE, HIGH);
            digitalWrite(LED_RED, LOW);
            delay(125);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_RED, HIGH);
            delay(125);
          }
          digitalWrite(LED_BLUE, LOW);
          digitalWrite(LED_RED, LOW);
        }
        if (IrReceiver.decodedIRData.address == 0x7 && !progmode){
          if (IrReceiver.decodedIRData.command == startCommand){
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_GREEN, HIGH);
            start = true;
          }
          else if (IrReceiver.decodedIRData.command == stopCommand){
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_GREEN, LOW);
            stopped = true;
            start = false;
          }
        }
      }
      else if (IrReceiver.decodedIRData.protocol == SONY){
        Serial.print("Command: 0x");
        Serial.println(IrReceiver.decodedIRData.command, HEX);
        if (IrReceiver.decodedIRData.command == 0x3F){
          digitalWrite(LED_BLUE, HIGH);
          digitalWrite(LED_RED, HIGH);
          delay(250);
          digitalWrite(LED_BLUE, LOW);
          digitalWrite(LED_RED, LOW);
          delay(250);
          digitalWrite(LED_BLUE, HIGH);
          digitalWrite(LED_RED, HIGH);
          progmode = true;
          stopped = false;
        }
      }
    }
    else if (remoteType == JSUMO){
      ;
    }
    else if (remoteType == MODULE){
      start = (bool)digitalRead(START);
    }
  }
}

uint8_t status;
uint8_t dataReady;
int left;
int right;
uint sum = 0;
uint count;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_LA, OUTPUT);
  pinMode(MOTOR_LB, OUTPUT);

  pinMode(MOTOR_RA, OUTPUT);
  pinMode(MOTOR_RB, OUTPUT);

  pinMode(SERVO_LEFT, OUTPUT);
  pinMode(SERVO_RIGHT, OUTPUT);

  SetupServos();
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.printf("Reset reason: %d\n", reason);

  Serial.begin(9600);
  pinMode(SHUT_BACK, OUTPUT);
  pinMode(SHUT_LEFT, OUTPUT);
  pinMode(SHUT_RIGHT, OUTPUT);
  delay(100);

  Wire.read();

  //esp_task_wdt_init(5, true); // 5s timeout, panic on trigger
  //esp_task_wdt_add(NULL); // Add current thread

  Wire.setPins(SDA0, SCL0);
  Wire1.setPins(SDA1, SCL1);
  Wire.begin();
  Wire1.begin();
  Wire.setClock(1000000);
  Wire1.setClock(750000);

  IrReceiver.begin(IR_RECEIVER);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_FRONT_LEFT, INPUT);
  pinMode(IR_FRONT, INPUT);
  pinMode(IR_FRONT_RIGHT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  /*
  digitalWrite(SHUT_BACK, HIGH);
  delay(10);
  if(!backSensor.begin(0x32,false,&Wire,Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE)){
    while (true){
      Serial.println("Back sensor fail");
    }
  }
  else{
    backSensor.startRangeContinuous(22);
  }
  */
  
  //digitalWrite(SHUT_LEFT, HIGH);
  delay(100);

  Serial.println("before");
  
  long startTime = millis();
  bool startup /*= TOF_FRONT_LEFT.begin(0x29,Wire)*/;
  long stopTime = millis();
  /*
  if (startup == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }
  
  Serial.print("Firmware transfer time: ");
  float timeTaken = (stopTime - startTime) / 1000.0;
  Serial.print(timeTaken, 3);
  Serial.println("s");

  TOF_FRONT_LEFT.setResolution(4*4); //Enable all 64 pads
  TOF_FRONT_LEFT.setRangingFrequency(30);
  TOF_FRONT_LEFT.setRangingMode(SF_VL53L5CX_RANGING_MODE::CONTINUOUS);
  TOF_FRONT_LEFT.setAddress(0x30);
  TOF_FRONT_LEFT.startRanging();
  */
  
  pinMode(SHUT_RIGHT, INPUT_PULLUP);
  delay(10);
  
  startTime = millis();
  startup = TOF_RIGHT.begin(0x29,Wire);
  stopTime = millis();
  if (startup == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }

  Serial.print("Firmware transfer time: ");
  float timeTaken = (stopTime - startTime) / 1000.0;
  Serial.print(timeTaken, 3);
  Serial.println("s");

  TOF_RIGHT.setResolution(4*4); //Enable all 64 pads
  TOF_RIGHT.setRangingFrequency(60);
  TOF_RIGHT.setRangingMode(SF_VL53L5CX_RANGING_MODE::CONTINUOUS);\
  TOF_RIGHT.setAddress(0x31);
  TOF_RIGHT.startRanging();


  pinMode(SHUT_LEFT, INPUT_PULLUP);
  delay(10);

  startTime = millis();
  startup = TOF_LEFT.begin(0x29,Wire);
  stopTime = millis();
  if (startup == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }

  Serial.print("Firmware transfer time: ");
  timeTaken = (stopTime - startTime) / 1000.0;
  Serial.print(timeTaken, 3);
  Serial.println("s");

  TOF_LEFT.setResolution(4*4); //Enable all 64 pads
  TOF_LEFT.setRangingFrequency(60);
  TOF_LEFT.setRangingMode(SF_VL53L5CX_RANGING_MODE::CONTINUOUS);\

  TOF_LEFT.startRanging();
  //TOF_FRONT_LEFT.setAddress(0x32);

  startTime = millis();
  startup = TOF_FRONT_RIGHT.begin(0x29,Wire1);
  stopTime = millis();
  if (startup == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }

  Serial.print("Firmware transfer time: ");
  timeTaken = (stopTime - startTime) / 1000.0;
  Serial.print(timeTaken, 3);
  Serial.println("s");

  TOF_FRONT_RIGHT.setResolution(4*4); //Enable all 64 pads
  TOF_FRONT_RIGHT.setRangingFrequency(60);
  TOF_FRONT_RIGHT.setRangingMode(SF_VL53L5CX_RANGING_MODE::CONTINUOUS);\

  TOF_FRONT_RIGHT.startRanging();
  delay(500);

  if (remoteType == MODULE){
    pinMode(START, INPUT);
  }
  else{
    pinMode(LED_GREEN, OUTPUT);
  }
  
  events.on("Start", ReadStart);
}

bool oldstart = false;

void ReadStart(void* data){
  start = (bool)digitalRead(START);
}

void loop() {
  
  /*
  Serial.print(analogRead(LINE_LEFT));
  Serial.print("\t");
  Serial.print(analogRead(LINE_RIGHT));
  Serial.print("\n");
  */
  /*
  Serial.print(analogReadMilliVolts(IR_LEFT));
  Serial.print("\n");
  /*
  Serial.print(digitalRead(IR_FRONT_LEFT));
  Serial.print("\t");
  Serial.print(digitalRead(IR_FRONT));
  Serial.print("\t");
  Serial.print(digitalRead(IR_FRONT_RIGHT));
  Serial.print("\t");
  Serial.print(digitalRead(IR_RIGHT));
  Serial.print("\n");
  */
  /*
  events.emit("Start");
  Serial.println(start);
  */
  HandleRemote();
  /*/
  Serial.print("\nLeft: ");
  Serial.print(leftSensor.readRangeResult());
  Serial.print("\tRight: ");
  Serial.print(rightSensor.readRangeResult());
  Serial.print("\tBack: ");
  Serial.print(backSensor.readRangeResult());
  */
  //Poll sensor for new data
  
  if (false){
    TOF_FRONT_LEFT.getRangingData(&measurementData);
    frontLeftStatus = FAR;

    for (int x = 3 ; x >= 0 ; x--)
    {
      Serial.print("\t");
      //Serial.print(measurementData.distance_mm[x + y*imageWidth+1*imageWidth]);
      //Serial.print(measurementData.distance_mm[x+y*imageWidth+0*imageWidth] < 650 /*&& measurementData.target_status[x+y*imageWidth+0*imageWidth] == 5);
      int16_t data = measurementData.distance_mm[x+0*4];
      uint8_t status = measurementData.target_status[x+0*4];
      //Serial.print(measurementData.distance_mm[x+0*4]);
      if (status != 5 || data > CLOSE_DIST){
        frontLeftTOF[x] = FAR;
      }
      else if (data < EXTREMLY_CLOSE_DISTANCE){
        frontLeftTOF[x] = EXTREMLY_CLOSE;
        frontLeftStatus = EXTREMLY_CLOSE;
      }
      else if (data < VERY_CLOSE_DIST){
        frontLeftTOF[x] = VERY_CLOSE;
        if (frontLeftStatus < EXTREMLY_CLOSE){
          frontLeftStatus = VERY_CLOSE;
        }
      }
      else if (data < CLOSE_DIST){
        frontLeftTOF[x] = CLOSE;
        if (frontLeftStatus < VERY_CLOSE){
          frontLeftStatus = CLOSE;
        }
      }
      Serial.print(frontLeftTOF[x]);
    }
    Serial.print("\t\t");
  }
  

  if (TOF_FRONT_RIGHT.isDataReady()){
    TOF_FRONT_RIGHT.getRangingData(&measurementData1);
    frontRightStatus = FAR;

    for (int x = 3 ; x >= 0 ; x--)
    {
      Serial.print("\t");
      //Serial.print(measurementData.distance_mm[x + y*imageWidth+1*imageWidth]);
      //Serial.print(measurementData.distance_mm[x+y*imageWidth+0*imageWidth] < 650 /*&& measurementData.target_status[x+y*imageWidth+0*imageWidth] == 5);
      int16_t data = measurementData1.distance_mm[x+0*4];
      uint8_t status = measurementData1.target_status[x+0*4];
      //Serial.print(measurementData.distance_mm[x+0*4]);
      if (status != 5 || data > CLOSE_DIST){
        frontRightTOF[4-x] = FAR;
      }
      else if (data < EXTREMLY_CLOSE_DISTANCE){
        frontRightTOF[4-x] = EXTREMLY_CLOSE;
        frontRightStatus = EXTREMLY_CLOSE;
      }
      else if (data < VERY_CLOSE_DIST){
        frontRightTOF[4-x] = VERY_CLOSE;
        if (frontRightStatus < EXTREMLY_CLOSE){
          frontRightStatus = VERY_CLOSE;
        }
      }
      else if (data < CLOSE_DIST){
        frontRightTOF[4-x] = CLOSE;
        if (frontRightStatus < VERY_CLOSE){
          frontRightStatus = CLOSE;
        }
      }
      Serial.print((int)frontRightTOF[4-x]);
    }
    Serial.println();
  }

  if (TOF_LEFT.isDataReady()){
    TOF_LEFT.getRangingData(&measurementData2);
    leftStatus = FAR;

    for (int x = 3 ; x >= 0 ; x--)
    {
      //Serial.print("\t");
      //Serial.print(measurementData.distance_mm[x + y*imageWidth+1*imageWidth]);
      //Serial.print(measurementData.distance_mm[x+y*imageWidth+0*imageWidth] < 650 /*&& measurementData.target_status[x+y*imageWidth+0*imageWidth] == 5);
      int16_t data = measurementData2.distance_mm[x+3*4];
      uint8_t status = measurementData2.target_status[x+3*4];
      //Serial.print(measurementData.distance_mm[x+0*4]);
      if (status != 5 || data > CLOSE_DIST){
        leftTOF[x] = FAR;
      }
      else if (data < EXTREMLY_CLOSE_DISTANCE){
        leftTOF[x] = EXTREMLY_CLOSE;
        leftStatus = EXTREMLY_CLOSE;
      }
      else if (data < VERY_CLOSE_DIST){
        leftTOF[x] = VERY_CLOSE;
        if (leftStatus < EXTREMLY_CLOSE){
          leftStatus = VERY_CLOSE;
        }
      }
      else if (data < CLOSE_DIST){
        leftTOF[x] = CLOSE;
        if (leftStatus < VERY_CLOSE){
          leftStatus = CLOSE;
        }
      }
    }
  }

  if (TOF_RIGHT.isDataReady()){
    TOF_RIGHT.getRangingData(&measurementData3);
    rightStatus = FAR;

    for (int x = 3 ; x >= 0 ; x--)
    {
      //Serial.print("\t");
      //Serial.print(measurementData.distance_mm[x + y*imageWidth+1*imageWidth]);
      //Serial.print(measurementData.distance_mm[x+y*imageWidth+0*imageWidth] < 650 /*&& measurementData.target_status[x+y*imageWidth+0*imageWidth] == 5);
      int16_t data = measurementData3.distance_mm[x+3*4];
      uint8_t status = measurementData3.target_status[x+3*4];
      if (status != 5 || data > CLOSE_DIST){
        rightTOF[x] = FAR;
      }
      else if (data < EXTREMLY_CLOSE_DISTANCE){
        rightTOF[x] = EXTREMLY_CLOSE;
        rightStatus = EXTREMLY_CLOSE;
      }
      else if (data < VERY_CLOSE_DIST){
        rightTOF[x] = VERY_CLOSE;
        if (rightStatus < EXTREMLY_CLOSE){
          rightStatus = VERY_CLOSE;
        }
      }
      else if (data < CLOSE_DIST){
        rightTOF[x] = CLOSE;
        if (rightStatus < VERY_CLOSE){
          rightStatus = CLOSE;
        }
      }

      //Serial.print((int)rightTOF[x]);
    }
    //Serial.println();
  }


  //front 4/3
  //front right 2/1
  //front left
  //Serial.println(sum);
  /*
  sum = (sum << 1) & ((1U << 4) -1);
  //sum += digitalRead(IR_FRONT_LEFT);
  count = 0;
  for (int i = 0; i < 4; i++){
    count += (sum & (1<<i)) >> i;
  }
  digitalWrite(LED_GREEN, count>=3);
  */


  if ((analogReadMilliVolts(LOGIC_BATTERY_PD)/1000.0*1.398 < 3.5 && analogReadMilliVolts(LOGIC_BATTERY_PD)/1000 != 0) || (analogReadMilliVolts(MOTOR_BATTERY_PD)/1000.0 * 8.5 < 22.2 && analogReadMilliVolts(MOTOR_BATTERY_PD)/1000 != 0)){
    digitalWrite(LED_RED, HIGH);
  }
  //Serial.print(analogReadMilliVolts(MOTOR_BATTERY_PD)/1000.0*8.48);
  //Serial.println();
  if (oldstart != start && false){
    if (start){
      FlagsDown();
    }
    else{
      FlagsUp();
    }

    oldstart = start;
  }
  
  if (start){
    left = analogRead(LINE_LEFT);
    right = analogRead(LINE_RIGHT);
    if (left < 1000 && right < 1000){
      analogWrite(MOTOR_LA, 0);
      analogWrite(MOTOR_LB, 120);
      analogWrite(MOTOR_RA, 0);
      analogWrite(MOTOR_RB, 40);
      delay(300);
    }
    else if (left < 1000){
      analogWrite(MOTOR_LA, 0);
      analogWrite(MOTOR_LB, 50);
      analogWrite(MOTOR_RA, 0);
      analogWrite(MOTOR_RB, 160);
      delay(250);
    }
    else if (right < 1000){
      analogWrite(MOTOR_LA, 0);
      analogWrite(MOTOR_LB, 160);
      analogWrite(MOTOR_RA, 0);
      analogWrite(MOTOR_RB, 50);
      delay(250);
    }
    else if (frontRightStatus > 0){
      Serial.println("test");
      if (frontRightStatus > 2){
        analogWrite(MOTOR_LA, 127);
        analogWrite(MOTOR_LB, 127);
        analogWrite(MOTOR_RA, 127);
        analogWrite(MOTOR_RB, 127);
      }
      else if (frontRightStatus > 0){
        Serial.println("tezr");
        if (frontRightTOF[1] > 0 && frontRightTOF[2] > 0){
          analogWrite(MOTOR_LA, 160);
          analogWrite(MOTOR_LB, 160);
          analogWrite(MOTOR_RA, 160);
          analogWrite(MOTOR_RB, 160);
        }
        else if (frontRightTOF[3] > 0){
          if (frontRightTOF[2] > 0   ){
            analogWrite(MOTOR_LA, 140);
            analogWrite(MOTOR_LB, 0);
            analogWrite(MOTOR_RA, 80);
            analogWrite(MOTOR_RB, 0);
            delay(10);
          }
          else{
            analogWrite(MOTOR_LA, 140);
            analogWrite(MOTOR_LB, 0);
            analogWrite(MOTOR_RA, 50);
            analogWrite(MOTOR_RB, 0);
            delay(15);
          }
        }
        else if (frontRightTOF[0] > 0){
          if (frontRightTOF[1] > 0){
            analogWrite(MOTOR_LA, 80);
            analogWrite(MOTOR_LB, 0);
            analogWrite(MOTOR_RA, 140);
            analogWrite(MOTOR_RB, 0);
            delay(10);
          }
          else{
            analogWrite(MOTOR_LA, 50);
            analogWrite(MOTOR_LB, 0);
            analogWrite(MOTOR_RA, 140);
            analogWrite(MOTOR_RB, 0);
            delay(15);
          }
        }
      }
    }
    else{
      analogWrite(MOTOR_LA, 120);
      analogWrite(MOTOR_LB, 120);
      analogWrite(MOTOR_RA, 120);
      analogWrite(MOTOR_RB, 120);
    }
  }
  else{
    analogWrite(MOTOR_LA, 100);
    analogWrite(MOTOR_LB, 100);
    analogWrite(MOTOR_RA, 100);
    analogWrite(MOTOR_RB, 100);
  }
}
