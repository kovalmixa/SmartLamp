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
byte arr[NUM_LEDS];
byte arrT[NUM_LEDS];

const byte MAX_RADIUS = 16;
int circleSteps = 0;
int circleArr[3][4];
int point = 0;

int count = STEPS;
double dCount = 0;

//vars for mqtt messages
char msg[MSG_BUFFER_SIZE];
String message;

//WiFi clients
WiFiClient espClient = WiFiClient();
PubSubClient client(espClient);
ESP8266WebServer server(80);


void setup() {
  Serial.begin(115200);


  pinMode(buttonPin, INPUT);

  WiFiManager wm;
  wm.autoConnect("AutoConnectAP");
  WiFi.softAP(mqtt_username, "*1f2f3f@QW7ф7ф7qwe&");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.begin();
  randomSeed(analogRead(0));
  for (int i = 0; i < NUM_LEDS; i++){
      arr[i] = 0;
  }
  for (int i = 0; i < NUM_LEDS; i++){
      arrT[i] = 0;
  }
  for (int i = 0; i < 4; i++){
      circleArr[2][i] = -80 * i;
  }
  circleArr[0][0] = random(16);
  circleArr[1][0] = random(16);
  circleArr[2][0] = 0;
  button.setButtonLevel(HIGH);
  fillBySingleColor(0, 0, 255); //Blue indication before mqtt configuration
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  else{
    if(firstConnection){
      turnOff();
      firstConnection = 0;
    }
    else{
      button.tick();
      ButtonClick();
      ButtonHold();
      led();
      if (state) {
        if(mode == 0 && checkStates()){
          clearBrightnessMatrix();
          fillBySingleColor(Rcolor, Gcolor, Bcolor);
        }
        else if(mode == 1 && checkStates()){
          clearBrightnessMatrix();
          gradientFunction();
        }
        else if (mode > 1){
          clearBrightnessMatrix();
          modeAnimationFunc();
        }
      }
      else{
        turnOff();
      }
    }
  }
  lastState = state;
  lastMode = mode;
  lastLastColorVal = lastColorVal;
  lastLastBrightness = lastBrightness;
  server.handleClient();
  client.loop();
}
