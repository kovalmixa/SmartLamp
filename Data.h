#ifndef DATA
#define DATA
#pragma once

#include <vector>

enum ModType{
  None, PerlinNoise, Circles, Sinusoid
}

enum ExtraModType{
  Gradient, Pulse, Rainbow
}

struct LedMatrixData{
  bool power = false;
  short brightness = 100;
  short colorValue = 0;
  ModType modType = ModType.None;
  std::vector<ExtraModType> extraModTypes;
}


struct Data{
  bool tryConnectToMqtt = true;
  uint8_t channel;
  LedMatrixData ledMatrixData;
}

#endif