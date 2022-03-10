#include "raylib.h"
#include <string>


struct Ball {
    float x, y, speedX, speedY, radius;

    void draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void increaseSpeed() {
        if (speedX < 0) {
            speedX--;
        }
        else {
            speedX++;
        }
        if (speedY < 0) {
            speedY--;
        }
        else {
            speedY++;
        }
    }
};

struct Paddle {
    float x, y, speed, width, height;

    Rectangle GetRectangle()
    {
        return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
    }

    void draw()
    {
        DrawRectangleRec(GetRectangle(), WHITE);
    }
};

int main(void)
{   const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong");
    const char* startingText = "Press space to play!";
    const char* restartText = "Press space to restart!";

    Ball ball;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.radius = 10;
    ball.speedX = 2;
    ball.speedY = 2;

    Paddle playerPaddle;
    playerPaddle.x = 25;
    playerPaddle.y = screenHeight / 2;
    playerPaddle.speed = 10;
    playerPaddle.height = 100;
    playerPaddle.width = 10;

    Paddle AIPaddle;
    AIPaddle.x = screenWidth - 25;
    AIPaddle.y = screenHeight / 2;
    AIPaddle.speed = 10;
    AIPaddle.height = 100;
    AIPaddle.width = 10;

    bool gameGoingOn = false;
    bool weHaveAWinner = false;
    int playerScore = 0;
    int AIScore = 0;
    const char* winnerText;


    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (gameGoingOn) {

            ball.x += ball.speedX;
            ball.y += ball.speedY;

            if (ball.y >= screenHeight || ball.y <= 0) {
                ball.speedY *= -1;
            }

            if (IsKeyDown(KEY_W)) {
                if (playerPaddle.y >= 0 + playerPaddle.height / 2) {
                    playerPaddle.y -= playerPaddle.speed;
                }
            }

            if (IsKeyDown(KEY_S)) {
                if (playerPaddle.y <= screenHeight - playerPaddle.height / 2) {
                    playerPaddle.y += playerPaddle.speed;
                }
            }

            if (ball.y > AIPaddle.y) {
                if (AIPaddle.y <= screenHeight - AIPaddle.height / 2 && ball.y > AIPaddle.y)
                {
                    AIPaddle.y += AIPaddle.speed;
                }
            }

            if (ball.y < AIPaddle.y) {
                if (AIPaddle.y >= 0 + AIPaddle.height / 2 && ball.y < AIPaddle.y)
                {
                    AIPaddle.y -= AIPaddle.speed;
                }
            }

            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, playerPaddle.GetRectangle())) {
                if (ball.speedX < 0) {
                    ball.speedX *= -1;
                }
                ball.increaseSpeed();
                if (ball.y > playerPaddle.y) {
                    if (ball.speedY < 0) ball.speedY *= -1;
                }
                else
                {
                    if (ball.speedY > 0) ball.speedY *= -1;
                }
            }

            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, AIPaddle.GetRectangle())) {
                if (ball.speedX > 0) {
                    ball.speedX *= -1;
                }
                ball.increaseSpeed();
                if (ball.y > AIPaddle.y) {
                    if (ball.speedY < 0) ball.speedY *= -1;
                }
                else
                {
                    if (ball.speedY > 0) ball.speedY *= -1;
                }
            }

            if (ball.x < 0) {
                AIScore++;
                ball.x = screenWidth / 2;
                ball.y = screenHeight / 2;
                ball.speedX = -2;
                ball.speedY = 2;
            }

            if (ball.x > screenWidth) {
                playerScore++;
                ball.x = screenWidth / 2;
                ball.y = screenHeight / 2;
                ball.speedX = 2;
                ball.speedY = 2;
            }

            if (playerScore == 3 || AIScore == 3) {
                weHaveAWinner = true;
                gameGoingOn = false;
                if (playerScore == 3) winnerText = "You win!";
                else winnerText = "Computer wins!";
            }

            BeginDrawing();

            ClearBackground(BLACK);

            ball.draw();
            playerPaddle.draw();
            AIPaddle.draw();

            DrawText(std::to_string(playerScore).c_str(), screenWidth/2 - 30, 0, 40, YELLOW);
            DrawText(std::to_string(AIScore).c_str(), screenWidth/2, 0, 40, YELLOW);

            EndDrawing();

        }
        else 
        {
            int textWidth;
            if (weHaveAWinner) {
                playerScore = 0;
                AIScore = 0;

                textWidth = MeasureText(winnerText, 100);

                BeginDrawing();

                ClearBackground(BLACK);
                DrawText(winnerText, screenWidth / 2 - textWidth / 2, screenHeight / 2 - 50, 100, YELLOW);
                textWidth = MeasureText(restartText, 40);
                DrawText(restartText, screenWidth / 2 - textWidth / 2, screenHeight - 60, 40, YELLOW);

                EndDrawing();

                if (IsKeyPressed(KEY_SPACE)) {
                    gameGoingOn = true;
                }
            }
            else
            {
                BeginDrawing();

                ClearBackground(BLACK);

                textWidth = MeasureText(startingText, 60);
                DrawText(startingText, screenWidth / 2 - textWidth / 2, screenHeight / 2 - 30, 60, YELLOW);
                if (IsKeyPressed(KEY_SPACE)) gameGoingOn = true;

                EndDrawing();
            }

        } 
    }

    
    CloseWindow();        
    return 0;
}
