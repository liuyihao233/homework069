#define LED_A 2
#define LED_B 4

int pwmVal = 0;
int step = 2;

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  analogWrite(LED_A, pwmVal);
  analogWrite(LED_B, 255 - pwmVal);

  pwmVal += step;
  if (pwmVal >= 255) {
    pwmVal = 0;
  }
  delay(8);
}