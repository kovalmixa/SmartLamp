#ifndef LED_MATRIX
#define LED_MATRIX
#pragma once

#include "Arduino.h"

class ChannelIndicator : public Singltone<ChannelIndicator>{
  public:
  void writeChannelNumber(const uint8_t number) const;

  protected:
  static ChannelIndicator();

  private:
  #define CLOCK_PIN 16
  #define LATCH_PIN 2
  #define DATA_PIN 13

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
  void setDigit(uint8_t digit, bool isRight) const;
  void clearDisplay() const;
}

inline void writeChannelNumber(const uint8_t number) const{
  clearDisplay();
  setDigit(number/10, false);
  //добавить делеи на базе millis()

  clearDisplay();
  setDigit(number%10, true);
  //добавить делеи на базе millis()
}

ChannelIndicator(){
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

inline void setDigit(uint8_t digit, bool isRight){

}

inline void clearDisplay(){
  for (int i = 0; i < PINS_COUNT; i++)
    digitalWrite(INDICATOR_PINS[i], !IS_COMMON_CATHODE);
}

#endif