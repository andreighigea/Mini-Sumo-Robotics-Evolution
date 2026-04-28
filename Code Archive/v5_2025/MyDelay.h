#ifndef MY_DELAY_H
#define MY_DELAY_H

class MyDelay{
  public:
    MyDelay(){
      _start = 0;
      _end = 0;
    }

    void StartTimer(int duration){
      _start = (int)millis();
      _end = _start + duration;
    }

    bool IsOver(){
      return (int)millis() > _end;
    }

    void Interrupt(){
      _end = (int)millis();
    }

  private:
    int _start;
    int _end;
};

#endif