#include "game.hpp"
#include "constants.hpp"



// Main game class that contains the input box and timer
// Controls the game loop and the game over state
Game::Game() : highestScore(0), gameOver(false) {}

void Game::Update()
{
    if (!gameOver)
    {
        inputBox.Update();
        timer.Update();

        if (timer.isTimeUp())
        {
            gameOver = true;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            inputBox = InputBox();
            timer = Timer();
            gameOver = false;
        }
    }
}

void Game::Draw() const
{
    if (!gameOver)
    {
        inputBox.Draw();
        timer.Draw();
    }
    else
    {
        int gameScore = inputBox.getScore();
        if (gameScore > highestScore)
        {
            const_cast<Game *>(this)->setHighestScore(gameScore);
        }

        DrawText(TextFormat("Highest Score: %i", highestScore), 10, 10, 20, BLACK);
        DrawText(TextFormat("Your Score: %i", gameScore), 10, 30, 20, BLACK);
        DrawText("Game Over!", SCREEN_WIDTH / 2 - MeasureText("Game Over!", 40) / 2, SCREEN_HEIGHT / 2 - 20, 40, RED);
        DrawText("Press ENTER to play again", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to play again", 20) / 2, SCREEN_HEIGHT / 2 + 30, 20, BLACK);
    }
}