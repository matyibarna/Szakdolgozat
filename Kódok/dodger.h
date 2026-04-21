#ifndef DODGER_H
#define DODGER_H
#include <TFT_eSPI.h>

#define EXIT_BUTTON 25
#define JOY_X 34

void runDodger(TFT_eSPI &tft) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString("DODGER", tft.width() / 2, 10, 4);

  // game area
  const int gameTop = 50;
  const int gameBottom = tft.height();
  const int gameLeft = 0;
  const int gameRight = tft.width();

  // player
  int playerW = 20;
  int playerH = 10;
  int playerX = tft.width() / 2 - playerW / 2;
  int playerY = gameBottom - playerH - 5;

  // falling block
  int blockW = 20;
  int blockH = 20;
  int blockX = random(0, (tft.width() - blockW));
  int blockY = gameTop;

  int blockSpeed = 3;
  int score = 0;

  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  // joystick setup
  const int center = 2048;
  const int deadzone = 700;

  while (true) {
    // exit
    if (digitalRead(EXIT_BUTTON) == LOW) {
      tft.fillScreen(TFT_BLACK);
      return;
    }

    // erase old positions
    tft.fillRect(playerX, playerY, playerW, playerH, TFT_BLACK);
    tft.fillRect(blockX, blockY, blockW, blockH, TFT_BLACK);

    // joystick read
    int rawX = analogRead(JOY_X);
    int dx = rawX - center;

    if (dx < -deadzone) {
      playerX -= 5;
    } else if (dx > deadzone) {
      playerX += 5;
    }

    // keep player inside screen
    if (playerX < gameLeft) playerX = gameLeft;
    if (playerX + playerW > gameRight) playerX = gameRight - playerW;

    // move falling block
    blockY += blockSpeed;

    // if block reaches bottom, reset it and increase score
    if (blockY > gameBottom) {
      blockY = gameTop;
      blockX = random(0, (tft.width() - blockW));
      score++;

      // optional speed increase every 5 points
      if (score % 5 == 0 && blockSpeed < 8) {
        blockSpeed++;
      }
    }

    // collision detection
    bool hit =
      playerX < blockX + blockW &&
      playerX + playerW > blockX &&
      playerY < blockY + blockH &&
      playerY + playerH > blockY;

    if (hit) {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawCentreString("GAME OVER", tft.width() / 2, tft.height() / 2 - 20, 4);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawCentreString(("Score: " + String(score)).c_str(), tft.width() / 2, tft.height() / 2 + 20, 2);

      delay(3000);
      return;
    }

    // draw player and block
    tft.fillRect(playerX, playerY, playerW, playerH, TFT_GREEN);
    tft.fillRect(blockX, blockY, blockW, blockH, TFT_RED);

    delay(35);
  }
}
#endif