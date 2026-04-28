#ifndef RandomSearch_h
#define RandomSearch_h

// Include library for moving motors
#include "Utilities/MotorController.h"
#include "Utilities/Delay.h"


// ========================
namespace randomSearch {
  const uint16_t WHITE_VALUE = 60;
  const uint16_t CLOSE = 400;
  const uint16_t EXTREMLY_CLOSE = 200;

  Delay moveDelay;
  Delay startDelay;
  Delay rotate;
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
void RandomSearch(){
  using namespace randomSearch;

  if (rightLineSensor <= WHITE_VALUE || leftLineSensor < WHITE_VALUE){
    if (rightLineSensor <= WHITE_VALUE && leftLineSensor <= WHITE_VALUE){
      MoveMotors(0,255,0,170);
      moveDelay.StartDelay(250);
    }
    else if (rightLineSensor < WHITE_VALUE){
      MoveMotors(0, 255/2, 0, 255/3); // Rotate backwards right
      moveDelay.StartDelay(200);
    }
    else if (leftLineSensor <= WHITE_VALUE){
      MoveMotors(0, 255/3, 0, 255/2);
      moveDelay.StartDelay(200);
    }
  }
  else if (frontRight <= CLOSE || frontLeft <= CLOSE){
    rotate.Interrupt();
    if (frontRight <= EXTREMLY_CLOSE || frontLeft <= EXTREMLY_CLOSE){
      if (frontRight-frontLeft > 25){
        // left extremly close
        MoveMotors(0,0,170,0);
      }
      else if (frontLeft-frontRight > 25){
        MoveMotors(170,0,0,0);
      }
      else{
        MoveMotors(170,0,170,0);
      }
    }
    else if (frontRight-frontLeft > 50){
      // left extremly close
      MoveMotors(70,0,150,0);
    }
    else if (frontLeft-frontRight > 50){
      MoveMotors(150,0,70,0);
    }
    else{
      MoveMotors(160,0,160,0);
    }
  }
  else if (left <= CLOSE && rotate.IsOver()){
    MoveMotors(0,0,170,0);
    rotate.StartDelay(200);
  }
  else if (right <= CLOSE && rotate.IsOver()){
    MoveMotors(170,0,0,0);
    rotate.StartDelay(200);
  }
  else if (moveDelay.IsOver() && rotate.IsOver()){
    MoveMotors(80, 0, 80, 0); // Move forward
  }
}

#endif