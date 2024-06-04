void handleConnect(){
  String mqtt_password = server.arg("password");
  server.send(200, "text/html", "astablished connection"); 
}
void handleRoot(){
  message = "<html lang='en'>";
  message += "<head>";
  message += "<meta charset='UTF-8'>";
  message += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  message += "<style>";
  message += "body{";
  message += "display: flex;";
  message += "justify-content: center;";
  message += "align-items: center;";
  message += "height: 100vh;";
  message += "margin: 10px;";
  message += "margin-top: 10px;";
  message += "color: rgb(0, 33, 104);";
  message += "font-weight: bold;";
  message += "background: linear-gradient(to top, rgb(101, 163, 255), hsl(0, 0%, 100%));";
  message += "}";
  message += ".column{";
  message += "display: flex;";
  message += "flex-direction: column;";
  message += "background-color: #fdf7fd;";
  message += "border: 10px solid rgb(55, 110, 192);";
  message += "border-radius: 10px;";
  message += "padding: 50px;";
  message += "border-right: 60px solid rgb(55, 110, 192);";
  message += "border-top-right-radius: 50px;";
  message += "border-bottom: 30px solid rgb(55, 110, 192);";
  message += "}";
  message += ".strok{";
  message += "display: flex;";
  message += "flex-direction: row;";
  message += "}";
  message += ".smartlamp{";
  message += "display: flex;";
  message += "flex-direction: row;";
  message += "padding-bottom: 50px;";
  message += "}";
  message += ".brightness{";
  message += "display: flex;";
  message += "justify-content: center;";
  message += "align-items: center;";
  message += "padding-left: 30px;";
  message += "font-size: 20px;";
  message += "}";
  message += ".brightness:hover{";
  message += "color: rgb(20, 66, 163);";
  message += "}";
  message += ".onOff{";
  message += "display: flex;";
  message += "justify-content: center;";
  message += "align-items: center;";
  message += "font-weight: bold;";
  message += "padding: 10px 20px;";
  message += "background-color: rgb(0, 102, 255);";
  message += "border: 10px solid  rgb(0, 102, 255);";
  message += "border-radius: 10px;";
  message += "color: #ffffff;";
  message += "width: 100px;";
  message += "height: 50px;";
  message += "}";
  message += ".modes{";
  message += "display: flex;";
  message += "justify-content: center;";
  message += "align-items: center;";
  message += "font-weight: bold;";
  message += "background-color: rgb(0, 102, 255);";
  message += "border: 10px solid  rgb(0, 102, 255);";
  message += "border-radius: 10px;";
  message += "padding-left: 10px;";
  message += "color: #ffffff;";
  message += "margin-top: 20px;";
  message += "}";
  message += ".colorDisplay{";
  message += "width: 300;";
  message += "height: 25px;";
  message += "background-color: rgb(255, 255, 255);";
  message += "border-radius: 10px;";
  message += "}";
  message += ".colorRange{";
  message += "justify-content: center;";
  message += "align-items: center;";
  message += "width: 200px;";
  message += "}";
  message += "</style>";
  message += "</head>";
  message += "<body>";
  message += "<div id='DIV' class='column'>";
  message += "<h1 class='smartlamp'>Smart lamp</h1>";
  message += "<div class='strok'>";
  message += "<button name=onOff id='onOff' class='onOff'>Off</button>";
  message += "<p class='brightness'>Brightness:</p>";
  message += "</div>";
  message += "<input name=brightnessRange type='range' id='brightnessRange' class='colorRange' min='10' max='100'> ";
  message += "<p class='brightness'>Color:</p>";
  message += "<input name=colorRange type='range' id='colorRange' class='colorRange' min='0' max='1023'>";
  message += "<div id='colorDisplay' class='colorDisplay'></div>";
  message += "<button id='modes' class='modes'>Modes:</button>";
  message += "</div>";
  message += "</body>";
  message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.js' type='text/javascript'></script>";
  message += "<script>";
  message += "document.addEventListener('DOMContentLoaded', function () {";
  message += "let modeState = false;";
  message += "let lightMode =";
  message += mode;
  message += ";";
  message += "let Rcolor, Gcolor, Bcolor;";
  message += "let state =";
  message += state;
  message += ";";
  message += "let buttonState = ";
  message += buttonState;
  message += ";";
  message += "let brightness =";
  message += brightness;
  message += ";";
  message += "let colorVal =";
  message += colorVal;
  message += ";";
  message += "let sub = '";
  message += mqtt_topic;
  message += "';";
  message += "let lastMessage = 'null';";
  message += "let client = new Paho.MQTT.Client('broker.hivemq.com', 8000, 'adfsgd');";
  message += "client.onConnectionLost = onConnectionLost;";
  message += "client.onConnectionLost = onConnectionLost;";
  message += "client.onMessageArrived = onMessageArrived;";
  message += "client.connect({ onSuccess: onConnect });";
  message += "function onConnect() {";
  message += "console.log('onConnect');";
  message += "client.subscribe(sub);";
  message += "}";
  message += "const onOff = document.getElementById('onOff');";
  message += "onOff.addEventListener('mouseover', function(){ mOver(this, state); });";
  message += "onOff.addEventListener('mouseout', function(){ mOut(this, state); });";
  message += "const modes = document.getElementById('modes');";
  message += "modes.addEventListener('mouseover', function(){ mOver(this); });";
  message += "modes.addEventListener('mouseout', function(){ mOut(this); });";
  message += "const mode1 = document.createElement('button');";
  message += "const mode2 = document.createElement('button');";
  message += "const mode3 = document.createElement('button');";
  message += "const mode4 = document.createElement('button');";
  message += "modeInit(mode1, 'gradient', 'Gradient');";
  message += "modeInit(mode2, 'perlin_noise', 'Perlin noise');";
  message += "modeInit(mode3, 'circles', 'Circles');";
  message += "modeInit(mode4, 'sinusoid', 'Sinusoid');";
  message += "let modeArray = [mode1, mode2, mode3, mode4];";
  message += "mode1.addEventListener('click', function(){ chooseMode(this, 1); });";
  message += "mode2.addEventListener('click', function(){ chooseMode(this, 2); });";
  message += "mode3.addEventListener('click', function(){ chooseMode(this, 3); });";
  message += "mode4.addEventListener('click', function(){ chooseMode(this, 4); });";
  message += "const brightnessRange = document.getElementById('brightnessRange');";
  message += "const colorRange = document.getElementById('colorRange');";
  message += "brightnessRange.value = brightness;";
  message += "colorRange.value = colorVal;";
  message += "function mOver(obj, state = 0) {";
  message += "if(state){";
  message += "obj.style.backgroundColor = 'green';";
  message += "obj.style.borderColor = 'green';";
  message += "} else {";
  message += "obj.style.backgroundColor = 'rgb(0, 71, 179)';";
  message += "obj.style.borderColor = 'rgb(0, 71, 179)';";
  message += "}";
  message += "}";
  message += "function mOut(obj, state = 0) {";
  message += "if(state){";
  message += "obj.style.backgroundColor = 'rgb(0, 163, 8)';";
  message += "obj.style.borderColor = 'rgb(0, 163, 8)';";
  message += "} else {";
  message += "obj.style.backgroundColor = 'rgb(0, 102, 255)';";
  message += "obj.style.borderColor = 'rgb(0, 102, 255)';";
  message += "}";
  message += "}";
  message += "function rgbColors(colorVal) {";
  message += "let colorVal2 = colorVal / (1023 / (3 * Math.PI / 2));";
  message += "if(colorVal2 >= Math.PI / 2) Rcolor = Math.sin(colorVal2 + Math.PI) * 255;";
  message += "else Rcolor = Math.sin(colorVal2 + (Math.PI / 2)) * 255;";
  message += "if(Rcolor < 0) Rcolor = 0;";
  message += "Gcolor = Math.sin(colorVal2) * 255;";
  message += "if(Gcolor < 0) Gcolor = 0;";
  message += "Bcolor = Math.sin(colorVal2 + 3 * Math.PI / 2) * 255;";
  message += "if(Bcolor < 0) Bcolor = 0;";
  message += "}";
  message += "rgbColors(colorVal);";
  message += "colorDisplay.style.backgroundColor = `rgb(${Rcolor},${Gcolor},${Bcolor})`;";
  message += "onOff.addEventListener('click', function() {";
  message += "if(state == 1) state = 0;";
  message += "else state = 1;";
  message += "onOffColor();";
  message += "sendMessage();";
  message += "});";
  message += "function onOffColor() {";
  message += "if(state == 1) {";
  message += "onOff.textContent = 'On';";
  message += "onOff.style.backgroundColor = 'green';";
  message += "onOff.style.borderColor = 'green';";
  message += "} else {";
  message += "onOff.textContent = 'Off';";
  message += "modeArray.forEach(function(element) {";
  message += "element.setAttribute('data-state', 'false');";
  message += "});";
  message += "onOff.style.backgroundColor = 'rgb(0, 102, 255)';";
  message += "onOff.style.borderColor = 'rgb(0, 102, 255)';";
  message += "modeArray.forEach(function(element) {";
  message += "element.style.backgroundColor = 'rgb(0, 102, 255)';";
  message += "element.style.borderColor = 'rgb(0, 102, 255)';";
  message += "});";
  message += "}";
  message += "}";
  message += "brightnessRange.addEventListener('input', function(){";
  message += "brightnessRange.addEventListener('mouseup', function(){";
  message += "brightness = brightnessRange.value;";
  message += "sendMessage();";
  message += "});";
  message += "brightnessRange.addEventListener('touchend', function(){";
  message += "brightness = brightnessRange.value;";
  message += "sendMessage();";
  message += "});";
  message += "});";
  message += "colorRange.addEventListener('input', function(){";
  message += "rgbColors(colorRange.value);";
  message += "colorDisplay.style.backgroundColor = `rgb(${Rcolor},${Gcolor},${Bcolor})`;";
  message += "colorRange.addEventListener('mouseup', function(){";
  message += "colorVal = colorRange.value;";
  message += "sendMessage();";
  message += "});";
  message += "colorRange.addEventListener('touchend', function(){";
  message += "colorVal = colorRange.value;";
  message += "sendMessage();";
  message += "});";
  message += "});";
  message += "modes.addEventListener('click', function(){";
  message += "if(!modeState){";
  message += "modeState = !modeState;";
  message += "document.getElementById('DIV').appendChild(mode1);";
  message += "document.getElementById('DIV').appendChild(mode2);";
  message += "document.getElementById('DIV').appendChild(mode3);";
  message += "document.getElementById('DIV').appendChild(mode4);";
  message += "}";
  message += "else{";
  message += "modeState = !modeState;";
  message += "mode1.remove();";
  message += "mode2.remove();";
  message += "mode3.remove();";
  message += "mode4.remove();";
  message += "}";
  message += "});";
  message += "function modeInit(obj, id, text, lMode){";
  message += "obj.setAttribute('id', id);";
  message += "obj.textContent = text;";
  message += "obj.classList.add('modes');";
  message += "obj.addEventListener('mouseover', function(){mOver(this, this.getAttribute('data-state') === 'true');});";
  message += "obj.addEventListener('mouseout', function(){mOut(this, this.getAttribute('data-state') === 'true');});";
  message += "}";
  message += "function chooseMode(obj, lMode){";
  message += "if(state){";
  message += "let currentState = obj.getAttribute('data-state');";
  message += "currentState = currentState === 'true' ? 'false' : 'true';";
  message += "obj.setAttribute('data-state', currentState);";
  message += "if (currentState === 'true') {";
  message += "modeArray.forEach(function (element) {";
  message += "element.style.backgroundColor = 'rgb(0, 102, 255)';";
  message += "element.style.borderColor = 'rgb(0, 102, 255)';";
  message += "});";
  message += "obj.style.backgroundColor = 'green';";
  message += "obj.style.borderColor = 'green';";
  message += "modeArray.forEach(function (element) {";
  message += "if (element !== obj) {";
  message += "element.setAttribute('data-state', 'false');";
  message += "}";
  message += "});";
  message += "lightMode = lMode;";
  message += "}";
  message += "else {";
  message += "obj.style.backgroundColor = 'rgb(0, 102, 255)';";
  message += "obj.style.borderColor = 'rgb(0, 102, 255)';";
  message += "lightMode = 0;";
  message += "}";
  message += "sendMessage();";
  message += "}";
  message += "}";
  message += "function sendMessage(){";
  message += "let variables = lightMode.toString() + buttonState.toString() + state.toString() + numbers(brightness, 3) + numbers(colorVal, 4);";
  message += "if(lastMessage != variables){";
  message += "message = new Paho.MQTT.Message(variables);";
  message += "message.destinationName = sub;";
  message += "client.send(message);";
  message += "}";
  message += "lastMessage = variables;";
  message += "}";
  message += "function numbers(value, width){";
  message += "let numStr = value.toString();";
  message += "let zeros = width - numStr.length;";
  message += "let result = ''; ";
  message += "for (let i = 0; i < zeros; i++) {";
  message += "result += '0';";
  message += "}";
  message += "return result + numStr;";
  message += "}";
  message += "function onConnectionLost(responseObject){";
  message += "if(responseObject != 0){";
  message += "console.log('Connection lost', responseObject.errorMessage);";
  message += "}";
  message += "}";
  message += "function onMessageArrived(message){";
  message += "console.log('Arrived message', message.payloadString);";
  message += "lightMode = parseInt(message.payloadString[0]);";
  message += "buttonState = parseInt(message.payloadString[1]);";
  message += "state = parseInt(message.payloadString[2]);";
  message += "if(state == 0) lightMode = 0;";
  message += "onOffColor();";
  message += "lastMessage = lightMode.toString() + buttonState.toString() + state.toString() + numbers(brightness, 3) + numbers(colorVal, 4);";
  message += "}";
  message += "});";
  message += "</script>";
  message += "</html>";
  server.send(200, "text/html", message);
}
   
