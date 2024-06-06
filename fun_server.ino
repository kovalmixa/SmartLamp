void button(){
  if(buttonState != lastButtonState && buttonState == 1){
      state = !state;
      mode = 0;
      sprintf(msg, "%hd%hd%hd%hd",mode , state, pulse, rainbow);
      for(int i = 0; i < 4; i++){
         msg[i+4] = Numbers(brightness, 4)[i];
      }
      for(int i = 0; i < 4; i++){
         msg[i+8] = Numbers(colorVal, 4)[i];
      }
      client.publish(mqtt_topic, msg);
  }
}
int Conc(char a, char b, char c, char d = '\0'){
  char arr[5];
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
  arr[4] = '\0';
  String conc = String(arr);
  return atoi(conc.c_str()); 
}
void animationColor(int colorValue, int brightness){
  double value2 = colorValue / (1023 / (3  * PI / 2));
  if(value2 >= PI / 2){
    Rcolor = sin(value2 + PI) * 255 * ((float)brightness / 1023);
  }
  else{
    Rcolor = sin(value2 + (PI / 2)) * 255 * ((float)brightness / 1023);
  }
  Gcolor = sin(value2) * 255 * ((float)brightness / 1023);
  Bcolor = sin(value2 + 3 * PI / 2) * 255 * ((float)brightness / 1023);
  if(Rcolor <  0){
    Rcolor = 0;
  }
  if (Gcolor < 0){
    Gcolor = 0;
  }
  if (Bcolor <  0){
    Bcolor = 0;
  }
}
void led(){
  if(!rainbow){
    if (colorVal > lastColorVal + 5) {
      lastColorVal += 5;
      animationColor(lastColorVal, lastBrightness);
    }
    else if (lastColorVal > colorVal + 5) {
     lastColorVal -= 5;
     animationColor(lastColorVal, lastBrightness);
    }
  }
  else{
    Rainbow();
  }
  if(!pulse){
   if(brightness > lastBrightness + 5){
      lastBrightness += 5;
      animationColor(lastColorVal, lastBrightness);
    }
    else if(lastBrightness > brightness + 5){
      lastBrightness -= 5;
      animationColor(lastColorVal, lastBrightness);
    }
  }
  else{
    if(brightness > 945){
      brightness = 945;
    }
    Pulse();
  }
}
  
String Numbers(int value, int width){
  String numStr = String(value);
  int i = 0, count = 0;
  while(numStr[i]){
    count++;
    i++;
  }
  int zeros = width - count;
  String result = ""; 
    for (int i = 0; i < zeros; i++) {
        result += '0';
    }
  return result + numStr;
}
bool checkStates(){
  return (lastState != state || lastButtonState != buttonState || lastMode != mode || 
          lastLastColorVal != lastColorVal || lastLastBrightness != lastBrightness);
}
void clearBrightnessMatrix(){
  if(mode != lastMode){
    for (int i = 0; i < NUM_LEDS; i++){
      arr[i] = 0;
    }
  }
}
