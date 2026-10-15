#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <cstdlib>
#include <ctime>
#include <functional>

#include "Singleton.h"
#include "Timer.h"
#include "Data.h"

class MqttHandler : public Singleton<MqttHandler> {
friend class Singleton<MqttHandler>;
public:
  void setFunctionOnCallBack(std::function<void(LedMatrixData*)> cb);
  bool tryConnectToMqttServer(const uint8_t channelId);
  void trySendDataToServer(Data* data);
  inline bool isConnectedToServer() { return client.connected(); }

protected:
  MqttHandler();

private:
  std::function<void(LedMatrixData*)> onDataReceived;

  static const int MQTT_LED_PIN = 15;
  static const int MSG_BUFFER_SIZE = 200;

  String _ssid = "smartLampDevice_";
  String _password;
  String _mqttTopic = "smartLamp_";
  const char* _mqttServer = "broker.hivemq.com";
  const int _mqttPort = 1883;
  int _currentRandIndex;

  char _msg[MSG_BUFFER_SIZE];

  WiFiClient espClient;
  PubSubClient client{espClient};

  Timer TIMER;
  const unsigned int ROTATION_ENCODER_SEND_DELAY = 100;
  const unsigned int TIME_TO_RECONNECT = 3000;
  const unsigned int CHANNEL_CHANGE_DELAY = 500;

  uint8_t _lastChannelId;
  uint8_t _channelToReconnect;
  short _lastBrightness, _lastColor;

  void tryReconnectChannel(const uint8_t channelId);
  static void callback(char* topic, byte* payload, unsigned int length);
};

inline void MqttHandler::setFunctionOnCallBack(std::function<void(LedMatrixData*)> cb) {
    onDataReceived = cb;
}

inline bool MqttHandler::tryConnectToMqttServer(const uint8_t channelId) {
  if (!TIMER.isTick(TIME_TO_RECONNECT)) return false;
  _lastChannelId = channelId;
  Serial.print("Attempting MQTT connection...");
  
  if (client.connect(_ssid.c_str(), "", "")) {
    Serial.println("connected");
    client.subscribe((_mqttTopic + String(channelId)).c_str());
  } 
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(". Try again...");
  }
  return isConnectedToServer();
}

inline void MqttHandler::trySendDataToServer(Data* data) {
  if (_lastChannelId != data->channel) {
    tryReconnectChannel(data->channel);
    return;
  }
  // client.publish(_mqttTopic.c_str(), _msg);
}

inline MqttHandler::MqttHandler() {
  pinMode(MQTT_LED_PIN, OUTPUT);

  std::srand(std::time(nullptr));
  _currentRandIndex = std::rand() % 10000 + 1;
  _ssid += String(_currentRandIndex);
  _password = String(std::rand());

  WiFiManager wm;
  wm.autoConnect("AutoConnectAP");

  client.setServer(_mqttServer, _mqttPort);
  client.setCallback(callback);
}

inline void MqttHandler::tryReconnectChannel(const uint8_t channelId) {
  if (_channelToReconnect != channelId) {
    _channelToReconnect = channelId;
    TIMER.setDefault();
  } else {
    if (TIMER.isTick(CHANNEL_CHANGE_DELAY)) {
      tryConnectToMqttServer(channelId);
    }
  }
}

inline void MqttHandler::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

#endif