#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

#include "ChannelIndicator.h"
#include "InputHandler.h"
#include "MqttHandler.h"
#include "LedMatrix.h"
#include "Data.h"

//pins
#define LED_MATRIX_PIN 3
#define CLOCK_PIN 16
#define LATCH_PIN 2
#define DATA_PIN 13

//objects
Data data;
ChannelIndicator channelIndicator(CLOCK_PIN, LATCH_PIN, DATA_PIN);
LedMatrix<LED_MATRIX_PIN> ledMatrix(16, 30);

InputHandler* inputHandler;
MqttHandler* mqttHandler;

//main vars
short int colorVal = 0, lastColorVal = 0, lastLastColorVal = 0;
short int lastBrightness = 512, lastLastBrightness = 512;
short int mode = 0, lastMode = 0;
short int sign = 1, sign2 = 5;
double Rcolor = 255, Gcolor = 0, Bcolor = 0, brightness = 512;
double brightnessBefore = 512;
bool state = 0, lastState = 0;
bool pulse = 0, rainbow = 0;
double bCounter = 1.1;

//vars for mode functions
// byte arr[NUM_LEDS];
// byte arrT[NUM_LEDS];

// const byte MAX_RADIUS = 16;
// int circleSteps = 0;
// int circleArr[3][4];
// int point = 0;

// int count = STEPS;
// double dCount = 0;

//vars for mqtt messages

void rewriteLampData(LedMatrixData* newLampData){

}

void setup() {
  Serial.begin(115200);

  inputHandler = &(InputHandler::getInstance());
  inputHandler->tryGetInput(&data);

  mqttHandler = &(MqttHandler::getInstance());
  mqttHandler->setFunctionOnCallBack(rewriteLampData);
  mqttHandler->tryConnectToMqttServer(data.channel);
  // for (int i = 0; i < NUM_LEDS; i++){
  //     arr[i] = 0;
  // }
  // for (int i = 0; i < NUM_LEDS; i++){
  //     arrT[i] = 0;
  // }
  // for (int i = 0; i < 4; i++){
  //     circleArr[2][i] = -80 * i;
  // }
  // circleArr[0][0] = random(16);
  // circleArr[1][0] = random(16);
  // circleArr[2][0] = 0;
  // button.setButtonLevel(HIGH);
  // fillBySingleColor(0, 0, 255); //Blue indication before mqtt configuration
}

void loop() {
  if (data.tryConnectToMqtt && !mqttHandler->isConnectedToServer())
    mqttHandler->tryConnectToMqttServer(data.channel);
  else{
    bool isChangedData = inputHandler->tryGetInput(&data);
    channelIndicator.tryWriteChannelNumber(data.channel);
    ledMatrix.writeMatrix(&(data.ledMatrixData));
    if (isChangedData) mqttHandler->trySendDataToServer(&data);
    // client.loop();
  }
}
