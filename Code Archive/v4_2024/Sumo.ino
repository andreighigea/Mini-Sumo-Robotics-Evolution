#include <Wire.h>
#include <QTRSensors.h>
#include <Adafruit_VL53L0X.h>

#include "esp_system.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "Teo.h"

#include "Utilities/Delay.h"
#include "Utilities/MotorController.h"
#include "Utilities/TOFSensors.h"
#include "Utilities/LineSensors.h"

#include "RandomSearch.h"
#include "Strategy.h"

// ======== Config =========
const uint8_t TIMING_BUGET = 50;
const int PWM_FREQUENCY = 150000; // 150 kHz
const bool devMode = false;

const bool OutputTOF = false;
const bool OutputQTR = true;

const uint8_t speed = 255;
enum DebugMode mode = NORMAL;
// =========================


// ======== Logic ==========
bool matchStarted = false;
enum Strategies strategy;
// =========================

void setup(){
  // Initialize Serial
  Serial.begin(9600);

  // Dip Switch Setup
  pinMode(DIP1, INPUT_PULLDOWN);
  pinMode(DIP2, INPUT_PULLDOWN);
  pinMode(DIP3, INPUT_PULLDOWN);

  
  //analogWriteFrequency(MOTOR_A1, PWM_FREQUENCY);
  //analogWriteFrequency(MOTOR_A2, PWM_FREQUENCY);
  //analogWriteFrequency(MOTOR_B1, PWM_FREQUENCY);
  //analogWriteFrequency(MOTOR_B2, PWM_FREQUENCY);

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  
  /*
  ledcAttachChannel(MOTOR_A1, PWM_FREQUENCY, 8, 0); // Attach PWM channel to the pin
  ledcAttachChannel(MOTOR_A2, PWM_FREQUENCY, 8, 1); // Attach PWM channel to the pin
  ledcAttachChannel(MOTOR_B1, PWM_FREQUENCY, 8, 2); // Attach PWM channel to the pin
  ledcAttachChannel(MOTOR_B2, PWM_FREQUENCY, 8, 3); // Attach PWM channel to the pin
  */
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector

  // Start Module Setup
  pinMode(START, INPUT);

  // Start Wire Communication
  Wire.begin();
  //Wire.setClock(400000);

  // Initialize TOF sensors
  BeginTof(TIMING_BUGET);

  // Initialize line sensors
  BeginLineSensors();
}

void loop() {
  matchStarted = (bool) digitalRead(START); // Check if match started
  Strategies strategy = static_cast<Strategies>(digitalRead(DIP1) + digitalRead(DIP2) * 2 + digitalRead(DIP3) * 4); // Set strategy number

  ReadTofSensors(); // Read Time Of Flight sensors
  ReadLineSensors(); // Read QTR sensors

  // If OutputTOF, print tof sensor values
  if (OutputTOF){
    PrintTofSensors();
  }

  // If OutputQTR, print line sensor values
  if (OutputQTR){
    PrintLineSensors();
  }


  if (matchStarted || devMode){
    RunStrategy(strategy, mode);
  }
  else{
    MoveMotors(0,0,0,0); // Stop motors
  }
}