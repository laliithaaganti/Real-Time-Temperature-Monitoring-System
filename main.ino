#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 sensor settings
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialize OLED display (Address 0x3C is common for 128x64)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Temp Monitor System");
  display.display();
  delay(2000);
}

void loop() {
  // Reading humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
  float f = dht.readTemperature(true); // Fahrenheit

  // Check if any reads failed
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Calculate Heat Index (Required per project scope) [cite: 121]
  float hic = dht.computeHeatIndex(t, h, false);

  // Displaying data on OLED [cite: 122]
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Environmental Data:");
  
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(t);
  display.print(" C / ");
  display.print(f);
  display.println(" F");

  display.setCursor(0, 40);
  display.print("Humidity: ");
  display.print(h);
  display.println(" %");

  display.setCursor(0, 55);
  display.print("Heat Index: ");
  display.print(hic);
  display.println(" C");

  display.display();

  // Wait 2 seconds between measurements
  delay(2000);
}
