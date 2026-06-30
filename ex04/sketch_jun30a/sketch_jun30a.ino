#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 200
#define DEBOUNCE_MS 300   // 加长防抖间隔，杜绝连续误触

bool ledState = false;
volatile bool touchIntFlag = false;
unsigned long lastTouchTime = 0;
bool isTouchHolding = false; // 标记是否正在触摸

void gotTouch() {
  touchIntFlag = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  unsigned long now = millis();
  int touchRaw = touchRead(TOUCH_PIN);
  bool curTouch = (touchRaw < THRESHOLD);

  // 检测到触摸中断
  if (touchIntFlag) {
    // 防抖+必须是刚碰到、之前没按住
    if ((now - lastTouchTime > DEBOUNCE_MS) && curTouch && !isTouchHolding) {
      isTouchHolding = true; // 标记进入长按状态
    }
    touchIntFlag = false;
  }

  // 检测松手：之前按住，现在松开 → 执行翻转
  if (isTouchHolding && !curTouch) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastTouchTime = now;
    isTouchHolding = false; // 清除按住标记
  }

  delay(20);
}