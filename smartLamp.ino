#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define buttonPin 15

#define DATA_PIN 13
#define NUM_LEDS 256
CRGB leds[NUM_LEDS];

#define MSG_BUFFER_SIZE  (50)

//192.168.231.234
//
const char* ssid = "guest";
const char* password = "#Knowledge-Pool@";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_username = "Smartlamp1";
const char* mqtt_password = "2qw12QWL1";
const char* mqtt_topic = "rgb_led_smart_lamp";
const char* clientId = "ESP8266Client-d9b1";
const int mqtt_port = 1883;

int colorVal = 0, lastColorVal = 0, lastLastColorVal = 0;
int lastBrightness = 50, lastLastBrightness = 50;
int bcounter = 0;
int mode = 0, lastMode = 0;
double Rcolor = 255, Gcolor = 0, Bcolor = 0 ,fadeAmount = 1.2 ,brightness = 50;
bool blinking = 0, fade = 0, buttonState = 0, lastButtonState = 0,  stateb = 0;
bool state = 0, lastState = 0;
char msg[MSG_BUFFER_SIZE];
String message;

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
}
void loop() {
  buttonState = digitalRead(buttonPin);
  if (!client.connected()) {
    reconnect();
  }
  else{
    button();
    led();
    if(mode != 1) bcounter = 0;
    if (state) {
      if(mode == 0){
        if(lastState != state || lastButtonState != buttonState || lastMode != mode || 
          lastLastColorVal != lastColorVal || lastLastBrightness != lastBrightness){
          rgb(Rcolor, Gcolor, Bcolor); 
        }
      }
      else{
          switch(mode){
            case 1 : {Blinking(); break;}
            case 2 : {Fading(); break;}
          }
      }
    }
    else rgb(0, 0, 0);
  }
  lastState = state;
  lastMode = mode;
  lastLastColorVal = lastColorVal;
  lastLastBrightness = lastBrightness;
  lastButtonState = buttonState;
  server.handleClient();
  client.loop();
}
