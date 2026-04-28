#define I2C_ADDRESS 0x1B
#define LOX_ADDRESS 0x29

#define LEFT_REG 0x01
#define FRONT_LEFT_REG 0x02
#define FRONT_RIGHT_REG 0x03
#define RIGHT_REG 0x04
#define BACK_REG 0x05
#define ALL_ON_REG 0x10
#define ALL_OFF_REG 0x11

#define RED_IR 0xFF10
#define GREEN_IR 0xFF90
#define BLUE_IR 0xFF50
#define PURPLE_IR 0xFF58
#define YELLOW_IR 0xFF18
#define WHITE_IR 0xFFC0
#define ORANGE_IR 0xFF28

#define DIP_REG 0x20
#define IR_REG 0x21

#ifdef CONFIG_IDF_TARGET_ESP32S3
  #include "ESP32S3.h"
#elif CONFIG_IDF_TARGET_ESP32C3
  #include "ESP32C3.h"
#endif