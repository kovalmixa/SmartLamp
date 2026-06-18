#ifndef DATA
#define DATA
#pragma once

enum ModType{
  PerlinNoise,
  Circles,
  Sinusoid
}

enum ExtraModType{
  Gradient,
  Pulse,
  Rainbow
}

struct Data{
  float brightness;
  float colorValue;
  ModType modType;
  ExtraModType extraModType[];
}

#endif