#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define Arial_24 &FreeSans9pt7b
#define Arial_48 &FreeSans12pt7b
#define Arial_60 &FreeSans18pt7b
#define ILI9341_BLACK  0x0000
#define ILI9341_RED    0xF800
#define ILI9341_GREEN  0x07E0
#define ILI9341_YELLOW 0xFFE0
#define CS_PIN  4  //Waveshare Touch
#define TFT_DC  7  //Waveshare
#define TFT_CS 10  //Waveshare
#define TFT_BL  9  //Waveshare backlight
// MOSI=11, MISO=12, SCK=13
#define TIRQ_PIN  2

XPT2046_Touchscreen ts(CS_PIN);
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_BL);

// List of items
const char* items[] = { "Yumurta", "Pekmez", "Sut", "Zeytin" };

// Normal range for each item
const char* normalRanges[] = { "40-50 mg/dL", "100-200 mg/dL", "70-100 mg/dL", "40-60 mg/dL" };

// Current item index
int currentItemIndex = 0;

// Flag to indicate whether the item list is shown
bool itemListShown = true;

void setup() {
  Serial.begin(9600);
  tft.init(240, 320, SPI_MODE0);
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(1);
  while (!Serial && (millis() <= 1000));
}

void loop() {
  // Check if the screen is touched
  if (ts.touched()) {
    // Wait for the touch to be released before processing
    while (ts.touched()) {}

    // Toggle between item list and normal range display
    itemListShown = !itemListShown;

    // If we're showing the normal range, update the current item index
    if (!itemListShown) {
      currentItemIndex = (currentItemIndex + 1) % 4; // Rotate between 0-3
    }
  }

  // Display the appropriate screen based on itemListShown flag
  if (itemListShown) {
    // Display the item list
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(Arial_24);

    for (int i = 0; i < 4; i++) {
      tft.setCursor(60, 80 + i * 40);
      tft.print(items[i]);
    }
  } else {
    // Display the normal range for the selected item
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(Arial_60);
    tft.setCursor(60, 80);
    tft.print(items[currentItemIndex]);

    tft.setTextColor(ILI9341_GREEN);
    tft.setFont(Arial_24);
    tft.setCursor(60, 150);
    tft.print("Normal aralik: ");
    tft.print(normalRanges[currentItemIndex]);
  }
}
