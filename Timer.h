#ifndef TIMER
#define TIMER
#pragma once

#include "Arduino.h"

class Timer{
  public:
  inline void setDefault() { prevMillis = millis(); }
  inline bool isTick(uint32_t delay, bool isReset = true) 
  { 
    uint32_t curMillis = millis();
    if (curMillis - prevMillis >= delay){
      if (isReset) prevMillis += delay;
      if (curMillis - prevMillis > delay) {
        prevMillis = curMillis; 
      }
      return true;
    }
    return false;
  }
  private:
  uint32_t prevMillis = 0;
};

#endif