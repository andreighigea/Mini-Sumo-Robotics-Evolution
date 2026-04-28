#include <sys/_stdint.h>
#ifndef Settings_h
#define Settings_h

#define LEFT_REG 0x01
#define FRONT_LEFT_REG 0x02
#define FRONT_RIGHT_REG 0x03
#define RIGHT_REG 0x04
#define BACK_REG 0x05
#define ALL_ON_REG 0x10
#define ALL_OFF_REG 0x11

namespace FlagSettings{
  bool returnUp = true;
  bool matador = true;
  uint8_t desiredFlagDelay = 170; // milliseconds

  uint8_t rightServoPin = 13;
  uint8_t leftServoPin = 1;
}

namespace TOFSensorsSettings {
  bool useBackSensor = false;
  uint16_t maxSeeingDist = 400;

  uint16_t rightMax = 850;
  uint16_t leftMax = 850;

  uint16_t rightClose = 450;
  uint16_t leftClose = 500;
  uint16_t frontRightClose = 350;
  uint16_t frontLeftClose = 440;

  uint16_t rightVeryClose = 300;
  uint16_t leftVeryClose = 310;
  uint16_t frontLeftVeryClose = 270;
  uint16_t frontRightVeryClose = 270;

  uint16_t backClose = 250;
  uint8_t backVeryClose = 100;
}

namespace Teo {
  uint8_t I2C_ADDRESS = 0x08;

  //Defaults
  uint8_t def0 = 2;
  uint8_t def1 = 2;
  uint8_t def2 = 1;
  uint8_t def3 = 1;
}

#endif