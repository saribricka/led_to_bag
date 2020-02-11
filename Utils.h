#ifndef Environment_H
#define Environment_H

#define LED1_PIN 7
#define LED2_PIN 8
#define LED3_PIN 9
#define LEDB_PIN 10
#define LEDR_PIN 11
#define BUTTONS_PIN 2
#define BUTTOND_PIN 3
#define POT_PIN A0

#define NUM_LEVELS 8
#define DEBOUNCE_DELAY 50
#define IN_BAG_THRESH_TIME 500

void initialState(void);
void readyToPlay(void);
void ledToBag(void);
void ledInBag(void);

enum State {INITIAL_STATE, READY_TO_PLAY, PLAYING, IN_BAG} state;
long level[NUM_LEVELS] = {10000, 7500, 5000, 2500, 1000, 500, 250, 100};
void (*functions[4])(void) = {initialState, readyToPlay, ledToBag, ledInBag};

static long lastDebounceTime = 0;
static int lastButtonState = LOW;
static int buttonState;
static int positionWhilePlaying;
static int currIntensity = 0;
static long inBagTimer = 0;

void debounce(void (*function)()) {
  int reading = digitalRead(BUTTOND_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState) {
        (*function)();
      }
    }
  }
  lastButtonState = reading;
}

void clickButtonS() {
  for (int i = LED1_PIN; i <= LED3_PIN; i++) {
    digitalWrite(i, LOW);
  }
  Serial.println("Go!");
  state = READY_TO_PLAY;
}

void clickButtonD() {
  digitalWrite(positionWhilePlaying, LOW);
  positionWhilePlaying++;
  if (positionWhilePlaying == LEDB_PIN) {
    currIntensity = 0;
    inBagTimer = millis();
    state = IN_BAG;
  } else {
    digitalWrite(positionWhilePlaying, HIGH);
  }
}
#endif
