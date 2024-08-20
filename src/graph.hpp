#pragma once
#include <list>

class Graph {
private:
    int m_v; // # of vertices in the graph
    std::list<int>* adj; // array of adj list
};
