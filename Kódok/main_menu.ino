#include <TFT_eSPI.h>
#include "snake.h"
#include "pong.h"
#include "dodger.h"

TFT_eSPI tft = TFT_eSPI();

// button-pins
#define BTN_UP     14
#define BTN_DOWN   27
#define BTN_OK     26
#define BTN_EXIT   25

// menu variables
int selected = 0;
const int totalItems = 3;
bool inGame = false;
bool exitRequested = false;

// debounce
unsigned long lastPressTime = 0;
const int debounceDelay = 65;

// declare func
void drawMenu();
void drawMenuItem(const char *label, int index, bool selected);
void handleButtons();
bool buttonPressed(int pin);
void debugButtons();

void setup() {
  Serial.begin(115200);

  // display initialisation
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // button setup
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_EXIT, INPUT_PULLUP);

  // button test
  drawMenu();

  Serial.println("=== Main Menu Started ===");
  Serial.println("Button test active...");
}

void loop() {
  if (!inGame) {
    debugButtons();
    handleButtons();
  }
  else {
    if (buttonPressed(BTN_EXIT)) {
      exitRequested = true;
      Serial.println("Exit button pressed -> return to menu");
    }
  }
}

// main menu
void drawMenu() {
  tft.fillScreen(TFT_BLACK);
  drawMenuItem("Snake", 0, selected == 0);
  drawMenuItem("Pong", 1, selected == 1);
  drawMenuItem("Dodger", 2, selected == 2);
}

void drawMenuItem(const char *label, int index, bool isSelected) {
  int y = 40 + index * 40;
  if (isSelected) {
    tft.fillRoundRect(20, y - 5, 200, 30, 5, TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
  } else {
    tft.fillRoundRect(20, y - 5, 200, 30, 5, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.setCursor(30, y);
  tft.print(label);
}

// button handling
void handleButtons() {
  if (millis() - lastPressTime < debounceDelay) return;

  if (buttonPressed(BTN_UP)) {
    selected = (selected - 1 + totalItems) % totalItems;
    drawMenu();
    lastPressTime = millis();
  }

  if (buttonPressed(BTN_DOWN)) {
    selected = (selected + 1) % totalItems;
    drawMenu();
    lastPressTime = millis();
  }

  if (buttonPressed(BTN_OK)) {
    inGame = true;
    exitRequested = false;
    tft.fillScreen(TFT_BLACK);

    if (selected == 0) runSnake(tft);
    if (selected == 1) runPong(tft);
    if (selected == 2) runDodger(tft);

    inGame = false;
    drawMenu();
    lastPressTime = millis();
  }
}

// button press detection
bool buttonPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30); // tiny debounce
    if (digitalRead(pin) == LOW) return true;
  }
  return false;
}

// for debugging
void debugButtons() {
  Serial.print("UP:");
  Serial.print(digitalRead(BTN_UP));
  Serial.print(" DOWN:");
  Serial.print(digitalRead(BTN_DOWN));
  Serial.print(" OK:");
  Serial.print(digitalRead(BTN_OK));
  Serial.print(" EXIT:");
  Serial.println(digitalRead(BTN_EXIT));
  delay(100);
}
