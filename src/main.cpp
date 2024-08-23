#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include "graph.hpp"

/*********************************
* Constants and global variables *
**********************************/

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int TOTAL_TIME = 20;
const int MAX_INPUT_CHARS = 30;

int highestScore = 0;

int CalculateScore(float edge_weight) {
    return static_cast<int>(edge_weight * 1000);
}

class InputBox {
private:
    std::string name;
    Rectangle textBox;
    int framesCounter;

    void HandleTextInput() {
        int key = GetKeyPressed();

        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (name.length() < MAX_INPUT_CHARS)) {
                name += static_cast<char>(key);
            }
            key = GetKeyPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !name.empty()) {
            name.pop_back();
        }
    }
public:
    InputBox() {
        name = "";
        textBox = { SCREEN_WIDTH/2.0f - 100, 180, 225, 50 };
        framesCounter = 0;
    }

    void Update() {
        HandleTextInput();
        framesCounter++;
    }

    std::string getInputWord() const {
        return name;
    }

    void Draw() const {
        DrawText("Type a word!", 240, 140, 20, DARKGRAY);

        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, GRAY);
        DrawText(name.c_str(), textBox.x + 5, textBox.y + 8, 40, MAROON);

        if (name.length() < MAX_INPUT_CHARS) {
            if ((framesCounter / 20) % 2 == 0) {
                DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(name.c_str(), 40), 
                         static_cast<int>(textBox.y) + 12, 40, MAROON);
            }
        } else {
            DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }
    }
};

class Timer {
public:
    Timer() : m_time(TOTAL_TIME), m_frameCount(0) {}

    void Update() {
        m_frameCount++;
        if (m_frameCount >= 60) {  // Update every 60 frames (1 second at 60 FPS)
            m_frameCount = 0;
            if (m_time > 0) {
                m_time--;
            }
        }
    }

    int getTime() const {
        return m_time;
    }

    bool isTimeUp() const {
        return m_time <= 0;
    }

    void Draw() const {
        DrawText(TextFormat("Time: %02i", m_time), 600, 10, 20, MAROON);
    }

private:
    int m_time; 
    int m_frameCount;
};

// Create a graph from the data in the word_bank.csv file
ScoreGraph CreateGraph() {
    ScoreGraph scoring_system;
    
    std::ifstream file("src/data/word_bank.csv");
    std::string line;

    while (std::getline(file, line)) {
        
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

class Game {
public:
    int highestScore;
    InputBox inputBox = InputBox();
    Timer timer = Timer();
    // ScoreGraph scoring_system;

    Game() {
        highestScore = 0;
        // scoring_system = CreateGraph();
    }

    void Update() {
        inputBox.Update();
        timer.Update();
    }

    void Draw() const {
        inputBox.Draw();
        timer.Draw();
    }

    bool isGameOver() const {
        return timer.isTimeUp();
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [text] example - input box");
    SetTargetFPS(60);
    bool gameOver = false;

    InputBox inputBox;
    Timer timer;
    // ScoreGraph scoring_system = CreateGraph();

    while (!WindowShouldClose()) {
        if (!gameOver) {
            inputBox.Update();
            timer.Update();
            
            if (timer.isTimeUp()) {
                gameOver = true;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (!gameOver) {
            inputBox.Draw();
            timer.Draw();
        } else {
            DrawText("Game Over!", SCREEN_WIDTH/2 - MeasureText("Game Over!", 40)/2, SCREEN_HEIGHT/2 - 20, 40, RED);
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}