#include "input_box.hpp"
#include "constants.hpp"
#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>

ScoreGraph CreateGraph() {
    ScoreGraph scoring_system;

    try {
        std::ifstream file("src/data/word_bank.csv");
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open word_bank.csv");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string src_word, dest_word, weight_str;

            if (std::getline(iss, src_word, ',') &&
                std::getline(iss, dest_word, ',') &&
                std::getline(iss, weight_str, ',')) {
                
                double weight = std::stod(weight_str);

                scoring_system.addNode(src_word);
                scoring_system.addNode(dest_word);
                scoring_system.addEdge(src_word, dest_word, weight);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating graph: " << e.what() << std::endl;
    }

    return scoring_system;
}

InputBox::InputBox()
    : inputWord(""), textBox{SCREEN_WIDTH / 2.0f - 160, 180, 250, 50},
      framesCounter(0), score(0), invalidWordEntered(false), lastInvalidWord("") {
    scoring_system = CreateGraph();
    goalWord = GetRandomWord();
    goalNode = scoring_system.getNode(goalWord);
}

void InputBox::Update() {
    HandlePoints();
    HandleTextInput();
    framesCounter++;
}

void InputBox::Draw() const {
    DrawText(TextFormat("Goal word: %s", goalWord.c_str()), 240, 140, 20, DARKGRAY);

    DrawRectangleRec(textBox, GRAY);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, GRAY);
    DrawText(inputWord.c_str(), textBox.x + 5, textBox.y + 8, 20, BLACK);

    if (std::find(all_words.begin(), all_words.end(), inputWord) != all_words.end()) {
        DrawText(TextFormat("%s already entered", inputWord.c_str()), 230, 300, 20, GRAY);
    } else if (invalidWordEntered) {
        DrawText(TextFormat("%s is not a valid word", lastInvalidWord.c_str()), 230, 300, 20, GRAY);
    }

    if (inputWord.length() < MAX_INPUT_CHARS) {
        if ((framesCounter / 20) % 2 == 0) {
            DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(inputWord.c_str(), 20),
                     static_cast<int>(textBox.y) + 12, 20, BLACK);
        }
    } else {
        DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
    }

    DrawText(TextFormat("Score: %i", score), SCREEN_WIDTH - 225, SCREEN_HEIGHT - 50, 20, BLACK);
}

void InputBox::HandleTextInput() {
    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 'a' && key <= 'z') && inputWord.length() < MAX_INPUT_CHARS) {
            inputWord += static_cast<char>(key);
            invalidWordEntered = false;
        }
        key = GetCharPressed();
    }

    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyDown(KEY_BACKSPACE)) && !inputWord.empty()) {
        inputWord.pop_back();
        invalidWordEntered = false;
    }
}

void InputBox::HandlePoints() {
    if (IsKeyPressed(KEY_ENTER) && !inputWord.empty()) {
        if (std::find(all_words.begin(), all_words.end(), inputWord) != all_words.end()) {
            return;
        }

        Node* inputNode = scoring_system.getNode(inputWord);
        if (inputNode == nullptr) {
            invalidWordEntered = true;
            lastInvalidWord = inputWord;
            return;
        }

        invalidWordEntered = false;
        lastInvalidWord = "";
        all_words.push_back(inputWord);

        int points = scoring_system.getScore(inputNode, goalNode);
        score += points;

        inputWord = "";
    }
}

std::string InputBox::GetRandomWord() {
    std::vector<std::string> words;
    std::ifstream file("./src/data/all_word.txt");
    std::string word;

    while (std::getline(file, word)) {
        words.push_back(word);
    }

    if (words.empty()) {
        return "No words found";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, words.size() - 1);
    return words[dist(gen)];
}