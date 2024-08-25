#pragma once
#include "input_box.hpp"
#include "timer.hpp"

class Game
{
public:
    Game();
    void Update();
    void Draw() const;
    bool isGameOver() const { return gameOver; }
    int getHighestScore() const { return highestScore; }
    void setHighestScore(int newScore) { highestScore = newScore; }

private:
    InputBox inputBox;
    Timer timer;
    int highestScore;
    bool gameOver;
};