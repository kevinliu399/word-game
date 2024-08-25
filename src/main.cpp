#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include "graph.hpp"

/**********************************
 * Constants and global variables *
 **********************************/

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int TOTAL_TIME = 40;
const int MAX_INPUT_CHARS = 20;

int highestScore = 0;

// Create a graph from the data in the word_bank.csv file
ScoreGraph CreateGraph()
{
    ScoreGraph scoring_system;

    std::ifstream file("src/data/word_bank.csv");
    std::string line;

    while (std::getline(file, line))
    {

        // data is in format : src_word,dest_word,weight
        std::string src_word = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string dest_word = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string str = line;
        double weight = stod(line);

        scoring_system.addNode(src_word);
        scoring_system.addNode(dest_word);
        scoring_system.addEdge(src_word, dest_word, weight);
    }

    return scoring_system;
}

class InputBox
{
private:
    std::string inputWord;
    Rectangle textBox;
    int framesCounter;
    std::vector<std::string> all_words;
    std::string goalWord;
    int score;
    ScoreGraph scoring_system;
    Node *goalNode;

    bool invalidWordEntered;
    std::string lastInvalidWord;

    void HandleTextInput()
    {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 'a' && key <= 'z') && inputWord.length() < MAX_INPUT_CHARS)
            {
                inputWord += static_cast<char>(key);
                invalidWordEntered = false;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !inputWord.empty())
        {
            inputWord.pop_back();
            invalidWordEntered = false;
        }
    }

    void HandlePoints()
    {
        if (IsKeyPressed(KEY_ENTER) && !inputWord.empty())
        {
            if (std::find(all_words.begin(), all_words.end(), inputWord) != all_words.end())
            {
                // word already exists
                return;
            }

            Node *inputNode = scoring_system.getNode(inputWord);
            if (inputNode == nullptr)
            {
                // invalid word
                invalidWordEntered = true;
                lastInvalidWord = inputWord;
                return;
            }

            // valid word
            invalidWordEntered = false;
            lastInvalidWord = "";
            all_words.push_back(inputWord);

            int points = scoring_system.getScore(inputNode, goalNode);
            score += points;

            inputWord = ""; // clear the input box
        }
    }

    // Get a random word from all_word.txt
    std::string GetRandomWord()
    {
        std::ifstream file("./src/data/all_word.txt");
        std::vector<std::string> words;
        std::string word;

        while (std::getline(file, word))
        {
            words.push_back(word);
        }

        if (words.empty())
        {
            return "No words found";
        }

        // Seed the random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, words.size() - 1);
        std::string random_word = words[dist(gen)];

        return random_word;
    }

public:
    InputBox()
    {
        inputWord = "";
        textBox = {SCREEN_WIDTH / 2.0f - 160, 180, 250, 50};
        framesCounter = 0;
        score = 0;
        scoring_system = CreateGraph();
        goalWord = GetRandomWord();
        goalNode = scoring_system.getNode(goalWord);

        invalidWordEntered = false;
        lastInvalidWord = "";
    }

    void Update()
    {
        HandlePoints();
        HandleTextInput();
        framesCounter++;
    }

    void Draw() const
    {
        DrawText(TextFormat("Goal word: %s", goalWord.c_str()), 240, 140, 20, DARKGRAY);

        DrawRectangleRec(textBox, WHITE);
        DrawRectangleLines(textBox.x, textBox.y, textBox.width + 25, textBox.height, GRAY);
        DrawText(inputWord.c_str(), textBox.x + 5, textBox.y + 8, 20, MAROON);

        // message for when the word was already entered
        if (std::find(all_words.begin(), all_words.end(), inputWord) != all_words.end())
        {
            DrawText(TextFormat("%s already entered", inputWord.c_str()), 230, 300, 20, GRAY);
        }

        // message for invalid or non-existent word in the dict
        else if (invalidWordEntered)
        {
            DrawText(TextFormat("%s is not a valid word", lastInvalidWord.c_str()), 230, 300, 20, GRAY);
        }

        if (inputWord.length() < MAX_INPUT_CHARS)
        {
            if ((framesCounter / 20) % 2 == 0)
            {
                DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(inputWord.c_str(), 20),
                         static_cast<int>(textBox.y) + 12, 20, MAROON);
            }
        }
        else
        {
            DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }

        DrawText(TextFormat("Score: %i", score), SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 20, MAROON);
    }

    std::string getGoalWord() const
    {
        return goalWord;
    }

    std::string getInputWord() const
    {
        return inputWord;
    }

    int getScore() const
    {
        return score;
    }

    void setScore(int newScore)
    {
        score = newScore;
    }
};

class Timer
{
public:
    Timer() : m_time(TOTAL_TIME), m_frameCount(0) {}

    void Update()
    {
        m_frameCount++;
        if (m_frameCount >= 60)
        {
            // Update every 60 frames (1 second at 60 FPS)
            m_frameCount = 0;
            if (m_time > 0)
            {
                m_time--;
            }
        }
    }

    int getTime() const
    {
        return m_time;
    }

    bool isTimeUp() const
    {
        return m_time <= 0;
    }

    void Draw() const
    {
        DrawText(TextFormat("Time: %02i", m_time), 600, 10, 20, MAROON);
    }

private:
    int m_time;
    int m_frameCount;
};

class Game
{
public:
    InputBox inputBox = InputBox();
    Timer timer = Timer();

    void Update()
    {
        inputBox.Update();
        timer.Update();
    }

    void Draw() const
    {
        inputBox.Draw();
        timer.Draw();
    }

    bool isGameOver() const
    {
        return timer.isTimeUp();
    }
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Word Association Game by Kevin");
    SetTargetFPS(60);
    bool gameOver = false;
    Game game = Game();

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            game.Update();

            if (game.timer.isTimeUp())
            {
                gameOver = true;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver)
        {
            game.Draw();
        }
        else
        {
            DrawText("Game Over!", SCREEN_WIDTH / 2 - MeasureText("Game Over!", 40) / 2, SCREEN_HEIGHT / 2 - 20, 40, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}