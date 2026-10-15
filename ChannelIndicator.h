#ifndef CHANNEL_INDICATOR_H
#define CHANNEL_INDICATOR_H
#pragma once

#include "Arduino.h"
#include "Timer.h"

class ChannelIndicator {
public:
  ChannelIndicator(uint8_t clockPin = 16, uint8_t latchPin = 2, uint8_t dataPin = 13);
  
  void tryWriteChannelNumber(const uint8_t number);

private:
  const uint8_t _clockPin;
  const uint8_t _latchPin;
  const uint8_t _dataPin;

  Timer TIMER;
  bool isActiveRightDigit = false;
  static const bool IS_COMMON_CATHODE = true;
  
  static const uint8_t SEGMENT_MAP[11];

  void setDigit(uint8_t digit, bool isRight);
  inline void clearDisplay() { updateShiftRegister(0); }
  void updateShiftRegister(const uint8_t leds);
};

const uint8_t ChannelIndicator::SEGMENT_MAP[11] = {
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

inline ChannelIndicator::ChannelIndicator(uint8_t clockPin, uint8_t latchPin, uint8_t dataPin)
  : _clockPin(clockPin), _latchPin(latchPin), _dataPin(dataPin) {
  pinMode(_clockPin, OUTPUT);
  pinMode(_latchPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
}

inline void ChannelIndicator::tryWriteChannelNumber(const uint8_t number) {
  if (TIMER.isTick(5)) isActiveRightDigit = !isActiveRightDigit;
  if (isActiveRightDigit) {
    clearDisplay();
    setDigit(number / 10, false);
  } else {
    clearDisplay();
    setDigit(number % 10, true);
  }
}

inline void ChannelIndicator::setDigit(uint8_t digit, bool isRight) {
  uint8_t subValue = IS_COMMON_CATHODE ? 0 : UINT8_MAX;
  uint8_t targetDigit = (digit > 9) ? 10 : digit;
  uint8_t regBitMask = abs(SEGMENT_MAP[targetDigit] - subValue);
  regBitMask |= isRight ? 0 : (1 << 7);
  updateShiftRegister(regBitMask);
}

inline void ChannelIndicator::updateShiftRegister(const uint8_t leds) {
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST, leds);
  digitalWrite(_latchPin, HIGH);
}

#endif