#ifndef SINGLTONE
#define SINGLTONE
#pragma once

template <typename T>
class Singltone{
  public:
  inline static T& getInstance(){
    if (!_instance) _instance = new T()
    return *_instance;
  }
  
  private:
  T* _instance;
  inline ~Singltone() { delete _instance; }
}

#endif