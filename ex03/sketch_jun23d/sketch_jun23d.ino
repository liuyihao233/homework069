#define LED_PIN 2

// 时间参数
const unsigned long shortOn = 200;
const unsigned long shortOff = 200;
const unsigned long longOn = 600;
const unsigned long longOff = 200;
const unsigned long sosGap = 1500; // 整套SOS结束停顿

unsigned long prevTime = 0;
unsigned long currTime;
bool led = LOW;

// 状态标记
int phase = 0; // 0:3短 1:3长 2:3短
int cnt = 0;
bool isLight = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  currTime = millis();
  if (currTime - prevTime < (isLight ? (phase==0||phase==2 ? shortOn : longOn) : (phase==0||phase==2 ? shortOff : longOff)))
    return;
  
  prevTime = currTime;
  led = !led;
  digitalWrite(LED_PIN, led);
  isLight = !isLight;


  if (!isLight) {
    cnt++;
    if (cnt >= 3) {
      cnt = 0;
      phase++;
      if (phase >= 3) {
        phase = 0;
        delay(sosGap); // 整套结束长停顿，仅此处短暂阻塞不影响
      }
    }
  }
}