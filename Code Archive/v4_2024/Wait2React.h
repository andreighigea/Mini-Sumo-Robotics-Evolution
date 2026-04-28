#ifndef Wait2React_h
#define Wait2React_h

// Include library for moving motors
#include "Utilities/MotorController.h"
#include "Utilities/Delay.h"


// ========================
namespace wait2React {
  const uint16_t WHITE_VALUE = 120;
  const uint8_t DISTANT = 600;
  const uint16_t CLOSE = 400;
  const uint16_t EXTREMLY_CLOSE = 200;

  Delay leftLineTimeout;
  Delay rightLineTimeout;
  Delay lineBackUpDelay;
  Delay lineReactDelay;
}
// =========================


// ========= Logic =========
extern uint16_t left;
extern uint16_t right;
extern uint16_t frontLeft;
extern uint16_t frontRight;

extern uint16_t leftLineSensor;
extern uint16_t rightLineSensor;
// ==========================


// Callable function to act as robot strategy
void Wait2React(){
  using namespace wait2React;


  if (leftLineSensor <= WHITE_VALUE && leftLineTimeout.IsOver()){
    leftLineTimeout.StartDelay(25);
  }

  if (rightLineSensor <= WHITE_VALUE && rightLineTimeout.IsOver()){
    rightLineTimeout.StartDelay(25);
  }

  if (!leftLineTimeout.IsOver() && !rightLineTimeout.IsOver()){
    MoveMotors(0, 170, 0, 170);
    delay(150);
    MoveMotors(170,0,0,170);
    lineReactDelay.StartDelay(175);
  }
  else if (leftLineTimeout.IsOver() && !rightLineTimeout.IsOver() && lineReactDelay.IsOver()){
    MoveMotors(0,170,0,120);
    lineReactDelay.StartDelay(250);
  }
  else if (!leftLineTimeout.IsOver() && rightLineTimeout.IsOver() && lineReactDelay.IsOver()){
    MoveMotors(0,120,0,170);
    lineReactDelay.StartDelay(250);
  }
  else if (lineReactDelay.IsOver()){
    MoveMotors(60,0,60,0);
  }
}

#endif