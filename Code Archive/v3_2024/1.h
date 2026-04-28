#ifndef Strat1_h
#define Strat1_h
#include "Settings.h"
using namespace Teo;
using namespace TOFSensorsSettings;

bool stop;

void Strat1(){
  if ((lineLeft <= 300 && lineRight <= 300) && !lineDelay.IsOver()){
    if (dip == 0){
      if (back < backVeryClose){
        ledcWrite(4, 0);
        ledcWrite(5, 255);
        ledcWrite(6, 0);
        ledcWrite(7, 255);
        delay(200);
      }
      else if (back < backClose){
        ledcWrite(4, 0);
        ledcWrite(5, 200);
        ledcWrite(6, 0);
        ledcWrite(7, 200);
        delay(300);
        ledcWrite(4, 200);
        ledcWrite(5, 0);
        ledcWrite(6, 0);
        ledcWrite(7, 200);
        //lineDelay.StartTimer(80);
        lineDelay.StartTimer(200);
      }
      else {
        ledcWrite(4, 0);
        ledcWrite(5, 200);
        ledcWrite(6, 0);
        ledcWrite(7, 200);
        delay(300);
        ledcWrite(4, 200);
        ledcWrite(5, 0);
        ledcWrite(6, 0);
        ledcWrite(7, 200);
        //lineDelay.StartTimer(125);
        lineDelay.StartTimer(400);
      }
      lineDet = "A";
    }
  }
  else if ((lineLeft <= 300 || lineRight <= 300) && lineDelay.IsOver() /*&& backDelay.IsOver() && sideDelay.IsOver()*/){
    touchedLine = true;
    if (lineLeft <= 300){
      ledcWrite(4, 0);
      ledcWrite(5, 220);
      ledcWrite(6, 0);
      ledcWrite(7, 130);
      lineDelay.StartTimer(400);
      lineDet = "L";
    }
    else{
      ledcWrite(4, 0);
      ledcWrite(5, 160);
      ledcWrite(6, 0);
      ledcWrite(7, 230);
      lineDelay.StartTimer(400);
      lineDet = "R";
    }

    lineLeft = analogRead(LEFT_LINE);
    lineRight = analogRead(RIGHT_LINE);
  }
  else if (frontRight < 400){
    if (frontRight < 100){
      ledcWrite(4, 255);
      ledcWrite(5, 0);
      ledcWrite(6, 255);
      ledcWrite(7, 0);
    }else{
      ledcWrite(4, 160);
      ledcWrite(5, 0);
      ledcWrite(6, 160);
      ledcWrite(7, 0);
    }
  }
  else if (right < 500){
    if (right < 100){
      ledcWrite(4, 0);
      ledcWrite(5, 200);
      ledcWrite(6, 200);
      ledcWrite(7, 0);
      delay ( 150);
    }
    else if (right < 500){
      ledcWrite(4, 100);
      ledcWrite(5, 0);
      ledcWrite(6, 200);
      ledcWrite(7, 0);
      delay (150);
    }
  }
  else{
    if (dip != 3){
      ledcWrite(4, 200);
      ledcWrite(5, 0);
      ledcWrite(6, 200);
      ledcWrite(7, 0);
    }
    else {
      ledcWrite(4, 50);
      ledcWrite(5, 0);
      ledcWrite(6, 50);
      ledcWrite(7, 0);
    }
  }
  if (!touchedLine && lineDelay.IsOver()){
    // Starting mode
    if (dip == 0){
      // 180 & charge
      ledcWrite(4, 60);
      ledcWrite(5, 0);
      ledcWrite(6, 60);
      ledcWrite(7, 0);
      
    }
    else if (dip == 1){
      // 100 & charge
      ledcWrite(4, 60);
      ledcWrite(5, 0);
      ledcWrite(6, 60);
      ledcWrite(7, 0);
    }
    else if (dip == 2){
      // round n round
      ledcWrite(4, 80);
      ledcWrite(5, 0);
      ledcWrite(6, 60);
      ledcWrite(7, 0);
    }
  }
}

#endif