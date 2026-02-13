# ESP32 FreeRTOS Sensor Dashboard (Wokwi)

This project demonstrates a **FreeRTOS-based embedded system** using an **ESP32**, simulated in **Wokwi**.

The system reads data from an **MPU6050 accelerometer/gyroscope** and a **potentiometer (ADC)**, and displays the results on an **SSD1306 OLED display**.  
A **push button interrupt** is used to switch between display modes.

---

##  Features

- ESP32 Arduino Core
- FreeRTOS multitasking
- MPU6050 (I2C)
- SSD1306 OLED (I2C)
- Potentiometer ADC input
- GPIO interrupt (push button)
- Mode switching using ISR
- Wokwi simulation compatible

---

##  System Architecture

**Tasks:**
- `taskMPU` – Reads MPU6050 sensor data
- `taskADC` – Reads ADC value from potentiometer
- `taskDisplay` – Updates OLED display
- Button ISR toggles display mode

---

##  Pin Connections

### I2C (Shared Bus)
| Device | ESP32 |
|------|------|
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### ADC
| Device | ESP32 |
|------|------|
| Potentiometer SIG | GPIO 34 |

### Button
| Device | ESP32 |
|------|------|
| Push Button | GPIO 15 |
| Other side | GND |

---

##  Display Modes

### Mode 1 – MPU6050
- Acceleration X, Y, Z

### Mode 2 – ADC
- Raw ADC value (0–4095)

Button press toggles between modes.

---

##  How to Run (Wokwi)

1. Open the project in Wokwi
2. Ensure required libraries are installed
3. Start simulation
4. Use:
   - Potentiometer → change ADC value
   - Button → switch display mode
   - MPU6050 controls → change accelerometer values

---

##  Notes

- SSD1306 OLED was chosen instead of SPI TFT due to better FreeRTOS compatibility in Wokwi
- ESP32 ADC is explicitly configured for 12-bit resolution
- Push button wired diagonally to ensure correct contact usage

---

## 📌 Author

**Pokala Bharath Vamsi**

---


