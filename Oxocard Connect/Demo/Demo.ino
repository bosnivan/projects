#include <ezButton.h>
#include <SPI.h>
#include <TFT_eSPI.h>

const uint8_t LCD_LED = 19;
const uint8_t BUTTON_MIDDLE = 2;
const uint8_t BUTTON_UP = 39;
const uint8_t BUTTON_DOWN = 38;
const uint8_t BUTTON_LEFT = 36;
const uint8_t BUTTON_RIGHT = 37;

TFT_eSPI tft = TFT_eSPI();
ezButton middle(BUTTON_MIDDLE);
ezButton up(BUTTON_UP);
ezButton down(BUTTON_DOWN);
ezButton left(BUTTON_LEFT);
ezButton right(BUTTON_RIGHT);

void setup() {
  pinMode(2, INPUT);  // needed for middle button
  pinMode(LCD_LED, OUTPUT);
  digitalWrite(LCD_LED, HIGH);
  tft.init();
  tft.setRotation(1);
  printOnScreen("Oxocard");
}

void loop() {
  middle.loop();
  if (middle.isReleased()) {
    printOnScreen("MIDDLE");
  }
  up.loop();
  if (up.isReleased()) {
    printOnScreen("UP");
  }
  down.loop();
  if (down.isReleased()) {
    printOnScreen("DOWN");
  }
  left.loop();
  if (left.isReleased()) {
    printOnScreen("LEFT");
  }
  right.loop();
  if (right.isReleased()) {
    printOnScreen("RIGHT");
  }
}

void printOnScreen(String text) {
  tft.fillScreen(TFT_BLACK);
  tft.startWrite();
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(5);
  tft.println(text);
  tft.endWrite();
}
