#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <queue>

const int64_t kMax = 2009000999;

using Vertex = int64_t;

struct Neighbour {
    Vertex from;
    Vertex to;
    int64_t weight;
};

class IGraph {
public:
    using Edge = Neighbour;

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) = 0;
    virtual const std::vector<Edge>& Neighbours(Vertex vertex) = 0;
    virtual size_t Size() = 0;
};

class GraphList : public IGraph {
private:
    size_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;

public:
    using Edge = Neighbour;

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int64_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) override;
    const std::vector<Edge>& Neighbours(Vertex vertex) override;
    size_t Size() override;
};

void Relax(IGraph::Edge edge, std::vector<int64_t>& distance, std::vector<int64_t>& new_distance);

int64_t FindShortestPathFordBellman(IGraph& graph, Vertex start, Vertex finish, int64_t iterations_number);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t towns_number = 0;
    int64_t fligths_number = 0;
    int64_t nights_number = 0;
    Vertex start = 0;
    Vertex finish = 0;
    Vertex from = 0;
    Vertex to = 0;
    int64_t cost = 0;

    std::cin >> towns_number >> fligths_number >> nights_number >> start >> finish;

    GraphList graph(towns_number);

    for (int64_t i = 0; i < fligths_number; ++i) {
        std::cin >> from >> to >> cost;
        graph.InsertEdge(--from, --to, cost);
    }

    int64_t min_distance = FindShortestPathFordBellman(graph, --start, --finish, nights_number);

    std::cout << min_distance << "\n";

    return 0;
}

GraphList::GraphList(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
}

const std::vector<GraphList::Edge>& GraphList::Neighbours(Vertex vertex) {
    return graph_[vertex];
}

size_t GraphList::Size() {
    return vertex_number_;
}

int64_t FindShortestPathFordBellman(IGraph& graph, Vertex start, Vertex finish, int64_t iterations_number) {
    std::vector<int64_t> distance(graph.Size() + 1, kMax);
    std::vector<int64_t> new_distance(graph.Size() + 1, kMax);
    int64_t min_distance = kMax;
    distance[start] = 0;
    for (int64_t i = 0; i < iterations_number; ++i) {
        new_distance.assign(graph.Size() + 1, kMax);
        for (auto vertex = 0; vertex < static_cast<int64_t>(graph.Size()); ++vertex) {
            for (const IGraph::Edge& edge : graph.Neighbours(vertex)) {
                Relax(edge, distance, new_distance);
            }
        }
        std::swap(distance, new_distance);
        min_distance = std::min(min_distance, distance[finish]);
    }

    if (min_distance == kMax) {
        return -1;
    }
    return min_distance;
}

void Relax(IGraph::Edge edge, std::vector<int64_t>& distance, std::vector<int64_t>& new_distance) {
    if (new_distance[edge.to] > distance[edge.from] + edge.weight) {
        new_distance[edge.to] = distance[edge.from] + edge.weight;
    }
}