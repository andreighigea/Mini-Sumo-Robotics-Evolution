#ifndef Kamikaze_h
#define Kamikaze_h

// Include library for moving motors
#include "Utilities/MotorController.h"
#include "Utilities/Delay.h"

// ========================
namespace kamikaze {
  const uint16_t WHITE_VALUE = 60;
  const uint16_t DISTANT = 600;
  const uint16_t CLOSE = 350;
  const uint16_t EXTREMLY_CLOSE = 150;

  const uint8_t PARALLEL_MARGIN = 20;
  const uint8_t PARALLEL_ATTENUATION_MARGIN = 75;

  Delay brakeDelay;
  Delay moveDelay;
  Delay startDelay;
  Delay rotate;
}
// =========================
bool once = true;

// ========= Logic =========
extern uint16_t left;
extern uint16_t right;
extern uint16_t frontLeft;
extern uint16_t frontRight;

extern uint16_t leftLineSensor;
extern uint16_t rightLineSensor;
// ==========================


// Callable function to act as robot strategy
void Kamikaze(){
  using namespace kamikaze;
  
  /*
  if ((rightLineSensor <= WHITE_VALUE || leftLineSensor < WHITE_VALUE)){
    if (rightLineSensor <= WHITE_VALUE && leftLineSensor <= WHITE_VALUE){
      MoveMotors(0,255,0,255);
      delay(125);
      MoveMotors(0, 255, 140, 0);
      moveDelay.StartDelay(200);
    }
    else if (rightLineSensor < WHITE_VALUE){
      MoveMotors(0, 255, 0, 140); // Rotate backwards right
      moveDelay.StartDelay(150);
    }
    else if (leftLineSensor <= WHITE_VALUE){
      MoveMotors(0, 140, 0, 255);
      moveDelay.StartDelay(140);
    }
  }
  */
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
      if (abs(frontRight-frontLeft) < PARALLEL_MARGIN){
        MoveMotors(255,0,255,0);
      }
      else if (frontRight-frontLeft > PARALLEL_MARGIN){
        MoveMotors(0,0,constrain(200*(frontRight-frontLeft)/PARALLEL_ATTENUATION_MARGIN,0,200),0);
      }
      else if (frontLeft-frontRight > PARALLEL_MARGIN){
        MoveMotors(constrain(200*(frontLeft-frontRight)/PARALLEL_ATTENUATION_MARGIN,0,200),0,0,0);
      }
    }
    else if (abs(frontRight-frontLeft) < PARALLEL_MARGIN){
      MoveMotors(200,0,200,0);
    }
    else if (frontRight-frontLeft > PARALLEL_MARGIN){
      MoveMotors(100,0,constrain(200*(frontRight-frontLeft)/PARALLEL_ATTENUATION_MARGIN,0,200),0);
    }
    else if (frontLeft-frontRight > PARALLEL_MARGIN){
      MoveMotors(constrain(200*(frontLeft-frontRight)/PARALLEL_ATTENUATION_MARGIN,0,200),0,100,0);
    }
  }
  else if (left <= CLOSE && rotate.IsOver()){
    MoveMotors(0,0,200,0);
    rotate.StartDelay(180);
  }
  else if (right <= CLOSE && rotate.IsOver()){
    MoveMotors(200,0,0,0);
    rotate.StartDelay(180);
  }
  else if (moveDelay.IsOver() && rotate.IsOver()){
    MoveMotors(120, 0, 120, 0); // Move forward
  }
}

#endif