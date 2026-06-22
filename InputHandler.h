#ifndef INPUT_HANDLER
#define INPUT_HANDLER
#pragma once

#include <EncButton.h>
#include <Arduino.h>

class InputHandler : public Singltone<InputHandler>{
  public:
  bool tryGetInput(Data* data);

  protected:
  InputHandler();

  private:
    #define POT_PIN A0
    #define BUT_PIN 0
    #define BRT_CLK_PIN 5
    #define BRT_DT_PIN 4
    #define CLR_CLK_PIN 14
    #define CLR_DT_PIN 12
    const int MAX_ENCODER_VALUE = 1000;

    bool _lastbrightStateCLK, _lastColorStateCLK;
    EncButton<EB_TICK, BUT_PIN> button;

    void readRotationEncoder(const uint8_t clkPin, const uint8_t dtPin, 
        const bool lastStateCLK, int* value);
    void processButtonInput(Data* data, bool* isChanged);
}

inline bool tryGetInput(Data* data){
  auto* ledMatrixData = &(data->ledMatrixData); 
  bool isChanged = false;

  uint8_t channelInput = map(analogRead(POT_PIN), 0, 1023, 0, 100);
  if (channelInput != data->channel) isChanged = true;

  int brightness, color;
  readRotationEncoder(BRT_CLK_PIN, BRT_DT_PIN, &_lastbrightStateCLK, &brightness);
  readRotationEncoder(CLR_CLK_PIN, CLR_DT_PIN, &_lastColorStateCLK, &color);

  if (brightness > MAX_ENCODER_VALUE) brightness *= -1;
  if (color > MAX_ENCODER_VALUE) color = 0;

  if (brightness != data->brightness){
    ledMatrixData->brightness = (short)abs(brightness);
    isChanged = true;
  }
  if (color != data->color){
    ledMatrixData->color = (short)color;
    isChanged = true;
  }
  
  processButtonInput(data, &isChanged);
  return isChanged;
}
inline void InputHandler(){
  pinMode(POT_PIN, INPUT_PULLUP);
  pinMode(BUT_PIN, INPUT_PULLUP);

  pinMode(BRT_CLK_PIN, INPUT_PULLUP);
  pinMode(BRT_DT_PIN, INPUT_PULLUP);

  pinMode(CLR_CLK_PIN, INPUT_PULLUP);
  pinMode(CLR_DT_PIN, INPUT_PULLUP);

  _lastButtonState = digitalRead(BUT_PIN);
  _lastbrightStateCLK = digitalRead(BRT_CLK_PIN);
  _lastColorStateCLK = digitalRead(CLR_CLK_PIN);
}

inline void readRotationEncoder(const uint8_t clkPin, const uint8_t dtPin, 
    const bool* lastStateCLK, int* value){
  bool currentStateCLK = digitalRead(clkPin);
  if (currentStateCLK != *lastStateCLK) {
    if (digitalRead(dtPin) != currentStateCLK) value--;
    else value++;
  }
  *lastStateCLK = currentStateCLK;
}

inline void processButtonInput(Data* data, bool* isChanged){
  button.tick();
  bool isHold = false;
  if (button.isClick()){
    data->ledMatrixData.power = !data->ledMatrixData.power;
    *isChanged = true;
  }
  else if(button.isRelease()) data->tryConnectToMqtt = !data->tryConnectToMqtt;
}

#endif