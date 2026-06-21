#ifndef DATA
#define DATA
#pragma once

enum ModType{
  PerlinNoise, Circles, Sinusoid
}

enum ExtraModType{
  Gradient, Pulse, Rainbow
}

struct LedMatrixData{
  bool power;
  short brightness;
  short colorValue;
  ModType modType;
  ExtraModType extraModType[];
}


struct Data{
  bool tryConnectToMqtt;
  uint8_t channel;
  LedMatrixData ledMatrixData;
}

#endif