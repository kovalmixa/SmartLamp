#ifndef LED_MATRIX
#define LED_MATRIX
#pragma once

#include "Arduino.h"
#include <FastLED.h>
#include <iostream>
#include <cmath>

#include "Data.h"
#include "TimeHandler.h"

class LedMatrix{
  public:
  LedMatrix(const uint8_t  rowSize, const uint8_t  maxBrightness);
  ~LedMatrix();

  void writeMatrix(const float dt, const Data* data) const;

  private:
  const MAX_HUE_VALUE = 1023;
  const uint8_t  DATA_PIN;
  const size_t ROW_SIZE;

  size_t _totalQuantity;
  CRGB* _leds;

  void setValueMat(const float dt, const Data* data) const;
  void setColorMat(const float dt, const Data* data) const;
  CRGB getRgb(const short colorVal) const;
}

inline LedMatrix(const uint8_t  dataPin, const uint8_t  rowSize,
    const uint8_t  maxBrightness) : DATA_PIN(dataPin), ROW_SIZE(rowSize) {
  _totalQuantity = rowSize * rowSize;
  _leds = new CRGB[NUM_LEDS];
  FastLED.setBrightness(maxBrightness);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(_leds, _totalQuantity);
}

inline ~LedMatrix(){
  delete[] _leds;
}


inline void writeMatrix(const float dt, const Data* data){

}

inline void setValueMat(const float dt, const Data* data){
  switch(data->modType){
    case PerlinNoise : {
      break;
    }
    case Circles : {
      break;
    }
    case Sinusoid : {
      break;
    }
    default: {
      break;
    }
  }

  switch(data->extraModType){
    case Pulse : {
      break;
    }
  }
}

inline void setColorMat(const float dt, const Data* data){
  CRGB rgb = getRgb(data->colorValue);
  switch(data->extraModType){
    case Gradient : {
      break;
    }
      case Rainbow : {
      break;
    }
      default : {

    }
  }
}

inline CRGB getRgb(const short colorVal) const{
  const CRGB rgb;
  const float colorValP = colorVal / (MAX_HUE_VALUE / (3 * Math.PI / 2));
  rgb.r = clamp((colorValP >= Math.PI / 2
   ? std::sin(colorValP + Math.PI) * 255
   : std::sin(colorValP + (Math.PI / 2)) * 255), 0 , 255);
  rgb.g = clamp(std::sin(colorValP) * 255, 0 , 255);
  rgb.b = clamp(std::sin(colorValP + 3 * Math.PI / 2) * 255, 0 , 255);
  return rgb;
}

#endif