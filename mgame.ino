#include "pitches.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

int buttons[4] = {0, 1, 2, 3};
int leds[4]    = {10, 11, 12, 13};

boolean button[4] = {0, 0, 0, 0};

#define buzzer 6

#define levelsInGame 50

int bt_simonSaid[100];
int led_simonSaid[100];

boolean lost;
int game_play, level, stage;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i <= 3; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   Welcome To   ");
  lcd.setCursor(0,1);
  lcd.print("> Memory  Game <");
  delay(2000);
  lcd.clear();

  randomSeed(analogRead(0));
}

void loop() {
  switch(stage) {
    case 0:
      displayStartMessage();
      break;
    case 1:
      playLevelIntroduction();
      break;
    case 2:
      playSequence();
      break;
    case 3:
      collectPlayerInputs();
      break;
    case 4:
      verifySequence();
      break;
    case 5:
      endGameWithLoss();
      break;
    case 6:
      handleLevelCompletion();
      break;
  }
}

void displayStartMessage() {
  lcd.setCursor(0, 0); lcd.print("Press 1st Button");
  lcd.setCursor(0, 1); lcd.print(" to Start Game ");
  button[0] = digitalRead(buttons[0]);
  while(button[0] == HIGH) {
    button[0] = digitalRead(buttons[0]);
  }
  level = 1, stage = 1, game_play = 1;
  lcd.clear();
}

void playLevelIntroduction() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Level: ");
  lcd.print(level);
  lcd.setCursor(0, 1);
  lcd.print(" -- Memorize -- ");
  delay(1500);
  stage = 2;
}

void playSequence() {
  for(int i = 1; i <= level; i++) {
    led_simonSaid[i] = random(10, 14);
    digitalWrite(led_simonSaid[i], HIGH);
    playBuzzer(led_simonSaid[i] - 7);
    delay(400);
    digitalWrite(led_simonSaid[i], LOW);
  }
  delay(500);
  stage = 3;
}

void collectPlayerInputs() {
  lcd.setCursor(0, 1);
  lcd.print("   -- Play --   ");
  for(int i = 0; i <= 3; i++) {
    button[i] = digitalRead(buttons[i]);
    if(button[i] == LOW) {
      bt_simonSaid[game_play] = leds[i];
      digitalWrite(leds[i], HIGH);
      playBuzzer(i + 1);
      while(button[i] == LOW) {
        button[i] = digitalRead(buttons[i]);
      }
      delay(50);
      digitalWrite(leds[i], LOW);
      game_play++;
      if(game_play - 1 == level) {
        game_play = 1;
        stage = 4;
      }
    }
  }
  delay(10);
}

void verifySequence() {
  lcd.setCursor(0, 1);
  lcd.print("  Verification  ");
  delay(1000);
  for(int i = 1; i <= level; i++) {
    if(led_simonSaid[i] != bt_simonSaid[i]) {
      lost = 1;
      break;
    }
  }
  stage = lost ? 5 : 6;
}

void endGameWithLoss() {
  lcd.setCursor(0, 1);
  lcd.print(" !! You Lost !! ");
  playLossTone();
  for(int i = 0; i <= 3; i++) {
    digitalWrite(leds[i], HIGH);
  }
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("!! GAME  OVER !!");
  noTone(buzzer);
  delay(1000);
  for(int i = 0; i <= 3; i++) {
    digitalWrite(leds[i], LOW);
  }
  level = 1, stage = 0, lost = 0;
}

void handleLevelCompletion() {
  lcd.setCursor(0, 1);
  lcd.print(" ** You  Win ** ");
  playWinTone();
  delay(1000);
  if(level == levelsInGame) {
    lcd.setCursor(0, 0);
    lcd.print("Congratulation");
    lcd.setCursor(0, 1);
    lcd.print(" Level Complete");
    delay(1000);
    lcd.clear();
    level = 1;
  } else {
    if(level < levelsInGame) level++;
  }
  stage = 1;
}

void playBuzzer(int x) {
  tone(buzzer, 650 + (x * 100));
  delay(300);
  noTone(buzzer);
}

void playLossTone() {
  tone(buzzer, NOTE_DS5);
  delay(300);

  tone(buzzer, NOTE_D5);
  delay(300);

  tone(buzzer, NOTE_CS5);
  delay(300);
}

void playWinTone(){
  tone(buzzer, NOTE_E4);
  delay(150);

  tone(buzzer, NOTE_G4);
  delay(150);

  tone(buzzer, NOTE_E5);
  delay(150);

  tone(buzzer, NOTE_C5);
  delay(150);

  tone(buzzer, NOTE_D5);
  delay(150);

  tone(buzzer, NOTE_G5);
  delay(150);
  noTone(buzzer);
}
