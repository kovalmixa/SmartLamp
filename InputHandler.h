#ifndef INPUT_HANDLER
#define INPUT_HANDLER
#pragma once

#include <EncButton.h>
#include <Arduino.h>

class InputHandler{
  public:
    InputHandler();
    bool tryGetInput(const Data* data) const;
  private:
    #define POT_PIN A0
    #define BUT_PIN 0
    #define BRT_CLK_PIN 5
    #define BRT_DT_PIN 4
    #define CLR_CLK_PIN 14
    #define CLR_DT_PIN 12

    bool _lastButtonState;

    bool _lastbrightStateCLK;
    bool _curbrightStateCLK;

    bool _lastColorStateCLK;
    bool _curColorStateCLK;

}

inline InputHandler(){
  pinMode(POT_PIN, INPUT);
  pinMode(BUT_PIN, INPUT);
  pinMode(BRT_CLK_PIN, INPUT_PULLUP);
  pinMode(BRT_DT_PIN, INPUT_PULLUP);
  pinMode(CLR_CLK_PIN, INPUT_PULLUP);
  pinMode(CLR_DT_PIN, INPUT_PULLUP);
}

inline bool tryGetInput(const Data* data){
  short channelInput = analogRead(POT_PIN);
  bool buttonState = analogRead(BUT_PIN);

  return false;
}

#endif