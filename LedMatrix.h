#ifndef LED_MATRIX_H
#define LED_MATRIX_H
#pragma once

#include "Arduino.h"
#include <FastLED.h>

#include "Data.h"
#include "Timer.h"
#include "MathFun.h"

template <uint8_t MATRIX_DATA_PIN>
class LedMatrix {
public:
  LedMatrix(const size_t rowSize, const uint8_t maxBrightness);
  ~LedMatrix();

  void writeMatrix(const LedMatrixData* data);
  
private:
  static const int FRAME_STEPS = 20;
  static const int FRAME_DELAY = 50;
  static const int MAX_HUE_VALUE = 1000;
  static const int MAX_BRIGHT_VALUE = 1000;

  const size_t ROW_SIZE;

  size_t _totalQuantity;
  uint8_t* _valuesArray = nullptr;
  CRGB* _colorsArray = nullptr;
  CRGB* _leds = nullptr;
  Timer TIMER;

  void setValueMat(const LedMatrixData* data);
  void setColorMat(const LedMatrixData* data);
  void mirrorEvenRows();

  CRGB getRGBFromHue(const int colorVal);
  void setValuesToCrgbs(CRGB* crgbs, const uint8_t* values);
  void multiplyCrgbsByValues(CRGB* crgbs, const uint8_t* values);
  void setValuesArray(const uint8_t value);
};

template <uint8_t MATRIX_DATA_PIN>
inline LedMatrix<MATRIX_DATA_PIN>::LedMatrix(const size_t rowSize, const uint8_t maxBrightness) : ROW_SIZE(rowSize) {
  _totalQuantity = rowSize * rowSize;
  _leds = new CRGB[_totalQuantity];
  _valuesArray = new uint8_t[_totalQuantity];
  _colorsArray = new CRGB[_totalQuantity];
  FastLED.setBrightness(maxBrightness);
  FastLED.addLeds<NEOPIXEL, MATRIX_DATA_PIN>(_leds, _totalQuantity);
}

template <uint8_t MATRIX_DATA_PIN>
inline LedMatrix<MATRIX_DATA_PIN>::~LedMatrix() {
  delete[] _leds;
  delete[] _valuesArray;
  delete[] _colorsArray;
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::writeMatrix(const LedMatrixData* data){
  setValueMat(data);
  setColorMat(data);
  mirrorEvenRows();
  FastLED.show();
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::setValueMat(const LedMatrixData* data){
  switch(data->modType){
    case PerlinNoise : break;
    case Circles : break;
    case Sinusoid : break;
    default: break;
  }

  for (auto& extra : data->extraModTypes) {
    if (extra == Pulse) {}
  }
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::setColorMat(const LedMatrixData* data){
  CRGB rgb = getRGBFromHue(data->colorValue);
  for (auto& extra : data->extraModTypes){
    switch(extra){
      case Gradient : break;
      case Rainbow : 
        millis() % 1023;
        break;
      case Pulse : break;
    }
  }
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::mirrorEvenRows(){
  for (size_t i = 1; i < ROW_SIZE; i += 2) {
    for (size_t j = 0; j < ROW_SIZE / 2; j++) {
      size_t leftIndex = i * ROW_SIZE + j;
      size_t rightIndex = i * ROW_SIZE + (ROW_SIZE - 1 - j);
      CRGB temp = _leds[leftIndex];
      _leds[leftIndex] = _leds[rightIndex];
      _leds[rightIndex] = temp;
    }
  }
}

template <uint8_t MATRIX_DATA_PIN>
inline CRGB LedMatrix<MATRIX_DATA_PIN>::getRGBFromHue(const int colorVal) {
  CRGB rgb;
  const float colorValP = (float)colorVal / (MAX_HUE_VALUE / (3.0f * M_PI / 2.0f));
  rgb.r = std::clamp((float)(colorValP >= M_PI / 2.0f
    ? sin(colorValP + M_PI) * 255.0f
    : sin(colorValP + (M_PI / 2.0f)) * 255.0f), 0.0f, 255.0f);
    
  rgb.g = std::clamp((float)(sin(colorValP) * 255.0f), 0.0f, 255.0f);
  
  rgb.b = std::clamp((float)(sin(colorValP + 3.0f * M_PI / 2.0f) * 255.0f), 0.0f, 255.0f);
  
  return rgb;
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::setValuesToCrgbs(CRGB* crgbs, const uint8_t* values){ 
  for (size_t i = 0; i < _totalQuantity; i++) crgbs[i].r = crgbs[i].g = crgbs[i].b = values[i]; 
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::multiplyCrgbsByValues(CRGB* crgbs, const uint8_t* values){ 
  for (size_t i = 0; i < _totalQuantity; i++){
    float propValue = (float)values[i] / 255.0f;
    _leds[i].r *= propValue;
    _leds[i].g *= propValue;
    _leds[i].b *= propValue;
  } 
}

template <uint8_t MATRIX_DATA_PIN>
inline void LedMatrix<MATRIX_DATA_PIN>::setValuesArray(const uint8_t value){ 
  if (_valuesArray) for (size_t i = 0; i < _totalQuantity; i++) _valuesArray[i] = value; 
}

#endif