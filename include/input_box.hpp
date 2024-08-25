#pragma once
#include "raylib.h"
#include "graph.hpp"
#include <string>
#include <vector>

class InputBox
{
public:
    InputBox();
    void Update();
    void Draw() const;
    std::string getGoalWord() const { return goalWord; }
    std::string getInputWord() const { return inputWord; }
    int getScore() const { return score; }

private:
    std::string inputWord;
    std::string goalWord;
    Rectangle textBox;
    int framesCounter;
    int score;
    std::vector<std::string> all_words;
    ScoreGraph scoring_system;
    Node *goalNode;
    
    bool invalidWordEntered;
    std::string lastInvalidWord;

    void HandleTextInput();
    void HandlePoints();
    std::string GetRandomWord();
};