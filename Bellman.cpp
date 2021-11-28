#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <queue>

const int64_t kMaxWeight = 30000;

using VertexType = size_t;

class Graph {
public:
    struct Edge {
        VertexType from;
        VertexType to;
        int64_t weight;
    };

private:
    size_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(size_t vertex_num);
    void InsertEdge(VertexType from_vertex, VertexType to_vertex, int64_t weight);
    std::vector<Edge>& Neighbours(VertexType vertex);
    size_t Size();
};

std::vector<int64_t> FordBellman(Graph& graph, VertexType start_vertex);

void Relax(Graph::Edge edge, std::vector<int64_t>& distance);

void PrintAnswer(std::vector<int64_t>& distances_from_start);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t vertex_num = 0;
    size_t edge_num = 0;
    VertexType from_vertex = 0;
    VertexType to_vertex = 0;
    int64_t weight = 0;

    std::cin >> vertex_num >> edge_num;
    Graph graph(vertex_num);

    for (size_t i = 0; i < edge_num; ++i) {
        std::cin >> from_vertex >> to_vertex >> weight;
        graph.InsertEdge(from_vertex, to_vertex, weight);
    }

    std::vector<int64_t> distances_from_start = FordBellman(graph, 1);

    PrintAnswer(distances_from_start);

    return 0;
}

Graph::Graph(size_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1);
}

void Graph::InsertEdge(VertexType from_vertex, VertexType to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
}

std::vector<Graph::Edge>& Graph::Neighbours(VertexType vertex) {
    return graph_[vertex];
}

size_t Graph::Size() {
    return vertex_number_;
}

std::vector<int64_t> FordBellman(Graph& graph, VertexType start_vertex) {
    std::vector<int64_t> distance(graph.Size() + 1, kMaxWeight);
    distance[start_vertex] = 0;

    for (size_t i = 0; i < graph.Size() - 1; ++i) {
        for (VertexType vertex = 1; vertex < graph.Size() + 1; ++vertex) {
            for (auto& edge : graph.Neighbours(vertex)) {
                Relax(edge, distance);
            }
        }
    }

    return distance;
}

void Relax(Graph::Edge edge, std::vector<int64_t>& distance) {
    if (distance[edge.from] < kMaxWeight) {
        if (distance[edge.to] > distance[edge.from] + edge.weight) {
            distance[edge.to] = distance[edge.from] + edge.weight;
        }
    }
}

void PrintAnswer(std::vector<int64_t>& distances_from_start) {
    for (VertexType vertex = 1; vertex < distances_from_start.size(); ++vertex) {
        std::cout << distances_from_start[vertex] << " ";
    }
    std::cout << "\n";
}