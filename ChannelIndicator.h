#ifndef LED_MATRIX
#define LED_MATRIX
#pragma once

#include "Arduino.h"

class ChannelIndicator{
  public:
  void setDigit(uint8_t digit, bool isRight) const;
  
  private:
  const int INDICATOR_PINS[] = {2, 3, 4, 5, 6, 7, 8}; 
  const int PINS_COUNT = sizeof(INDICATOR_PINS) / sizeof(INDICATOR_PINS[0]);
  const uint8_t segmentMap[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111,  // 9
  };
}

inline void setDigit(uint8_t digit, bool isRight){

}
#endif