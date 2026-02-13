#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ================= CONSTANTS ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define I2C_SDA   21
#define I2C_SCL   22

#define ADC_PIN   34
#define BTN_PIN   15

/* ================= OBJECTS ================= */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MPU6050 mpu;

/* ================= GLOBAL DATA ================= */
volatile bool displayMode = true;   // true = MPU, false = ADC
volatile int adcValue = 0;

sensors_event_t accel, gyro, temp;

/* ================= BUTTON ISR ================= */
void IRAM_ATTR buttonISR() {
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  if (now - lastTime > 300) {
    displayMode = !displayMode;
    lastTime = now;
  }
}

/* ================= TASK: MPU6050 ================= */
void taskMPU(void *pvParameters) {
  while (1) {
    mpu.getEvent(&accel, &gyro, &temp);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

/* ================= TASK: ADC ================= */
void taskADC(void *pvParameters) {
  while (1) {
    adcValue = analogRead(ADC_PIN);
    Serial.print("ADC RAW: ");
    Serial.println(adcValue);
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

/* ================= TASK: DISPLAY ================= */
void taskDisplay(void *pvParameters) {

  vTaskDelay(pdMS_TO_TICKS(500)); // allow I2C to stabilize

  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    if (displayMode) {
      display.println("MPU6050 DATA");
      display.print("AX: "); display.println(accel.acceleration.x);
      display.print("AY: "); display.println(accel.acceleration.y);
      display.print("AZ: "); display.println(accel.acceleration.z);
    } else {
      display.println("ADC MODE");
      display.print("Value: ");
      display.println(adcValue);
    }

    display.display();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  /* ---- ADC CONFIG (CRITICAL FOR ESP32 + WOKWI) ---- */
  analogReadResolution(12);        // 0–4095
  analogSetAttenuation(ADC_11db);  // Full 0–3.3V range

  /* ---- Button ---- */
  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(BTN_PIN, buttonISR, FALLING);

  /* ---- I2C ---- */
  Wire.begin(I2C_SDA, I2C_SCL);

  /* ---- OLED ---- */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 NOT FOUND");
    while (1);
  }

  /* ---- MPU6050 ---- */
  if (!mpu.begin()) {
    Serial.println("MPU6050 NOT FOUND");
    while (1);
  }

  display.clearDisplay();
  display.display();

  /* ---- FreeRTOS Tasks ---- */
  xTaskCreate(taskMPU, "MPU Task", 4096, NULL, 2, NULL);
  xTaskCreate(taskADC, "ADC Task", 2048, NULL, 1, NULL);
  xTaskCreate(taskDisplay, "Display Task", 4096, NULL, 1, NULL);
}

/* ================= LOOP ================= */
void loop() {
  vTaskDelay(portMAX_DELAY); // FreeRTOS runs everything
}
