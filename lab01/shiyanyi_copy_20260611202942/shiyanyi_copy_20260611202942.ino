// 定义LED引脚
const int ledPin = 4;  
const int ledPin_ = 5;
// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);

  // 【新版用法】直接将引脚、频率和分辨率绑定
  // 它会自动返回一个关联的通道（如果需要的话）
  ledcAttach(ledPin, freq, resolution);
  ledcAttach(ledPin_, freq, resolution);
}

void loop() {
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // 【新版用法】直接通过引脚号写入，不再需要指定通道
    ledcWrite(ledPin, dutyCycle);
    ledcWrite(ledPin_, dutyCycle);   
    delay(10);
  }

  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledPin, dutyCycle);
    ledcWrite(ledPin_, dutyCycle);   
    delay(10);
  }
  
  Serial.println("Breathing cycle completed");
}