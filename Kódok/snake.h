#ifndef SNAKE_H
#define SNAKE_H
#include <TFT_eSPI.h>

#define EXIT_BUTTON 25
#define JOY_X 34
#define JOY_Y 35

void runSnake(TFT_eSPI &tft) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("SNAKE", tft.width() / 2, 30, 4);

  const int cellSize = 10;
  int snakeX[100], snakeY[100];
  int snakeLength = 5;
  int dirX = 1, dirY = 0;
  int foodX = random(0, tft.width() / cellSize) * cellSize;
  int foodY = random(0, tft.height() / cellSize) * cellSize;

  // initialize snake in center
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = tft.width() / 2 - i * cellSize;
    snakeY[i] = tft.height() / 2;
  }

  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  // joystick setup
  const int center = 2048;
  const int deadzone = 700;

  while (true) {

    if (digitalRead(EXIT_BUTTON) == LOW) {
      tft.fillScreen(TFT_BLACK);
      return;
    }

    // joystick read
    int rawX = analogRead(JOY_X);
    int rawY = analogRead(JOY_Y);

    int dx = rawX - center;
    int dy = rawY - center;

    if (abs(dx) > abs(dy)) {
      // horizontal
      if (dx < -deadzone && dirX == 0) { dirX = -1; dirY = 0; }
      else if (dx >  deadzone && dirX == 0) { dirX = 1; dirY = 0; }
    } else {
      // vertical
      if (dy < -deadzone && dirY == 0) { dirX = 0; dirY = -1; }
      else if (dy >  deadzone && dirY == 0) { dirX = 0; dirY = 1; }
    }

    // move snake
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }

    snakeX[0] += dirX * cellSize;
    snakeY[0] += dirY * cellSize;

    // wall collision
    if (snakeX[0] < 0 || snakeY[0] < 0 ||
        snakeX[0] >= tft.width() || snakeY[0] >= tft.height()) {

      int score = snakeLength - 5;

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawCentreString("GAME OVER", tft.width() / 2, tft.height() / 2 - 20, 4);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawCentreString(("Score: " + String(score)).c_str(), tft.width() / 2, tft.height() / 2 + 20, 2);

      delay(3000);
      return;
    }

    // food eaten
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
      snakeLength++;
      foodX = random(0, tft.width() / cellSize) * cellSize;
      foodY = random(0, tft.height() / cellSize) * cellSize;
    }

    // draw
    tft.fillRect(foodX, foodY, cellSize, cellSize, TFT_RED);
    tft.fillRect(snakeX[0], snakeY[0], cellSize, cellSize, TFT_GREEN);
    for (int i = 1; i < snakeLength; i++) {
      tft.fillRect(snakeX[i], snakeY[i], cellSize, cellSize, TFT_DARKGREEN);
    }

    delay(80);
    tft.fillRect(snakeX[snakeLength - 1], snakeY[snakeLength - 1], cellSize, cellSize, TFT_BLACK);
  }
}
#endif