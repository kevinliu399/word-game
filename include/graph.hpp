#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

class Node
{
private:
    std::string m_value; // store the word
public:
    std::string getValue() const
    {
        return m_value;
    }

    void setValue(const std::string &newValue)
    {
        m_value = newValue;
    }

    Node(const std::string &init_val)
    {
        m_value = init_val;
    };
};

class Edge
{
private:
    double m_weight;
    Node *m_source;
    Node *m_destination;

public:
    double getWeight() const
    {
        return m_weight;
    }

    void setWeight(double new_weight)
    {
        m_weight = new_weight;
    }

    Node *getSource() const
    {
        return m_source;
    }

    Node *getDestination() const
    {
        return m_destination;
    }

    Edge(Node *source, Node *destination, double weight)
    {
        m_source = source;
        m_destination = destination;
        m_weight = weight;
    }
};

// Undirected weighted graph
class ScoreGraph
{
private:
    std::unordered_map<std::string, Node *> m_nodes;
    std::vector<Edge *> m_edges;

public:
    void addNode(const std::string &value)
    {
        if (m_nodes.find(value) == m_nodes.end())
        {
            m_nodes[value] = new Node(value);
        }
    }

    void addEdge(const std::string &source, const std::string &destination, double weight)
    {
        Node *sourceNode = m_nodes[source];
        Node *destNode = m_nodes[destination];
        m_edges.push_back(new Edge(sourceNode, destNode, weight));
    }

    Node *getNode(const std::string &value) const
    {
        auto it = m_nodes.find(value);
        if (it != m_nodes.end())
        {
            return it->second;
        }
        return nullptr;
    }

    const std::vector<Edge *> &getEdges() const
    {
        return m_edges;
    }

    // find a more optimized way to get score perhaps
    int getScore(Node *inputNode, Node *goalNode)
    {
        if (inputNode == nullptr || goalNode == nullptr)
        {
            throw std::invalid_argument("Input and goal words must not be null");
        }

        for (const auto &edge : m_edges)
        {
            if ((edge->getSource() == inputNode && edge->getDestination() == goalNode) ||
                (edge->getSource() == goalNode && edge->getDestination() == inputNode))
            {
                return static_cast<int>(edge->getWeight() * 1000);
            }
        }

        return 0;
    }
};
