//screen lcd
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
//networking
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Internet setting
const char* ssid = "name";
const char* password = "password";
//Your Domain name with URL path or IP address with path
const char* serverName = "servername";

//lcd screen
#define TFT_CS 22
#define TFT_RST 26
#define TFT_DC 27
#define TFT_MOSI 23
#define TFT_SCLK 18
//lcd screen caller
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
//TSC3200 sensor
#define S2 2         /*Define S2 Pin Number of ESP32*/
#define S3 4         /*Define S3 Pin Number of ESP32*/
#define sensorOut 15 /*Define Sensor Output Pin Number of ESP32*/
//coin vars
#define coinPin3 34 //1
#define coinPin4 35 //2
#define coinPin5 32 //5
#define coinPin6 33 //10
int coinStateSum = 0;
int coinState1 = 0;
int coinState2 = 0;
int coinState5 = 0;
int coinState10 = 0;
long coinSum = 0, coin1b = 0, coin2b = 0, coin5b = 0, coin10b = 0;

bool isCounterSum = false, isCounter1 = false, isCounter2 = false, isCounter5 = false, isCounter10 = false;
bool cSum = true, c1 = true, c2 = true, c5 = true, c10 = true;
//total coin
long countSum = 0, count1b = 0, count2b = 0, count5b = 0, count10b = 0;
//total banknote
long bank_countSum = 0, bank_20 = 0, bank_50 = 0, bank_100 = 0, bank_500 = 0, bank_1000 = 0;
// (min -> max)
// bank type : Red | Green | Blue
// 20 : 213-234 | 215-235 | 207-231
// 50 : 213-234 | 215-235 | 231-243
// 100 : 224-234 | 204-225 | 207-231
// 500 : 204-230 | 204-225 | 207-231
// 1000 : 217-243 | 215-235 | 207-243
/*Define int variables*/
int red20min = 224, red20max = 234, green20min = 225, green20max = 235, blue20min = 219, blue20max = 231;
int red50min = 224, red50max = 234, green50min = 225, green50max = 235, blue50min = 231, blue50max = 243;
int red100min = 234, red100max = 234, green100min = 215, green100max = 215, blue100min = 219, blue100max = 219;
int red500min = 234, red500max = 234, green500min = 215, green500max = 215, blue500min = 231, blue500max = 231;//locked for demo UNLOCKED
int red1000min = 999, red1000max = 999, green1000min = 999, green1000max = 999, blue1000min = 999, blue1000max = 999;//locked for demo
int red, green, blue;
int Frequency = 0;
//callibrate
int redMin = 3;
int greenMin = 3;
int blueMin = 3;
int redMax = 27;
int greenMax = 28;
int blueMax = 24;

//scroll down skip all of reading this
//CUSTOM FUNCTION DOWN HERE!

/*check if a duration has lapsed
  In:
    duration (in ms)
  Returns:
    false if duration has not lapsed, else true*/
bool wait(unsigned long &since, unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  unsigned long currentmillis = millis();
  if (currentmillis - since >= time) {
    since = currentmillis;
    return true;
  }
  return false;
}

//SUM FUNCTIONS
void CoinSum() {
   /*------------------ Coin Sum ------------------ */
    countSum++;
    Serial.print("Coin Sum : ");
    Serial.println(countSum);
    long coinAll = (count1b * 1) + (count2b * 2) + (count5b * 5) + (count10b * 10);
    Serial.print("Sum : ");
    Serial.print(coinAll);
    Serial.println("Bath");
}
void BanknoteSum() {
   /*------------------ Banknote Sum ------------------ */
    bank_countSum++;
    Serial.print("Banknote Sum : ");
    Serial.println(bank_countSum);
    long bankAll = (bank_20 * 20) + (bank_50 * 50) + (bank_100 * 100) + (bank_500 * 500) + (bank_1000 * 1000);
    Serial.print("Sum : ");
    Serial.print(bankAll);
    Serial.println("Bath");
}
long totalEverything() {
  long coinAll = (count1b * 1) + (count2b * 2) + (count5b * 5) + (count10b * 10);
  long bankAll = (bank_20 * 20) + (bank_50 * 50) + (bank_100 * 100) + (bank_500 * 500) + (bank_1000 * 1000);
  return coinAll + bankAll;
}

//Display printing
void saytext(char *text, uint16_t color, int line, int size) {
  tft.setCursor(0, line);
  tft.setTextSize(size);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.print(text);
}

//RGB colors
int getRed() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Red Color Frequency*/
  //return Frequency;//copy paste to use this for calibrating
  return map(Frequency, redMin, redMax, 255, 0);
}
int getGreen() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Green Color Frequency*/
  return map(Frequency, greenMin, greenMax, 255, 0);
}
int getBlue() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Blue Color Frequency*/
  return map(Frequency, blueMin, blueMax, 255, 0);
}

void cal() {
  if (red >= red20min && red <= red20max && green >= green20min && green <= green20max && blue >= blue20min && blue <= blue20max) {
    Serial.println("BANK 20!!!");
    bank_20++;
    BanknoteSum();
    tft.fillScreen(ST77XX_RED);
    delay(6000);//Stop everything to let the banknote in
    tft.fillScreen(ST77XX_GREEN);
  }
  else if (red >= red50min && red <= red50max && green >= green50min && green <= green50max && blue >= blue50min && blue <= blue50max) {
    Serial.println("BANK 50!!!");
    bank_50++;
    BanknoteSum();
    tft.fillScreen(ST77XX_RED);
    delay(6000);
    tft.fillScreen(ST77XX_GREEN);
  }
  else if (red >= red100min && red <= red100max && green >= green100min && green <= green100max && blue >= blue100min && blue <= blue100max) {
    Serial.println("BANK 100!!!");
    bank_100++;
    BanknoteSum();
    tft.fillScreen(ST77XX_RED);
    delay(6000);
    tft.fillScreen(ST77XX_GREEN);
  }
  else if (red >= red500min && red <= red500max && green >= green500min && green <= green500max && blue >= blue500min && blue <= blue500max) {
    Serial.println("BANK 500!!!");
    bank_500++;
    BanknoteSum();
    tft.fillScreen(ST77XX_RED);
    delay(6000);
    tft.fillScreen(ST77XX_GREEN);
  }
  else if (red >= red1000min && red <= red1000max && green >= green1000min && green <= green1000max && blue >= blue1000min && blue <= blue1000max) {
    Serial.println("BANK 1000!!!");
    bank_1000++;
    BanknoteSum();
    delay(6000);
    tft.fillScreen(ST77XX_GREEN);
  }
}

//Setup internet connection
void bootNet() {
  //Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

//upload data!
void uploadData() {
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
      
    StaticJsonDocument<200> doc;//allowcate memory
    // Add values in the document
    doc["bath1"] = count1b;
    doc["bath2"] = count2b;
    doc["bath5"] = count5b;
    doc["bath10"] = count10b;

    doc["bath20"] = bank_20;
    doc["bath50"] = bank_50;
    doc["bath100"] = bank_100;
    doc["bath500"] = bank_500;
    doc["bath1000"] = bank_1000;

    doc["money"] = totalEverything();
    
    String requestBody;
    serializeJson(doc, requestBody);
    Serial.println(requestBody);
     
    int httpResponseCode = http.POST(requestBody);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

//END OF CUSTOM FUNCTION!

void setup() {
  Serial.begin(115200);
  delay(1000);
  //LCD SETUP
  tft.init(240, 240, SPI_MODE3);
  Serial.println(F("hi lcd screen"));
  //COIN SETUP
  pinMode(coinPin3, INPUT);
  pinMode(coinPin4, INPUT);
  pinMode(coinPin5, INPUT);
  pinMode(coinPin6, INPUT);
  Serial.println(F("hi coin sensor"));
  //COLOR SETUP
  pinMode(S2, OUTPUT); /*Define S2 Pin as a OUTPUT*/
  pinMode(S3, OUTPUT); /*Define S3 Pin as a OUTPUT*/
  pinMode(sensorOut, INPUT);
  Serial.println(F("hi color sensor"));
  //BOOT NET
  bootNet();
  Serial.println(F("hi IOT"));
  //FINISH SETUP
  delay(1000);
}

//ticking system to fix coin lag when merge with offsets
long redWait = false;
long greenWait = false;
long blueWait = false;

void loop() {
  //LOOP SETTER
  static unsigned long Atime, Btime, Ctime, Dtime, Etime;

  //LOOP CODE
  delay(1);
  //Serial.print("h");

  //COIN COUNTING
  coinState1 = digitalRead(coinPin3);
  coinState2 = digitalRead(coinPin4);
  coinState5 = digitalRead(coinPin5);
  coinState10 = digitalRead(coinPin6);

  /*------------------Coin 1B ------------------ */
  if (coinState1 == HIGH && c1 == true) {
    isCounter1 = true;
    c1 = false;
  }
  if (coinState1 == LOW) {
    c1 = true;
  }
  if (isCounter1 == true) {
    isCounter1 = false;
    count1b++;
    Serial.print("Coin 1B : ");
    Serial.println(count1b);
    Serial.print(count1b * 1);
    Serial.println(" Bath");

    CoinSum();
  }
  /*------------------Coin 2B ------------------ */
  if (coinState2 == HIGH && c2 == true) {
    isCounter2 = true;
    c2 = false;
  }
  if (coinState2 == LOW) {
    c2 = true;
  }
  if (isCounter2 == true) {
    isCounter2 = false;
    count2b++;
    Serial.print("Coin 2B : ");
    Serial.println(count2b);
    Serial.print(count2b * 2);
    Serial.println(" Bath");

    CoinSum();
  }
  /*------------------Coin 5B ------------------ */
  if (coinState5 == HIGH && c5 == true) {
    isCounter5 = true;
    c5 = false;
  }
  if (coinState5 == LOW) {
    c5 = true;
  }
  if (isCounter5 == true) {
    isCounter5 = false;
    count5b++;
    Serial.print("Coin 5B : ");
    Serial.println(count5b);
    Serial.print(count5b * 5);
    Serial.println(" Bath");

    CoinSum();
  }
  /*------------------Coin 10B ------------------ */
  if (coinState10 == HIGH && c10 == true) {
    isCounter10 = true;
    c10 = false;
  }
  if (coinState10 == LOW) {
    c10 = true;
  }
  if (isCounter10 == true) {
    isCounter10 = false;
    count10b++;
    Serial.print("Coin 10B : ");
    Serial.println(count10b);
    Serial.print(count10b * 10);
    Serial.println(" Bath");

    CoinSum();
  }

  //GET RGB
  if (wait(Etime,1000)) {

   if (wait(Atime,200) && redWait == false) {
     //Serial.println("getting red");
     redWait = true;
     red = getRed();
    }
   if (wait(Btime,400) && greenWait == false) {
     //Serial.println("getting green");
      greenWait = true;
      green = getGreen();
    }
    if (wait(Ctime,600) && blueWait == false) {
      //erial.println("getting blue");
      blueWait = true;
      blue = getBlue();
    }

  }
  //Wait for all color and check for bank
  if (redWait == true && greenWait == true && blueWait == true) {
    redWait = false;
    greenWait = false;
    blueWait = false;
    
    Serial.print("Red Freq = ");
    Serial.print(red); /*Print Red Color Value on Serial Monitor*/
    Serial.print("   ");
    Serial.print("Green Freq = ");
    Serial.print(green); /*Print Green Color Value on Serial Monitor*/
    Serial.print("   ");
    Serial.print("Blue Freq = ");
    Serial.println(blue); /*Print Blue Color Value on Serial Monitor*/

    cal();
  }

  if (wait(Dtime,5000)) {
    tft.fillScreen(ST77XX_BLACK);
    saytext("total money :", ST77XX_WHITE, 0, 3);
    char buf[50];
    saytext(ltoa(totalEverything(), buf, 10), ST77XX_WHITE, 50, 3);
    //free(buf);//fix memory leak oops! this is bug dont un-comment
    //code at line 200+ something
    uploadData();
  }
}
