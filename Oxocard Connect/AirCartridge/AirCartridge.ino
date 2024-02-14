#include <Adafruit_SHT4x.h>     // Adafruit SHT4x Library
#include <SensirionI2CScd4x.h>  // Sensirion I2C SCD4x
#include <SensirionI2CSgp41.h>  // Sensirion I2C SGP41
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>

const uint8_t LCD_LED = 19;
const uint16_t defaultRh = 0x8000;
const uint16_t defaultT = 0x6666;

uint16_t vocConditioning = 2;  // 10 seconds

TFT_eSPI tft = TFT_eSPI();
Adafruit_SHT4x sht40 = Adafruit_SHT4x();
SensirionI2CScd4x scd40;
SensirionI2CSgp41 sgp41;

void setup() {
  pinMode(LCD_LED, OUTPUT);
  digitalWrite(LCD_LED, HIGH);
  Wire.begin();
  // SHT40
  sht40.begin();
  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  sht40.setHeater(SHT4X_NO_HEATER);
  // SCD40
  scd40.begin(Wire);
  scd40.stopPeriodicMeasurement();
  scd40.startPeriodicMeasurement();
  // SGP41
  sgp41.begin(Wire);
  // Display
  tft.init();
  tft.setRotation(1);
}

void loop() {
  // SHT40
  sensors_event_t hEvent, tEvent;
  sht40.getEvent(&hEvent, &tEvent);
  // SCD40
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;
  scd40.readMeasurement(co2, temperature, humidity);
  // SGP41
  uint16_t rawVoc = 0;
  uint16_t rawNox = 0;
  if (vocConditioning == 0) {
    sgp41.measureRawSignals(defaultRh, defaultT, rawVoc, rawNox);
  } else {
    sgp41.executeConditioning(defaultRh, defaultT, rawVoc);
    vocConditioning--;
  }
  // Display
  tft.fillScreen(TFT_BLACK);
  tft.startWrite();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 0);
  tft.println("SHT40");
  tft.setTextSize(2);
  tft.setCursor(5, 25);
  tft.println("Temperature: " + String(tEvent.temperature));
  tft.setCursor(5, 45);
  tft.println("Humidity: " + String(hEvent.relative_humidity));
  tft.setTextSize(3);
  tft.setCursor(20, 65);
  tft.println("SCD40");
  tft.setTextSize(2);
  tft.setCursor(5, 90);
  tft.println("CO2: " + String(co2));
  tft.setCursor(5, 110);
  tft.println("Temperature: " + String(temperature));
  tft.setCursor(5, 130);
  tft.println("Humidity: " + String(humidity));
  tft.endWrite();
  tft.setTextSize(3);
  tft.setCursor(20, 150);
  tft.println("SGP41");
  tft.setTextSize(2);
  tft.setCursor(5, 175);
  tft.println("VOC (raw): " + String(rawVoc));
  tft.setCursor(5, 195);
  tft.println("NOx (raw): " + String(rawNox));
  delay(5000);
}
