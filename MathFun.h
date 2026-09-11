#ifndef MATH_FUN_H
#define MATH_FUN_H
#pragma once

#include <FastLED.h>
#include "Arduino.h"

static void addSinusoid(uint8_t* valuesArray){

}

static void addCircle(uint8_t* valuesArray){

}

static void addLiniarInterpolation(uint8_t* valuesArray){

}

static void addBiliniarInterpolation(uint8_t* valuesArray){

}

static void addPerlinNoise(uint8_t* valuesArray){

}

static CRGB getRgb(const short colorVal, const maxValue) {
  const CRGB rgb;
  const float colorValP = colorVal / (maxValue / (3 * Math.PI / 2));
  rgb.r = clamp((colorValP >= Math.PI / 2
    ? std::sin(colorValP + Math.PI) * UINT8_MAX
    : std::sin(colorValP + (Math.PI / 2)) * UINT8_MAX), 0 , UINT8_MAX);
  rgb.g = clamp(std::sin(colorValP) * UINT8_MAX, 0 , UINT8_MAX);
  rgb.b = clamp(std::sin(colorValP + 3 * Math.PI / 2) * UINT8_MAX, 0 , UINT8_MAX);
  return rgb;
}

#endif