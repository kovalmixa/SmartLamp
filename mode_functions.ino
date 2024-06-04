void fillBySingleColor(int Rcolor, int Gcolor, int Bcolor) {
  fill_solid(leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
  FastLED.show();
}
void perlinNoiseFunction(){
  if(count == STEPS){
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        arrT[i * N + j] = random(255) + 1;
      }
    }
    middleVal();
    bilinearInterpolation();
    bilinearInterpolation();
    for (int i = 0; i < NUM_LEDS; i++){
      if( arrT[i] >= 100){
        arrT[i] *= 2;
      }
      else{
        arrT[i] /= 2;
        if(arrT[i] < 10){
          arrT[i] += 100;
        }
      }
    }
    for (int i = 0; i < NUM_LEDS; i++){
      arrT[i] = (int)((float)(arr[i] - arrT[i]) / STEPS);
    }
    count = 0;
  }
  for (int i = 0; i < NUM_LEDS; i++){
    arr[i] -= arrT[i];
    delayMicroseconds(200);
  }
  count++;
}
void middleVal(){
  for (int i = 0; i < NUM_LEDS; i++){
      arrT[i] = (arr[i] + arrT[i]) / 2;
  }
}
void bilinearInterpolation(){
  for (int i = 0; i < N; i++) {
    float tx = (float)i / (N - 1);
    for (int j = 0; j < N; j++) {
      float ty = (float)j / (N - 1);
      arrT[i * N + j] = (arrT[i * N + j] + (int)(((1 - tx) * ((1 - ty) * arrT[0] + ty * arr[N-1])) + (tx * ((1 - ty) * arrT[(N - 1) * N] + ty * arrT[N * N - 1])))) / 2;
    }
  }
} 
void circleFunction(){
  for (int i = 0; i < NUM_LEDS; i++){
      arr[i] = 0;
  }
  if(!(circleSteps % (MAX_RADIUS * 5)) && circleSteps){
    if(point == 4){
      point = 0;
      circleSteps = 0;
    }
    circleArr[0][point] = random(16);
    circleArr[1][point] = random(16);
    circleArr[2][point] = 0;
    point++;
  }
  for(int k = 0; k < 4; k++){
    spawnCircle(circleArr[0][k], circleArr[1][k] , circleArr[2][k]);
    circleArr[2][k]+=4;
  }
  circleSteps+=4;
  delay(20);
}
void spawnCircle(int y, int x, int R){
  if (R <= 0){
    return; 
  }
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++){
      int pixelPosition = (i - y) * (i - y) + (j - x) * (j - x);
      float R2 = (((float)R)/20) * (((float)R)/20);
      float coef = abs(pixelPosition - R2);
      int coef2 = R/((R > 20 * 3)? 15 : 10);
      if (coef > coef2) {
        continue;
      }
      float posBrightness = (float)(MAX_RADIUS * 20 - (float)R * 1.5) / 255;
      if(posBrightness > 0){
        arr[i * N + j] += (int)((((float)255 * (coef2 - coef)) / coef2) * posBrightness);
      }
    }
  }
}
void sinusFunction(){
  for (int i = 0; i < NUM_LEDS; i++){
      arr[i] = 0;
  }
  if(dCount < 2 * PI){
    dCount += 0.1;
  }
  else{
    dCount = 0;
  }
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      float coef = abs((float)i - 6 * sin((float)j / 6 + dCount) - 8);
      if (coef > 2) {
        continue;
      }
      arr[i * N + j] = (int)((float)255 * (2 - coef) / 2);
    }
  }
}
void inversion() {
  for (int i = 0; i < N; i += 2) {
    for (int j = 0; j < N / 2; j++) {
      unsigned char temp = arr[i * N + j];
      arr[i * N + j] = arr[i * N + N - 1 - j];
      arr[i * N + N - 1 - j] = temp;
    }
  }
}
void modeAnimationFunc() {
  switch (mode){
    case 2 : {
      perlinNoiseFunction();
      break;
    }
    case 3 : {
      circleFunction();
      break;
    }
    case 4 : {
      sinusFunction();
      break;
    }
  }
  inversion();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(Rcolor * (double)arr[i] / 255, Gcolor * (double)arr[i] / 255, Bcolor * (double)arr[i] / 255);
  }
  FastLED.show();
}
void gradientFunction() {
  double colorVal2 = 0, Rcolor2 = 0, Gcolor2 = 0, Bcolor2 = 0;
  if (lastColorVal / (1023 / (3  * PI / 2)) + (double)3 * PI / (double)4 < (double)3 * PI / (double)2){
    colorVal2 = lastColorVal / (1023 / (3  * PI / 2)) + (double)3 * PI / (double)4;
  }
  else{
    colorVal2 = lastColorVal / (1023 / (3  * PI / 2)) - (double)3 * PI / (double)4;
  }
  if (colorVal2 >= PI / (double)2){
    Rcolor2 = sin(colorVal2 + PI) * (double)255;
  }
  else{
    Rcolor2 = sin(colorVal2 + ((double)PI / 2)) * (double)255;
  }
  Gcolor2 = sin(colorVal2) * (double)255;
  Bcolor2 = sin(colorVal2 + ((double)3 * PI / (double)2)) * (double)255;
  if (Rcolor2 <= 0){
    Rcolor2 = 0;
  }
  if (Gcolor2 <= 0){
    Gcolor2 = 0;
  }
  if (Bcolor2 <= 0){
    Bcolor2 = 0;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    double t = (double)i / (NUM_LEDS - 1);
    leds[i] = CRGB(((double)1 - t) * Rcolor + t * Rcolor2 * ((float)lastBrightness / 100), 
                   ((double)1 - t) * Gcolor + t * Gcolor2 * ((float)lastBrightness / 100), 
                   ((double)1 - t) * Bcolor + t * Bcolor2 * ((float)lastBrightness / 100));
  }
  FastLED.show();
}
