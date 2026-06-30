#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 200
#define DEBOUNCE_MS 50

volatile bool touchTrigger = false;
volatile unsigned long lastTouchTime = 0;
int speedGear = 1; // 1/2/3档

int brightness = 0;
int step = 4;
bool upDir = true;

void gotTouch() {
  touchTrigger = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  unsigned long now = millis();
  // 触摸触发切换档位
  if (touchTrigger) {
    if (now - lastTouchTime > DEBOUNCE_MS) {
      speedGear++;
      if (speedGear > 3) speedGear = 1;
      Serial.print("当前档位：");
      Serial.println(speedGear);
      lastTouchTime = now;
    }
    touchTrigger = false;
  }

  // 根据档位设置呼吸速度
  int breathDelay;
  switch (speedGear) {
    case 1: breathDelay = 40; break; // 慢速
    case 2: breathDelay = 18; break; // 中速
    case 3: breathDelay = 6; break;  // 快速
  }

  // 呼吸渐变
  analogWrite(LED_PIN, brightness);
  if (upDir) {
    brightness += step;
    if (brightness >= 255) upDir = false;
  } else {
    brightness -= step;
    if (brightness <= 0) upDir = true;
  }
  delay(breathDelay);
}