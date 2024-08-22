#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include "graph.hpp"

int main() {

    ScoreGraph scoring_system = ScoreGraph();
    
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

        if (src_word == "winter" && dest_word == "autumn") {
            std::cout << weight << std::endl;
        }

        // add the nodes and edges to the graph
        scoring_system.addNode(src_word);
        scoring_system.addNode(dest_word);
        scoring_system.addEdge(src_word, dest_word, weight);
    }

    return 0;
}