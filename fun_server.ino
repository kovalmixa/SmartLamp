void rgb(double Rcolor, double Gcolor, double Bcolor) {
  fill_solid(leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
  FastLED.show();
}
void button(){
  if(buttonState != lastButtonState && buttonState == 1){
      state = !state;
      mode = 0;
      sprintf(msg, "%hd%hd%hd",mode, buttonState , state);
      String brightnessStr = Numbers(brightness, 3);
      for(int i = 0; i < 3; i++){
         msg[i+3] = Numbers(brightness, 3)[i];
      }
      String colorValStr = Numbers(colorVal, 4);
      for(int i = 0; i < 4; i++){
         msg[i+6] = colorValStr[i];
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
  if(value2 >= PI / 2) Rcolor = sin(value2 + PI) * 255 * ((float)brightness / 100);
  else Rcolor = sin(value2 + (PI / 2)) * 255 * ((float)brightness / 100);
  if(Rcolor <  0) Rcolor = 0;
  Gcolor = sin(value2) * 255 * ((float)brightness / 100); if (Gcolor < 0) Gcolor = 0;
  Bcolor = sin(value2 + 3 * PI / 2) * 255 * ((float)brightness / 100); if (Bcolor <  0) Bcolor = 0;
}
void led(){
  if (colorVal > lastColorVal + 5) {
    lastColorVal += 5;
    animationColor(lastColorVal, lastBrightness);
  }
  else if (lastColorVal > colorVal + 5) {
   lastColorVal -= 5;
   animationColor(lastColorVal, lastBrightness);
  }
  if(brightness > lastBrightness){
    lastBrightness++;
    delay(5);
    animationColor(lastColorVal, lastBrightness);
  }
  else if(lastBrightness > brightness){
    lastBrightness--;
    delay(5);
    animationColor(lastColorVal, lastBrightness);
  }
}

void Blinking(){
  static int32_t Time = millis();
  if((millis()-Time)>500){
    stateb = !stateb;
  Time = millis();
  }
  if(stateb) rgb(Rcolor, Gcolor, Bcolor);
  else rgb(0, 0, 0);
  
}
void Fading(){
  static int32_t Time = millis();
  if((millis()-Time)>100){
    Time = millis();
    rgb(Rcolor, Gcolor, Bcolor);
    brightness *= fadeAmount;
    if(brightness >= 100 || brightness <= 3) fadeAmount = 1/fadeAmount;
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
