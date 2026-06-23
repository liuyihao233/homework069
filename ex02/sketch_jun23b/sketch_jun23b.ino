// ex02 1Hz LED闪烁 基于millis非阻塞
#define LED_PIN 2
unsigned long prevTime = 0;
const unsigned long interval = 500; // 500ms翻转一次，1Hz
bool ledState = LOW;

void setup() {
 
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  unsigned long currTime = millis();
  // 判断是否达到间隔时间
  if (currTime - prevTime >= interval) {
    prevTime = currTime;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}