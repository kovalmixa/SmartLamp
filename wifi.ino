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
  if ((char)payload[1] == '1'){
    state = 1; buttonState = 1;
  }
  else if ((char)payload[1] == '0'){
    state = 0; buttonState = 1;
  }
  pulse = (char)payload[2] - '0';
  rainbow = (char)payload[3] - '0';
  brightness = Conc((char)payload[4], (char)payload[5], (char)payload[6], (char)payload[7]);
  colorVal = Conc((char)payload[8], (char)payload[9], (char)payload[10], (char)payload[11]);
}
