#ifndef LED_MATRIX_H
#define LED_MATRIX_H
#pragma once

#include "Arduino.h"
#include "Singleton.h"
#include "Timer.h"

class ChannelIndicator : public Singleton<ChannelIndicator> {
friend class Singleton<ChannelIndicator>;

public:
  void tryWriteChannelNumber(const uint8_t number);

protected:
  ChannelIndicator();

private:
  #define CLOCK_PIN 16
  #define LATCH_PIN 2
  #define DATA_PIN 13

  Timer TIMER;
  bool isActiveRightDigit = false;
  const bool IS_COMMON_CATHODE = true;
  const uint8_t segmentMap[11] = {
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111, // 9
    0b1000000  // -
  };

  void setDigit(uint8_t digit, bool isRight);
  inline void clearDisplay() { updateShiftRegister(0); }
  void updateShiftRegister(const uint8_t leds);
};

inline ChannelIndicator::ChannelIndicator() {
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

inline void ChannelIndicator::tryWriteChannelNumber(const uint8_t number) {
  if (TIMER.isTick(5)) isActiveRightDigit = !isActiveRightDigit;
  if (isActiveRightDigit){
    clearDisplay();
    setDigit(number / 10, false);
  }
  else{
    clearDisplay();
    setDigit(number % 10, true);
  }
}

inline void ChannelIndicator::setDigit(uint8_t digit, bool isRight) {
  uint8_t subValue = IS_COMMON_CATHODE ? 0 : UINT8_MAX;
  uint8_t regBitMask = abs(segmentMap[digit > 9 ? 10 : digit] - subValue);
  regBitMask |= isRight ? 0 : 1 << 7;
  updateShiftRegister(regBitMask);
}

inline void ChannelIndicator::updateShiftRegister(const uint8_t leds) {
  digitalWrite(LATCH_PIN, 0);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, leds);
  digitalWrite(LATCH_PIN, 1);
}

#endif