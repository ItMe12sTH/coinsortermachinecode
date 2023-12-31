#define S2 2         /*Define S2 Pin Number of ESP32*/
#define S3 4         /*Define S3 Pin Number of ESP32*/
#define sensorOut 15 /*Define Sensor Output Pin Number of ESP32*/

// (min -> max)
// bank type : Red | Green | Blue
// 20 : 204-230 | 215-235 | 207-231
// 50 : 192-230 | 194-235 | 219-243
// 100 : 217-243 | 204-235 | 207-231
// 500 : 204-230 | 204-225 | 207-231
// 1000 : 217-243 | 215-235 | 207-243
/*Define int variables*/
int red20min = 204, red20max = 230, green20min = 215, green20max = 235, blue20min = 207, blue20max = 231;
int red50min = 192, red50max = 230, green50min = 194, green50max = 235, blue50min = 219, blue50max = 243;
int red100min = 217, red100max = 243, green100min = 204, green100max = 225, blue100min = 207, blue100max = 231;
int red500min = 204, red500max = 230, green500min = 204, green500max = 225, blue500min = 207, blue500max = 231;
int red1000min = 217, red1000max = 243, green1000min = 215, green1000max = 235, blue1000min = 207, blue1000max = 243;
int red, green, blue;
int Frequency = 0;

int redMin = 3;
int greenMin = 3;
int blueMin = 3;
int redMax = 28;
int greenMax = 29;
int blueMax = 24;

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

int getRed() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Red Color Frequency*/
  //return Frequency;
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
    Serial.println("AHHHHHHHH BANK 20!!!");
    delay(5000);
  }
  if (red >= red50min && red <= red50max && green >= green50min && green <= green50max && blue >= blue50min && blue <= blue50max) {
    Serial.println("AHHHHHHHH BANK 50!!!");
    delay(5000);
  }
  if (red >= red100min && red <= red100max && green >= green100min && green <= green100max && blue >= blue100min && blue <= blue100max) {
    Serial.println("AHHHHHHHH BANK 100!!!");
    delay(5000);
  }
  if (red >= red500min && red <= red500max && green >= green500min && green <= green500max && blue >= blue500min && blue <= blue500max) {
    Serial.println("AHHHHHHHH BANK 500!!!");
    delay(5000);
  }
  if (red >= red1000min && red <= red1000max && green >= green1000min && green <= green1000max && blue >= blue1000min && blue <= blue1000max) {
    Serial.println("AHHHHHHHH BANK 1000!!!");
    delay(5000);
  }

}

void setup() {
  pinMode(S2, OUTPUT); /*Define S2 Pin as a OUTPUT*/
  pinMode(S3, OUTPUT); /*Define S3 Pin as a OUTPUT*/
  pinMode(sensorOut, INPUT);
  Serial.begin(115200); /*Set the baudrate to 115200*/
  Serial.print("Hello World From Shinn! banknote na!");
}

//ticking system to fix coin lag when merge with offsets
long redWait = false;
long greenWait = false;
long blueWait = false;

void loop() {
  //Loop Setter!
  static unsigned long Atime, Btime, Ctime;

  //This loop real loop
  delay(1);
  cal();

  if (wait(Atime,200) && redWait == false) {
    Serial.println("getting red");
    redWait = true;
    red = getRed();
  }

  if (wait(Btime,400) && greenWait == false) {
    Serial.println("getting green");
    greenWait = true;
    green = getGreen();
  }

  if (wait(Ctime,600) && blueWait == false) {
    Serial.println("getting blue");
    blueWait = true;
    blue = getBlue();
  }

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
  }
}
