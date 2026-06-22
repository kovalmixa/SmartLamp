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

Data data;
InputHandler inputHandler;
ChannelIndicator channelIndicator;
LedMatrix ledMatrix;
MqttHandler mqttHandler;

//main vars
bool firstConnection = 1;
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

void rewriteLampData(LedMatrixData newLampData){

}

void setup() {
  Serial.begin(115200);

  inputHandler = InputHandler.getInstance();
  inputHandler.tryGetInput(&data) ;

  channelIndicator = ChannelIndicator.getInstance();

  ledMatrix = LedMatrix.getInstance();
  ledMatrix.Setup(16, 50);

  mqttHandler = MqttHandler.getInstance();
  mqttHandler.setFunctionOnCallBack(rewriteLampData);
  mqttHandler.tryConnectToMqttServer(&data);
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
  if (data.tryConnectToMqtt && mqttHandler.!client.connected()) {
    reconnect();
    return;
  }

  bool isChangedData = inputHandler.tryGetInput(&data);
  channelIndicator.writeChannelNumber(data.channel);
  ledMatrix.writeMatrix(dt, &(data.ledMatrixData));
  if (isChangedData) mqttHandler.
  lastState = state;
  lastMode = mode;
  lastLastColorVal = lastColorVal;
  lastLastBrightness = lastBrightness;
  server.handleClient();
  client.loop();
}
