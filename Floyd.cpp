#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

const int64_t kMax = 10000001;

using VertexType = size_t;

class IGraph {
public:
    virtual void InsertEdge(VertexType from_vertex, VertexType to_vertex, int64_t weight) = 0;
    virtual int64_t GetEdgeWeight(VertexType from, VertexType to) = 0;
    virtual size_t Size() = 0;
};

class GraphMatrix : public IGraph {
private:
    size_t vertex_number_;
    std::vector<std::vector<int64_t>> graph_;

public:
    GraphMatrix() = default;
    ~GraphMatrix() = default;
    explicit GraphMatrix(size_t vertex_num);
    void InsertEdge(VertexType from_vertex, VertexType to_vertex, int64_t weight) override;
    int64_t GetEdgeWeight(VertexType from, VertexType to) override;
    size_t Size() override;
};

std::vector<std::vector<int64_t>> FloydWarshall(IGraph& graph);

void FillInitialDistances(IGraph& graph, std::vector<std::vector<int64_t>>& distance);

void PrintAnswer(std::vector<std::vector<int64_t>> distance);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t vertex_num = 0;
    int64_t weight = 0;
    std::cin >> vertex_num;

    GraphMatrix graph(vertex_num);
    for (VertexType from_vertex = 0; from_vertex < vertex_num; ++from_vertex) {
        for (VertexType to_vertex = 0; to_vertex < vertex_num; ++to_vertex) {
            std::cin >> weight;
            graph.InsertEdge(from_vertex, to_vertex, weight);
        }
    }

    std::vector<std::vector<int64_t>> distances = FloydWarshall(graph);

    PrintAnswer(distances);
}

GraphMatrix::GraphMatrix(size_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_, std::vector<int64_t>(vertex_number_, 0));
}

void GraphMatrix::InsertEdge(VertexType from_vertex, VertexType to_vertex, int64_t weight) {
    graph_[from_vertex][to_vertex] = weight;
}

int64_t GraphMatrix::GetEdgeWeight(VertexType from, VertexType to) {
    return graph_[from][to];
}

size_t GraphMatrix::Size() {
    return vertex_number_;
}

std::vector<std::vector<int64_t>> FloydWarshall(IGraph& graph) {
    std::vector<std::vector<int64_t>> distance(graph.Size(), std::vector<int64_t>(graph.Size(), 0));
    FillInitialDistances(graph, distance);

    for (VertexType temp = 0; temp < graph.Size(); ++temp) {
        for (VertexType from = 0; from < graph.Size(); ++from) {
            for (VertexType to = 0; to < graph.Size(); ++to) {
                if (distance[from][temp] + distance[temp][to] < distance[from][to]) {
                    distance[from][to] = distance[from][temp] + distance[temp][to];
                }
            }
        }
    }

    return distance;
}

void FillInitialDistances(IGraph& graph, std::vector<std::vector<int64_t>>& distance) {
    for (size_t from = 0; from < distance.size(); ++from) {
        for (size_t to = 0; to < distance.size(); ++to) {
            distance[from][to] = graph.GetEdgeWeight(from, to);
        }
    }
}

void PrintAnswer(std::vector<std::vector<int64_t>> distance) {
    for (VertexType from = 0; from < distance.size(); ++from) {
        for (VertexType to = 0; to < distance.size(); ++to) {
            std::cout << distance[from][to] << " ";
        }
        std::cout << "\n";
    }
}