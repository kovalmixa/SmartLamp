#ifndef TIMER
#define TIMER
#pragma once

class Timer{
  public:
  inline void setDefault() { _timePassed = 0; }
  inline bool isOver(float dt, uint timeToCount){
    _timePassed += dt;
    return _timePassed >= timeToCount;
  }
  
  private:
  float _timePassed = 0;
}

#endif