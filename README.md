# SmartCupStand  
A temperature-reactive smart cup stand built with **ESP32-C3**, **AHT10 temperature & humidity sensor**, **OLED display**, and **WS2812B addressable LEDs**.  
The LEDs animate based on temperature, while the OLED shows real-time readings and status icons (Hot / Cold).

---

## ✨ Features

- 🌡️ **Real-time temperature & humidity** via AHT10  
- 💡 **Animated WS2812B LEDs** — color changes based on temperature  
- 🖥️ **OLED Display (SSD1306)** showing:
  - Temperature  
  - Humidity  
  - HOT / COLD icons (coffee cup / snowflake)
- 🔄 **Smooth LED rotation animation**
- ⚡ Built on **ESP32-C3**, fully compatible with Arduino IDE  
- 🔍 Automatic **I2C address scanning** for OLED

---

## 📦 Hardware Used
- ESP32-C3 (DevKit or similar)
- AHT10 sensor module
- SSD1306 OLED Display (128x32)
- WS2812B LED strip (8 or 16 LEDs)
- USB-C cable
- Jumper wires

---

## 🔌 Wiring Diagram (Quick Overview)

| Component | ESP32-C3 Pin |
|----------|--------------|
| AHT10 SDA | GPIO 8 |
| AHT10 SCL | GPIO 9 |
| OLED SDA | GPIO 8 |
| OLED SCL | GPIO 9 |
| WS2812B Data | GPIO 6 |
| 5V (LEDs) | 5V |
| GND | GND |

> ⚠️ WS2812B LEDs must share **common ground** with ESP32-C3.

---

## 🎨 LED Behavior
The LEDs change color depending on temperature:

- **Cold (0–20°C)** → Blue tones  
- **Warm (20–35°C)** → Green → Yellow transition  
- **Hot (35–80°C)** → Orange → Red  

Additionally, one LED rotates along the strip to create a modern "loading" effect.

---

## 🖥️ OLED Display Behavior
The OLED shows:

