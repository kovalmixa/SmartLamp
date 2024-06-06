#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define buttonPin 15

#define DATA_PIN 13
#define NUM_LEDS 256
#define N 16
#define STEPS 20
CRGB leds[NUM_LEDS];

#define MSG_BUFFER_SIZE  (50)

//192.168.231.18

//vars for web, mqtt server and WiFi autentification
const char* ssid = "guest";
const char* password = "#Knowledge-Pool@";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_username = "Smartlamp1";
const char* mqtt_password = "2qw12QWL1";
const char* mqtt_topic = "rgb_led_smart_lamp";
const char* clientId = "ESP8266Client-d9b1";
const int mqtt_port = 1883;

//main vars
int colorVal = 0, lastColorVal = 0, lastLastColorVal = 0;
int lastBrightness = 512, lastLastBrightness = 512;
int mode = 0, lastMode = 0;
double Rcolor = 255, Gcolor = 0, Bcolor = 0, brightness = 512;
bool buttonState = 0, lastButtonState = 0;
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
  
  WiFiConnection();
  WiFi.softAP(mqtt_username, "*1f2f3f@QW7ф7ф7qwe&");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.begin();
  FastLED.setBrightness(20);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
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
}
void loop() {
  buttonState = digitalRead(buttonPin);
  if (!client.connected()) {
    reconnect();
  }
  else{
    button();
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
      fillBySingleColor(0, 0, 0);
    }
  }
  lastState = state;
  lastMode = mode;
  lastLastColorVal = lastColorVal;
  lastLastBrightness = lastBrightness;
  lastButtonState = buttonState;
  server.handleClient();
  client.loop();
}
