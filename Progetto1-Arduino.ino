#include "Utils.h"

int initialStateValue = LED1_PIN;
int initialStateIncrement = 1;
int fadeAmount = 5;
int score = 0;
long maxTime = level[0];
long currentTime;
extern int positionWhilePlaying;

void initialState() {
  score = 0;
  digitalWrite(initialStateValue, HIGH);
  digitalWrite(initialStateValue - initialStateIncrement, LOW);
  if(analogRead(POT_PIN) < 0){
    maxTime = level[0];
  } else {
    maxTime = level[analogRead(POT_PIN)/128];
  }
  if (initialStateValue == LED1_PIN) {
    initialStateIncrement = 1;
  }
  if (initialStateValue == LED3_PIN) {
    initialStateIncrement = -1;
  }
  initialStateValue += initialStateIncrement;
  delay(500);
}

void readyToPlay() {
  double time = 0;
  digitalWrite(LEDB_PIN, LOW);
  digitalWrite(LEDR_PIN, LOW);
  positionWhilePlaying = random(LED1_PIN, LED3_PIN + 1);
  digitalWrite(positionWhilePlaying, HIGH);
  state = PLAYING;
  currentTime = millis();
}

void ledToBag() {
  if((millis() - currentTime) >= maxTime){
    gameOver();
  }
  debounce(clickButtonD);
}

void ledInBag() {
  if ((millis() - inBagTimer) <= IN_BAG_THRESH_TIME) {
    debounce(gameOver);
  } else {
    scoreAPoint();
    inBagTimer = 0;
  }
}

void scoreAPoint() {
  analogWrite(LEDB_PIN, currIntensity);
  currIntensity += fadeAmount;
  if (currIntensity >= 255) {
    fadeAmount *= -1;
  }
  if (currIntensity == 0) {
    if (fadeAmount < 0) {
      fadeAmount *= -1;
    }
    score++;
    maxTime = maxTime * 7 / 8;
    Serial.println(String("Another object in the bag! Count: ") + score + String(" objects"));
    state = READY_TO_PLAY;
  }
  delay(25);
}

void gameOver() {
  digitalWrite(positionWhilePlaying, LOW);
  digitalWrite(LEDR_PIN, HIGH);
  Serial.println(String("Game Over - Score: ") + score);
  delay(2000);
  digitalWrite(LEDR_PIN, LOW);
  state = INITIAL_STATE;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);
  pinMode(BUTTOND_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTONS_PIN), clickButtonS, RISING);
  randomSeed(analogRead(0));
  state = INITIAL_STATE;
  Serial.println("Welcome to Led to Bag. Press Key TS to Start");
}

void loop() {
  (*functions[state])();
}
