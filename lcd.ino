#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS 22
#define TFT_RST 26
#define TFT_DC 27
#define TFT_MOSI 23
#define TFT_SCLK 18

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

/*
  check if a duration has lapsed
  In:
    duration (in ms)
  Returns:
    false if duration has not lapsed, else true
 */
boolean wait(unsigned long &since, unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  unsigned long currentmillis = millis();
  if (currentmillis - since >= time) {
    since = currentmillis;
    return true;
  }
  return false;
}

void setup() {
  tft.init(240, 240, SPI_MODE3);
  Serial.println(F("Initialized"));

  // text test
  /*tft.fillScreen(ST77XX_BLACK);
  saytext("total 1 bath coin : 999", ST77XX_WHITE, 0);
  saytext("total 2 bath coin : 999", ST77XX_WHITE, 10);*/

  delay(1000);//???
}

void loop() {
  //loop setter
  static unsigned long Dtime;

  //real loop here
  delay(1);

  if (wait(Dtime,5000)) {
    tft.fillScreen(ST77XX_BLACK);
    saytext("total 1 bath coin : 111", ST77XX_WHITE, 0);
    saytext("total 2 bath coin : 222", ST77XX_WHITE, 15);
    saytext("total 5 bath coin : 555", ST77XX_WHITE, 30);
    saytext("total 10 bath coin : 100", ST77XX_WHITE, 45);
    saytext("total 20 banknote : 2020", ST77XX_WHITE, 60);
    saytext("total 50 banknote : 5050", ST77XX_WHITE, 75);
    saytext("total 100 banknote : 1001", ST77XX_WHITE, 90);
    saytext("total 500 banknote : 5000", ST77XX_WHITE, 105);
    saytext("total 1000 banknote : 1011", ST77XX_WHITE, 120);
    saytext("total money : XXXXX", ST77XX_WHITE, 135);
  }
}

void saytext(char *text, uint16_t color, int line) {
  tft.setCursor(0, line);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.print(text);
}