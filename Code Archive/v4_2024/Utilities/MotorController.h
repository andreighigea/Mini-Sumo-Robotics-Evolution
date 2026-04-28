#include "esp32-hal.h"
#ifndef MotorController_h
#define MotorController_h

#include <Arduino.h>

extern const uint8_t MOTOR_A1;
extern const uint8_t MOTOR_A2;
extern const uint8_t MOTOR_B1;
extern const uint8_t MOTOR_B2;

void MoveMotors(uint8_t pwmA1, uint8_t pwmA2, uint8_t pwmB1, uint8_t pwmB2){
  
  analogWrite(MOTOR_A1, pwmA1);
  analogWrite(MOTOR_A2, pwmA2);

  analogWrite(MOTOR_B1, pwmB1);
  analogWrite(MOTOR_B2, pwmB2);
  
  /*
  ledcWrite(MOTOR_A1, pwmA1);
  ledcWrite(MOTOR_B1, pwmB1);
  ledcWrite(MOTOR_B2, pwmB2);
  ledcWrite(MOTOR_A2, pwmA2);
  */

  
}

#endif