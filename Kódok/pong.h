#ifndef PONG_H
#define PONG_H
#include <TFT_eSPI.h>

#define EXIT_BUTTON 25
#define JOY_X 34

void runPong(TFT_eSPI &tft) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("PONG", tft.width() / 2, 30, 4);

  int ballX = tft.width() / 2;
  int ballY = tft.height() / 6;
  int ballDX = 1, ballDY = 1;

  int paddleWidth = 60;
  int paddleHeight = 8;
  int paddleY = tft.height() - 30;
  int paddleX = (tft.width() - paddleWidth) / 2;

  int score = 0;

  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  // joystick setup
  const int deadzone = 800;
  const int center = 2048;

  bool exitRequested = false;

  while (!exitRequested) {

    // draw ball + paddle
    tft.fillCircle(ballX, ballY, 4, TFT_WHITE);
    tft.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, TFT_BLUE);

    delay(10);

    // clear previous frame
    tft.fillCircle(ballX, ballY, 4, TFT_BLACK);
    tft.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, TFT_BLACK);

    // move ball
    ballX += ballDX;
    ballY += ballDY;

    // wall bounce
    if (ballX <= 0 || ballX >= tft.width())  ballDX = -ballDX;
    if (ballY <= 0) ballDY = -ballDY;

    // joystick speed ramp
    int raw = analogRead(JOY_X);
    int diff = raw - center;

    if (abs(diff) > deadzone) {
      int speed = map(abs(diff), deadzone, 2000, 1, 3);
      if (speed < 1) speed = 1;
      if (speed > 3) speed = 3;

      if (diff < 0) paddleX -= speed;
      else          paddleX += speed;
    }

    // keep paddle in bounds
    if (paddleX < 0) paddleX = 0;
    if (paddleX + paddleWidth > tft.width())
      paddleX = tft.width() - paddleWidth;

    // paddle collision
    if (ballY >= paddleY - 4 && ballY <= paddleY &&
        ballX >= paddleX && ballX <= paddleX + paddleWidth) {
      ballDY = -ballDY;
      ballY = paddleY - 5;
      score++;
    }

    // game over
    if (ballY > tft.height()) {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawCentreString("GAME OVER", tft.width() / 2, tft.height() / 2 - 20, 4);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawCentreString(("Score: " + String(score)).c_str(), tft.width() / 2, tft.height() / 2 + 20, 2);

      delay(3000);
      return;
    }

    // exit
    if (digitalRead(EXIT_BUTTON) == LOW) {
      tft.fillScreen(TFT_BLACK);
      exitRequested = true;
    }
  }
}
#endif