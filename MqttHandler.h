#ifndef MQTT_HANDLER
#define MQTT_HANDLER
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <cstdlib>
#include <ctime>

#include "Timer.h"
#include "Data.h"

class MqttHandler : public Singltone<MqttHandler>{
  public:
  bool tryConnectToMqttServer(const uint8_t channelId);
  inline bool isConnectedToServer() const { return client.connected(); }
  inline void setFunctionOnCallBack() {  }
  void trySendDataToServer(Data* data);
  protected:
  static MqttHandler();

  private:
  #define MQTT_LED_PIN 15
  #define MSG_BUFFER_SIZE (200)
  const char* _ssid = "smartLampDevice_";
  const char* _password;
  const char* _mqttTopic = "smartLamp_";
  const int _mqttPort = 1883;
  int _currentRandIndex;

  char _msg[MSG_BUFFER_SIZE];
  char* _message;

  WiFiClient espClient = WiFiClient();
  PubSubClient client(espClient);

  const Timer TIMER;
  const uint ROTATION_ENCODER_SEND_DELAY = 100;
  const uint TIME_TO_RECONNECT = 3000;
  const uint CHANNEL_CHANGE_DELAY = 500;

  uint8_t _lastChannelId;
  uint8_t _channelToReconnect;
  short _lastBrightness, _lastColor;

  void tryReconnectChannel(const uint8_t channelId);
  void callback(char* topic, byte* payload, unsigned int length) ;
}

inline bool tryConnectToMqttServer(const uint8_t channelId) {
  if (!TIMER.isTick(TIME_TO_RECONNECT)) return false;
  _lastChannelId = channelId;
  Serial.print("Attempting MQTT connection...");
  if (client.connect(clientId, mqtt_username, mqtt_password)) {
    Serial.println("connected");
    client.subscribe(mqtt_topic + std::to_string(channelId));
  } 
  else Serial.println("failed, rc=" + std::to_string(client.state()) + ". Try again in " + std::to_string(TIME_TO_RECONNECT) + "milliseconds");
  return isConnectedToServer();
}

inline void trySendDataToServer(Data* data) {
  if (_lastChannelIdId != data->channel) {
    tryReconnect(data);
    return;
  }

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  
  if (error) return false;
  this->id = doc["id"];
  this->name = doc["name"].as<String>();
  return true;
  // client.publish(mqtt_topic, msg);
}


inline void tryReconnectChannel(const uint8_t channelId){
{
  if (_channelToReconnect != channelId){
    _channelToReconnect = channelId;
    TIMER.setDefault();
  }
  else (TIMER.isTick(CHANNEL_CHANGE_DELAY)) 
    tryConnectToMqttServer(channelId);
}

inline void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i]);
  Serial.println();
}

inline MqttHandler(){
  pinMode(MQTT_LED_PIN, OUTPUT);

  std::srand(std::time(0));
  _currentRandIndex = std::rand() % 10000 + 1;
  _ssid += std::to_string(_currentRandIndex);
  _password = std::to_string(std::rand());

  WiFiManager wm;
  wm.autoConnect("AutoConnectAP");
  WiFi.softAP(_ssid, _password);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

#endif