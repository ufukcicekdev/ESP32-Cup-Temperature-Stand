#define ESP32_ARDUINO_NO_RGB_BUILTIN

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>

///// PIN AYARLARI /////
#define I2C_SDA 8
#define I2C_SCL 9

#define LED_PIN 6
#define NUM_LEDS 16
#define LED_BRIGHTNESS 40

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_AHTX0 aht;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CRGB leds[NUM_LEDS];

// ---- I2C scanner (returns 0 if none found) ----
uint8_t scanI2C() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  Serial.println("I2C scan start...");
  uint8_t found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t err = Wire.endTransmission();
    if (err == 0) {
      Serial.printf("I2C device found at 0x%02X\n", addr);
      found = addr;
      if (addr == 0x3C || addr == 0x3D) return addr;
    }
  }
  return found;
}

// sıcaklığa göre renk üret
CRGB tempToColor(float tC) {
  float v = constrain(tC, 0, 80);
  uint8_t hue = map(v, 0, 80, 160, 0);
  return CHSV(hue, 255, 200);
}

// animasyon: sırayla LED yanma
int currentIndex = 0;
unsigned long animTimer = 0;
void animateLeds(float tC) {
  if (millis() - animTimer < 80) return;
  animTimer = millis();

  CRGB color = tempToColor(tC);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[currentIndex] = color;
  FastLED.show();

  currentIndex++;
  if (currentIndex >= NUM_LEDS) currentIndex = 0;
}

void setup() {
  Serial.begin(115200);
  delay(200);

  uint8_t i2cAddr = scanI2C();
  if (i2cAddr == 0) {
    Serial.println("No I2C devices found!");
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    while (1) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show(); delay(300);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show(); delay(300);
    }
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddr)) {
    Serial.println("Display init failed.");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (!aht.begin()) {
    Serial.println("AHT10 not found (continue)");
  } else {
    Serial.println("AHT10 OK");
  }

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

unsigned long sensorTimer = 0;
unsigned long messageTimer = 0;

float lastTemp = 25;
int messageIndex = 0;

// ---- ORTALAMA FONKSİYONU ----
void centerText(const char* text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = (128 - w) / 2;
  display.setCursor(x, y);
}

void loop() {

  // --- HER 1 SANIYE SICAKLIK OKU ---
  if (millis() - sensorTimer >= 1000) {
    sensorTimer = millis();

    sensors_event_t humidity, tempEv;
    if (aht.getEvent(&humidity, &tempEv)) {
      lastTemp = tempEv.temperature;
    }

    Serial.printf("Temp: %.1f C\n", lastTemp);
  }

  // --- OLED EKRANI HER 1.5 SN'DE GÜNCELLE ---
  if (millis() - messageTimer >= 1500) {
    messageTimer = millis();

    display.clearDisplay();

    // ÜST SATIR: SICAKLIK
    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "Temp: %.1f C", lastTemp);
    centerText(tempStr, 0);
    display.print(tempStr);

    // ALT SATIR: HOT / COLD + mesajlar dönüşümlü
    display.setCursor(0, 16);
    if (lastTemp >= 30) {
      centerText("HOT", 16);
      display.print("HOT");
    } else {
      centerText("COLD", 16);
      display.print("COLD");
    }

    // Ek mesajlar dönüşümlü olarak 2. satıra ekleniyor
    display.setCursor(0, 24);

    if (messageIndex == 0) {
      centerText("Thank you for watching", 24);
      display.print("Thank you for watching");
    } 
    else if (messageIndex == 1) {
      centerText("Abone olmayi unutmayin!", 24);
      display.print("Abone olmayi unutmayin!");
    }

    messageIndex++;
    if (messageIndex > 1) messageIndex = 0;

    display.display();
  }

  // LED animasyonu sürekli çalışacak
  animateLeds(lastTemp);
}
