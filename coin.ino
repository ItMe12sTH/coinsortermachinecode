//const int coinPin2 = 25;//cSum
#define coinPin3 34//1
#define coinPin4 35//2
#define coinPin5 32//5
#define coinPin6 33//10
int coinStateSum = 0;
int coinState1 = 0;
int coinState2 = 0;
int coinState5 = 0;
int coinState10 = 0;
long coinSum = 0, coin1b = 0, coin2b = 0, coin5b = 0, coin10b = 0;

boolean isCounterSum = false, isCounter1 = false, isCounter2 = false, isCounter5 = false, isCounter10 = false;
long countSum = 0, count1b = 0, count2b = 0, count5b = 0, count10b = 0;
boolean cSum = true, c1 = true, c2 = true, c5 = true, c10 = true;

void setup() {
  //pinMode(coinPin2, INPUT);
  pinMode(coinPin3, INPUT);
  pinMode(coinPin4, INPUT);
  pinMode(coinPin5, INPUT);
  pinMode(coinPin6, INPUT);
  Serial.begin(115200); /*Set the baudrate to 115200*/
}

void CoinSum() {
   /*------------------Coin Sum ------------------ */
    countSum++;
    Serial.print("Coin Sum : ");
    Serial.println(countSum);
    unsigned long coinAll = (count1b * 1) + (count2b * 2) + (count5b * 5) + (count10b * 10);
    Serial.print("Sum : ");
    Serial.print(coinAll);
    Serial.println("Bath");
}

void loop() {
  delay(1);
  //coinStateSum = digitalRead(coinPin2);
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
}
