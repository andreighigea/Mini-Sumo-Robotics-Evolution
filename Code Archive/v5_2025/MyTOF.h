#include "esp32-hal-gpio.h"
#ifndef MY_TOF_H
#define MY_TOF_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <SparkFun_VL53L5CX_Library.h> 
/*
class MyTOF{
  public:
    enum SensorType{
      VL53L0X,
      VL53L5CX
    };

    MyTOF(SensorType type, TwoWire wire = Wire, uint8_t address = 0x29, uint8_t shut = -1){
      _type = type;
      _wire = wire;
      _address = type;
      _shut = shut;
    }

    bool Init(){
      switch (_type){
        case VL53L5CX:
          
          break;
        default:
          break;
      }
    }

    bool StartRanging(){

    }

    void On(){
      if (shut >= 0){
        digitalWrite(_shut, HIGH);
      }
    }

    void Off(){
      if (shut >= 0){
        digitalWrite(_shut, LOW);
      }
    }

    void Read()

  private:
    SensorType _type;
    TwoWire _wire;
    uint8_t _address;
    uint8_t _shut;

    
};
*/
#endif