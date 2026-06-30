#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "OPPOA5";
const char* password = "12345678";

WebServer server(80);
const int touchPin = T0;

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>EX09 触摸实时仪表盘</title>
  <style>
    body{font-family:Arial; text-align:center; margin-top:80px;}
    #dataNum{font-size:90px; color:#0066cc; font-weight:bold;}
  </style>
</head>
<body>
  <h1>触摸传感器实时监控面板</h1>
  <div id="dataNum">0</div>

  <script>
    function refreshData(){
      fetch("/getdata")
      .then(res => res.text())
      .then(val => {
        document.getElementById("dataNum").innerText = val;
      })
    }
    setInterval(refreshData, 100);
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleGetData() {
  int val = touchRead(touchPin);
  server.send(200, "text/plain", String(val));
}

void setup() {
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
    Serial.println("\nWiFi连接失败，检查热点");
  }

  server.on("/", handleRoot);
  server.on("/getdata", handleGetData);
  server.begin();
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(touchPin);
  Serial.print("触摸数值：");
  Serial.println(touchVal);
}