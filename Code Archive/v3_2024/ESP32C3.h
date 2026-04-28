#include "esp32-hal.h"
#include <sys/_stdint.h>
#include "esp32-hal-gpio.h"
#ifndef ESP32C3_h
#define ESP32C3_h

#include <Wire.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define FRONT_RIGHT_SENSOR_XSHUT 20
#define RIGHT_SENSOR_XSHUT 21
#define FRONT_LEFT_SENSOR_XSHUT 5
#define LEFT_SENSOR_XSHUT 6
#define BACK_SENSOR_XSHUT 3


#define IR_SENSOR 10

const uint8_t DIP_SW1 = 1;
const uint8_t DIP_SW2 = 2;

IRrecv irrecv(IR_SENSOR);

decode_results results;
uint64_t result;

uint8_t dip;
uint8_t oldDip = 255;

bool hasChanged = false;

void SelectSensor(int bytes){
  if (bytes == 1){
    int val = (int) Wire.read();
    switch (val){
      case LEFT_REG:
        digitalWrite(LEFT_SENSOR_XSHUT, HIGH);
        digitalWrite(RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_LEFT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(BACK_SENSOR_XSHUT, LOW);
        break;
      case FRONT_LEFT_REG:
        digitalWrite(RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_LEFT_SENSOR_XSHUT, HIGH);
        digitalWrite(FRONT_RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(BACK_SENSOR_XSHUT, LOW);
        break;
      case FRONT_RIGHT_REG:
        digitalWrite(RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_RIGHT_SENSOR_XSHUT, HIGH);
        digitalWrite(BACK_SENSOR_XSHUT, LOW);
        break;
      case RIGHT_REG:
        digitalWrite(RIGHT_SENSOR_XSHUT, HIGH);
        digitalWrite(BACK_SENSOR_XSHUT, LOW);
        break;
      case BACK_REG:
        digitalWrite(BACK_SENSOR_XSHUT, HIGH);
        break;
      case ALL_ON_REG:
        digitalWrite(LEFT_SENSOR_XSHUT, HIGH);
        digitalWrite(RIGHT_SENSOR_XSHUT,HIGH);
        digitalWrite(FRONT_LEFT_SENSOR_XSHUT, HIGH);
        digitalWrite(FRONT_RIGHT_SENSOR_XSHUT, HIGH);
        digitalWrite(BACK_SENSOR_XSHUT, HIGH);
        break;
      case ALL_OFF_REG:
        digitalWrite(LEFT_SENSOR_XSHUT, LOW);
        digitalWrite(RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_LEFT_SENSOR_XSHUT, LOW);
        digitalWrite(FRONT_RIGHT_SENSOR_XSHUT, LOW);
        digitalWrite(BACK_SENSOR_XSHUT, LOW);
        break;
      default:
        Serial.println("Wrong Register");
        break;
    }
  }
}/*
void requestEvent() {
    uint8_t buffer[9];  // 8 bytes for IR code + 1 byte for pin states
    if (hasChanged){
      // Copy IR code into buffer (Little Endian format)
      for (int i = 0; i < 8; i++) {
          buffer[i] = (result >> (i * 8)) & 0xFF;
      }
    }
    else{
      for (int i = 0; i < 8; i++) {
          buffer[i] = 0;
      }
    }
    // Pack the two pin states into one byte (bitwise operations)
    buffer[8] = dip; // Store packed pin states

    // Send buffer to master
    Wire.write(buffer, sizeof(buffer));
}
*/
void setup() {
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(SelectSensor);
  //Wire.onRequest(requestEvent);

  pinMode(LEFT_SENSOR_XSHUT, OUTPUT);
  pinMode(FRONT_LEFT_SENSOR_XSHUT, OUTPUT);
  pinMode(FRONT_RIGHT_SENSOR_XSHUT, OUTPUT);
  pinMode(RIGHT_SENSOR_XSHUT, OUTPUT);
  pinMode(BACK_SENSOR_XSHUT, OUTPUT);

  pinMode(DIP_SW1, INPUT);
  pinMode(DIP_SW2, INPUT);
  //irrecv.enableIRIn(true);
  Serial.begin(9600);
}

void loop() {
  /*
  hasChanged = false;
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    if (results.value != 0xFFFFFFFFFFFFFFFF){
      serialPrintUint64(results.value, HEX);
      Serial.println("");
      result = results.value;
      hasChanged = true;
    }
    irrecv.resume();  // Receive the next value
  }
  dip = digitalRead(DIP_SW1) + digitalRead(DIP_SW2) * 2;
  if (dip != oldDip){
    hasChanged = true;
  }
  */
}

#endif