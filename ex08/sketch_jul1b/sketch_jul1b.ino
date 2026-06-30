#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "OPPOA5";
const char* password = "12345678";

WebServer server(80);
const int ledPin = 2;
const int touchPin = T0; // GPIO4触摸引脚

bool systemArm = false;   // 是否布防
bool alarmLock = false;   // 报警锁定状态

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>EX08 物联网安防报警器</title>
  <style>
    body{font-family:Arial; text-align:center; margin-top:50px;}
    button{font-size:24px; padding:15px 40px; margin:20px;}
    #tip{font-size:18px; color:#666;}
  </style>
</head>
<body>
  <h1>ESP32安防报警主机</h1>
  <button onclick="fetch('/arm')">布防 Arm</button>
  <button onclick="fetch('/disarm')">撤防 Disarm</button>
  <p id="tip">布防后触摸引脚触发持续报警，仅撤防可关闭</p>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

// 布防接口
void handleArm() {
  systemArm = true;
  server.send(200, "text/plain", "已开启布防");
}
// 撤防，清除报警
void handleDisarm() {
  systemArm = false;
  alarmLock = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "已撤防，报警复位");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.print("正在连接WiFi【");
  Serial.print(ssid);
  Serial.print("】");
  WiFi.begin(ssid, password);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\nWiFi连接成功");
    Serial.print("访问地址：http://");
    Serial.println(WiFi.localIP());
  }else{
    Serial.println("\nWiFi连接失败");
  }

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(touchPin);
  Serial.print("触摸原始数值：");
  Serial.println(touchVal);

  // 修改阈值为200：触摸后数值小于200就触发报警锁定
  if(systemArm && touchVal < 200 && !alarmLock){
    alarmLock = true;
  }

  // 报警时LED高频闪烁
  if(alarmLock){
    digitalWrite(ledPin, HIGH);
    delay(80);
    digitalWrite(ledPin, LOW);
    delay(80);
  }
}