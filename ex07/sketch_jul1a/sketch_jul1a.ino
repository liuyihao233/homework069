#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "OPPOA5";
const char* password = "12345678";

WebServer server(80);
const int ledPin = 2;
int brightness = 0;

// 首页路由
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>EX07 网页无极调光</title>
  <style>
    body{font-family:Arial; text-align:center; margin-top:50px;}
    input[type="range"]{width:80%; height:25px; margin:30px 0;}
    #showVal{font-size:40px; color:#0066cc; font-weight:bold;}
  </style>
</head>
<body>
  <h1>ESP32 PWM无极调光</h1>
  <input type="range" min="0" max="255" id="slider" value="0">
  <p>当前亮度值：<span id="showVal">0</span></p>

  <script>
    let slider = document.getElementById("slider");
    let valText = document.getElementById("showVal");
    slider.oninput = function(){
      let num = this.value;
      valText.innerText = num;
      // GET请求发送亮度
      fetch("/setled?b="+num);
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

// 处理亮度设置接口
void handleSetLed() {
  if(server.hasArg("b")){
    brightness = server.arg("b").toInt();
    brightness = constrain(brightness, 0, 255);
    analogWrite(ledPin, brightness);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  Serial.begin(115200);
  
  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setled", handleSetLed);
  server.begin();
}

void loop() {
  server.handleClient();
}
