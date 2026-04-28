#ifndef Strat2_h
#define Strat2_h

void Strat2(){
  lineLeft = analogRead(LEFT_LINE);
  lineRight = analogRead(RIGHT_LINE);
  if (!touchedLine){
    // Starting mode
    if (v1 == 1 || (v1 == 0 && def1 == 1)){
      // 180 & charge
      analogWrite(5, 50);
      analogWrite(6, 0);
      analogWrite(8, 50);
      analogWrite(9, 0);
    }
    else if (v1 == 2 || (v1 == 0 && def1 == 2)){
      // 100 & charge
      analogWrite(5, 50);
      analogWrite(6, 0);
      analogWrite(8, 50);
      analogWrite(9, 0);
    }
    else if (v1 == 3 || (v1 == 0 && def1 == 3)){
      // round n round
      analogWrite(5, 100);
      analogWrite(6, 0);
      analogWrite(8, 50);
      analogWrite(9, 0);
    }
  }
  if ((lineLeft <= 300 || lineRight <= 300) && lineDelay.IsOver() /*&& backDelay.IsOver() && sideDelay.IsOver()*/){
    touchedLine = true;
    if (lineLeft <= 300){
      analogWrite(5, 0);
      analogWrite(6, 200);
      analogWrite(8, 0);
      analogWrite(9, 70);
      lineDelay.StartTimer(280);
      lineDet = "L";
    }
    else{
      analogWrite(5, 0);
      analogWrite(6, 115);
      analogWrite(8, 0);
      analogWrite(9, 200);
      lineDelay.StartTimer(235);
      lineDet = "R";
    }

    lineLeft = analogRead(LEFT_LINE);
    lineRight = analogRead(RIGHT_LINE);
  }
  if ((lineLeft <= 300 && lineRight <= 300) && !lineDelay.IsOver() && (lineDet == "L" || lineDet == "R" || lineDet == "N")){
    if (v1 == 1 || (v1 == 0 && def1 == 1)){
      if (back < backVeryClose){
        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(8, 0);
        analogWrite(9, 255);
        delay(150);
      }
      else if (back < backClose){
        analogWrite(5, 0);
        analogWrite(6, 200);
        analogWrite(8, 0);
        analogWrite(9, 200);
        delay(70);
        analogWrite(5, 200);
        analogWrite(6, 0);
        analogWrite(8, 0);
        analogWrite(9, 200);
        //lineDelay.StartTimer(80);
        lineDelay.StartTimer(40);
      }
      else {
        analogWrite(5, 0);
        analogWrite(6, 200);
        analogWrite(8, 0);
        analogWrite(9, 200);
        delay(80);
        analogWrite(5, 200);
        analogWrite(6, 0);
        analogWrite(8, 0);
        analogWrite(9, 200);
        //lineDelay.StartTimer(125);
        lineDelay.StartTimer(50);
      }
      lineDet = "A";
    }
    else{
      if (back < backVeryClose){
        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(8, 0);
        analogWrite(9, 255);
        delay(150);
      }
      else {
        analogWrite(5, 0);
        analogWrite(6, 200);
        analogWrite(8, 0);
        analogWrite(9, 200);
        delay(70);
        analogWrite(5, 200);
        analogWrite(6, 0);
        analogWrite(8, 0);
        analogWrite(9, 200);
        //lineDelay.StartTimer(80);
        lineDelay.StartTimer(40);
      }
      
      lineDet = "A";
    }
  }
  else if ((right < rightMax || left < leftMax || frontLeft < frontLeftClose || frontRight < frontRightClose || back < backClose) && lineDelay.IsOver() && touchedLine && false){
    if (v3 == 1 || (v3 == 0 && def3 == 1)){
      if (frontRight < frontRightClose || frontLeft < frontLeftClose){
        if (frontRight < frontRightVeryClose && frontLeft < frontLeftVeryClose){
          Serial.println("test");
          analogWrite(5, 200);
          analogWrite(6, 0);
          analogWrite(8, 200);
          analogWrite(9, 0);
        }
        else if (frontRight < frontRightVeryClose && frontLeft < frontLeftClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(30);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 40);
            analogWrite(6, 0);
            analogWrite(8, 60);
            analogWrite(9, 0);
            delay(25);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            delay(40);
          }
        }
        else if (frontRight < frontRightClose && frontLeft < frontLeftVeryClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            delay(30);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 60);
            analogWrite(6, 0);
            analogWrite(8, 40);
            analogWrite(9, 0);
            delay(25);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(40);
          }
        }
        else if (frontRight < frontRightClose && frontLeft < frontLeftClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(35);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 75);
            analogWrite(6, 0);
            analogWrite(8, 75);
            analogWrite(9, 0);
            delay(25);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            delay(45);
          }
        }
        else if (frontRight < frontRightVeryClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 75);
            analogWrite(6, 0);
            analogWrite(8, 75);
            analogWrite(9, 0);
            delay(35);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 75);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(25);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            delay(45);
          }
        }
        else if (frontLeft < frontLeftVeryClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 75);
            analogWrite(6, 0);
            analogWrite(8, 75);
            analogWrite(9, 0);
            delay(35);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 75);
            analogWrite(9, 0);
            delay(25);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(45);
          }
        }
        else if (frontRight < frontRightClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 80);
            analogWrite(6, 0);
            analogWrite(8, 80);
            analogWrite(9, 0);
            delay(35);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 80);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            delay(30);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 110);
            analogWrite(6, 0);
            analogWrite(8, 60);
            analogWrite(9, 0);
            delay(45);
          }
        }
        else{
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 80);
            analogWrite(6, 0);
            analogWrite(8, 80);
            analogWrite(9, 0);
            delay(35);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 80);
            analogWrite(9, 0);
            delay(30);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 60);
            analogWrite(6, 0);
            analogWrite(8, 110);
            analogWrite(9, 0);
            delay(45);
          }
        }
      }
      else if ((left < leftMax || right < rightMax) && sideDelay.IsOver()){
        if (left < leftVeryClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 200);
            analogWrite(6, 0);
            analogWrite(8, 0);
            analogWrite(9, 0);
            sideDelay.StartTimer(50);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            sideDelay.StartTimer(80);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 175);
            analogWrite(6, 0);
            analogWrite(8, 40);
            analogWrite(9, 0);
            sideDelay.StartTimer(90);
          }
        }
        else if (left < leftClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 120);
            analogWrite(6, 0);
            analogWrite(8, 60);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 110);
            analogWrite(6, 0);
            analogWrite(8, 40);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 175);
            analogWrite(6, 0);
            analogWrite(8, 40);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
        }
        else if (right < rightVeryClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 0);
            analogWrite(6, 0);
            analogWrite(8, 200);
            analogWrite(9, 0);
            sideDelay.StartTimer(50);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            sideDelay.StartTimer(80);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 40);
            analogWrite(6, 0);
            analogWrite(8, 175);
            analogWrite(9, 0);
            sideDelay.StartTimer(90);
          }
        }
        else if (right < rightClose){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 60);
            analogWrite(6, 0);
            analogWrite(8, 120);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 40);
            analogWrite(6, 0);
            analogWrite(8, 110);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 40);
            analogWrite(6, 0);
            analogWrite(8, 175);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
        }
        else if (left < leftMax){
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 120);
            analogWrite(6, 0);
            analogWrite(8, 0);
            analogWrite(9, 60);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 100);
            analogWrite(6, 0);
            analogWrite(8, 50);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 130);
            analogWrite(6, 0);
            analogWrite(8, 40);
            analogWrite(9, 0);
            sideDelay.StartTimer(75);
          }
        }
        else{
          if (v0 == 1 || (v0 == 0 && def0 == 1)){
            analogWrite(5, 60);
            analogWrite(6, 0);
            analogWrite(8, 0);
            analogWrite(9, 120);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 2 || (v0 == 0 && def0 == 2)){
            analogWrite(5, 50);
            analogWrite(6, 0);
            analogWrite(8, 100);
            analogWrite(9, 0);
            sideDelay.StartTimer(70);
          }
          else if (v0 == 3 || (v0 == 0 && def0 == 3)){
            analogWrite(5, 40);
            analogWrite(6, 0);
            analogWrite(8, 130);
            analogWrite(9, 0);
            sideDelay.StartTimer(75);
          }
        }
      }
      else if (back < backVeryClose && sideDelay.IsOver()){

      }
      else if (back < backClose && backDelay.IsOver()) {

      }
    }
    else if (v3 == 2 || (v3 == 0 && def3 == 2)){
      // 100 & charge
      analogWrite(5, 50);
      analogWrite(6, 0);
      analogWrite(8, 50);
      analogWrite(9, 0);
    }
    else if (v3 == 3 || (v3 == 0 && def3 == 3)){
      // round n round
      analogWrite(5, 100);
      analogWrite(6, 0);
      analogWrite(8, 50);
      analogWrite(9, 0);
    }
  }
  else  if (touchedLine && lineDelay.IsOver() && moveDelay.IsOver() && sideDelay.IsOver() && backDelay.IsOver()){
    if (v2 == 1 || (v2 == 0 && def2 == 1)){
      // Full push
      analogWrite(5, 55);
      analogWrite(6, 0);
      analogWrite(8, 55);
      analogWrite(9, 0);
    }
    else if (v2 == 2 || (v2 == 0 && def2 == 2)){
      // Mid Push
      analogWrite(5, 40);
      analogWrite(6, 0);
      analogWrite(8, 40);
      analogWrite(9, 0);
    }
    else if (v2 == 3 || (v2 == 0 && def2 == 3)){
      // Cuatious push
      if (stop){
        stop = false;
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(8, 0);
        analogWrite(9, 0);
        moveDelay.StartTimer(250);
      }
      else{
        stop = true;
        analogWrite(5, 50);
        analogWrite(6, 0);
        analogWrite(8, 50);
        analogWrite(9, 0);
        moveDelay.StartTimer(250);
      }
    }
    
    lineDet = "N";
  }
}

#endif