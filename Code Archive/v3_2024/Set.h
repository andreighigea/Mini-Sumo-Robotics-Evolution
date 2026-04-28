#ifndef Set_h
#define Set_h

#include <Wire.h>
#include <IRutils.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include "Settings.h"

#define EEPROM_SIZE 1
#define EEPROM_ADDR 0

using namespace Teo;
uint64_t result = 0;

uint8_t dip;
uint8_t setDip;
uint8_t start = 0;

uint8_t bright;

Adafruit_NeoPixel pixel(1, 21, NEO_GRB + NEO_KHZ800);

// ==== Values to store (2 bits each: 0 to 3) ====
uint8_t v0;  // bits 1:0
uint8_t v1;  // bits 3:2
uint8_t v2;  // bits 5:4
uint8_t v3;  // bits 7:6

void SetupSet() {
  pixel.begin();
  pixel.setBrightness(100);

  pinMode(TX, INPUT_PULLDOWN);
  pinMode(RX, INPUT_PULLDOWN);
  EEPROM.begin(EEPROM_SIZE);

  // ==== Read and unpack ====
  uint8_t readByte = EEPROM.read(EEPROM_ADDR);

  v0 = readByte & 0b00000011;
  v1 = (readByte >> 2) & 0b00000011;
  v2 = (readByte >> 4) & 0b00000011;
  v3 = (readByte >> 6) & 0b00000011;
}

void SetLoop() {
  Wire.requestFrom(0x08, 11); // Request 8 bytes from the slave

  uint64_t receivedValue = 0;
  for (int i = 0; i < 8 && Wire.available(); i++) {
    uint8_t byteReceived = Wire.read();
    receivedValue |= ((uint64_t)byteReceived << (8 * i));
    //Serial.print("Test");
  }

  if (Wire.available()){
    dip = Wire.read();
    bright = Wire.read();
    start = Wire.read();
  }

  setDip = digitalRead(TX) * 2 + digitalRead(RX);
  pixel.setBrightness(bright);
/*
  if (dip == 3){

    uint8_t* set;

    switch (setDip) {
      case 0:
        set = &v0;
        break;
      case 1:
        set = &v1;
        break;
      case 2:
        set = &v2;
        break;
      case 3:
        set = &v3;
        break;
    }

    //Serial.print("Received 64-bit value: ");
    if (result != receivedValue){
      result = receivedValue;
      Serial.println(result, HEX);
      switch (result) {
        case 0xFF6897:
        case 0xFFC03F:
          *set = 0;
          break;
        case 0xFF30CF:
        case 0xFF10EF:
          *set = 1;
          break;
        case 0xFF18E7:
        case 0xFF906F:
          *set = 2;
          break;
        case 0xFF7A85:
        case 0xFF50AF:
          *set = 3;
          break;
      }
      uint8_t packedByte = (v3 << 6) | (v2 << 4) | (v1 << 2) | v0;
      EEPROM.write(EEPROM_ADDR, packedByte);
      EEPROM.commit();
    }
    else{
      switch (*set) {
        case 0:
          pixel.setPixelColor(0,pixel.Color(255,255,255));
          pixel.show();
          break;
        case 1:
          pixel.setPixelColor(0,pixel.Color(255,0,0));
          pixel.show();
          break;
        case 2:
          pixel.setPixelColor(0,pixel.Color(0,255,0));
          pixel.show();
          break;
        case 3:
          pixel.setPixelColor(0,pixel.Color(0,0,255));
          pixel.show();
          break;
      }
    }
  }
  else{
    pixel.setPixelColor(0,pixel.Color(0,0,0));
    pixel.show();
  }
  */
/*
  Serial.println("Unpacked 2-bit values:");
  Serial.print("v0: "); Serial.println(v0);
  Serial.print("v1: "); Serial.println(v1);
  Serial.print("v2: "); Serial.println(v2);
  Serial.print("v3: "); Serial.println(v3);
*/
}

#endif