#ifndef LED_MATRIX
#define LED_MATRIX
#pragma once

#include "Arduino.h"
#include <FastLED.h>
#include <iostream>
#include <cmath>

#include "Data.h"
#include "Timer.h"
#include "MathFun.h"

class LedMatrixData : public Singltone<LedMatrixData> {
  public:
  Setup(const uint8_t  rowSize, const uint8_t  maxBrightness);
  void writeMatrix(const float dt, const LedMatrixData* data) const;

  protected:
  ~LedMatrixData() const;

  private:
  const FRAME_STEPS = 20;
  const FRAME_DELAY = 50;
  const Timer TIMER;
  const MAX_HUE_VALUE = 1000;
  const MAX_BRIGHT_VALUE = 1000;
  const uint8_t  MATRIX_DATA_PIN;
  const size_t ROW_SIZE;

  size_t _totalQuantity;
  uint8_t* _valuesArray;
  CRGB* _colorsArray;
  CRGB* _leds;

  void setValueMat(const float dt, const LedMatrixData* data);
  void setColorMat(const float dt, const LedMatrixData* data);

  void mirrorEvenRows(CRGB* _leds);
  inline void setValuesArray(const uint8_t value){ for (int i = 0; i < NUM_LEDS; i++) _valuesArray[i] = value; }

  void setValuesToCrgbs(CRGB* crgbs, const uint8_t* values);
  void multiplyCrgbsByValues(CRGB* crgbs, const uint8_t* values);
}

inline Setup(const uint8_t  dataPin, const uint8_t  rowSize,
    const uint8_t  maxBrightness) : MATRIX_DATA_PIN(dataPin), ROW_SIZE(rowSize) {
  _totalQuantity = rowSize * rowSize;
  _leds = new CRGB[NUM_LEDS];
  _valuesArray = new uint8_t[NUM_LEDS];
  _colorsArray = new uint8_t[NUM_LEDS];
  FastLED.setBrightness(maxBrightness);
  FastLED.addLeds<NEOPIXEL, MATRIX_DATA_PIN>(_leds, _totalQuantity);
}

inline ~LedMatrixData() const{
  delete[] _leds, _valuesArray;
}


inline void writeMatrix(const LedMatrixData* data){
  setValueMat(dt, data);
  setColorMat(dt, data);
  mirrorEvenRows();
  FastLED.show();
}

inline void setValueMat(const LedMatrixData* data){

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

inline void setColorMat(const LedMatrixData* data){
  CRGB rgb = getRgb(data->colorValue);
  for (auto& extra : data->extraModTypes){
    switch(extra){
      case Gradient : {
        break;
      }
      case Rainbow : {
        millis() % 1023;
        break;
      }
      case Pulse : {
        break;
      }
    }
  }
}

inline void mirrorEvenRows(CRGB* _leds){
  for (int i = 1; i < ROW_SIZE; i+=2){
    for (int j = 0; j < ROW_SIZE; j++){
      auto& ledA = _leds[i * ROW_SIZE + j];
      auto& ledB = _leds[i * ROW_SIZE + j];
      auto tmpValue = *ledA;
      *ledA = *ledB;
      *ledB = tmpValue;
    }
  }
}

inline void setValuesToCrgbs(CRGB* crgbs, const uint8_t* values) { 
  for (int i = 0; i < NUM_LEDS; i++) crgbs[i].r = crgbs[i].g = crgbs[i].b = values[i]; 
}

inline void multiplyCrgbsByValues(CRGB* crgbs, const uint8_t* values) { 
  for (int i = 0; i < NUM_LEDS; i++){
    float propValue = values[i] / UINT8_MAX;
    _leds[i].r *= propValue;
    _leds[i].g *= propValue;
    _leds[i].b *= propValue;
  } 
}

#endif