#ifndef Delay_h
#define Delay_h

#include <Arduino.h>

// Initializable class to create delays
class Delay{
  public:
    Delay(){
      StartDelay(0);
    }

    void StartDelay(uint32_t duration){
      start_time = millis();
      end_time = start_time + duration;
      forceStop = false;
    }

    bool IsOver(){
      uint32_t current_time = millis();
      bool isOver = current_time > end_time;
      if (isOver){
        forceStop = false;
      }
      return isOver || forceStop;
    }

    void Interrupt(){
      forceStop = true;
    }

  private:
    bool forceStop = false;
    uint32_t start_time;
    uint32_t end_time;
};

#endif