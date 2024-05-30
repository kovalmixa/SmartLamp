void WiFiConnection(){
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  while(WiFi.status()!=WL_CONNECTED){
     delay(1000);
     Serial.println("Connecting...");
  }
  Serial.println("Connected");
  Serial.println("IP adress:");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
} 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  mode = (char)payload[0] - '0';
  if ((char)payload[2] == '1') {state = 1; buttonState = 1;}
  else if ((char)payload[2] == '0') {state = 0; buttonState = 1;}
  brightness = Conc((char)payload[3], (char)payload[4], (char)payload[5]);
  colorVal = Conc((char)payload[6], (char)payload[7], (char)payload[8], (char)payload[9]);
}
