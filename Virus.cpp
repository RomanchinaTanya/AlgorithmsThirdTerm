#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>

const int64_t kMax = LLONG_MAX;

template <class TElem>
using MinHeap = std::priority_queue<TElem, std::vector<TElem>, std::greater<TElem>>;

class Graph {
private:
    struct Edge {
        int64_t from;
        int64_t to;
        int64_t weight;
    };

    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<int64_t> distance_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    void Dijkstra(int64_t start);
    void PrintAnswer(std::vector<int64_t>& virused, int64_t healthy);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t countries_number = 0;
    int64_t lines_number = 0;
    int64_t virused_number = 0;
    int64_t from = 0;
    int64_t to = 0;
    int64_t cost = 0;
    std::cin >> countries_number >> lines_number >> virused_number;

    std::vector<int64_t> virused(virused_number);
    for (int64_t i = 0; i < virused_number; ++i) {
        std::cin >> virused[i];
        --virused[i];
    }
    Graph graph(countries_number);
    for (int64_t i = 0; i < lines_number; ++i) {
        std::cin >> from >> to >> cost;
        graph.Insert(--from, --to, cost);
    }
    std::cin >> from >> to;
    graph.Dijkstra(--to);
    graph.PrintAnswer(virused, --from);

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
    distance_.resize(vertex_number_ + 1ll);
    distance_.assign(vertex_number_ + 1ll, kMax);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
    graph_[to_vertex].push_back({to_vertex, from_vertex, weight});
}

void Graph::Dijkstra(int64_t start) {
    MinHeap<std::pair<int64_t, int64_t>> queue;
    distance_[start] = 0ll;
    queue.push({0ll, start});
    while (!queue.empty()) {
        auto[current_dist, vertex] = queue.top();
        queue.pop();
        if (current_dist > distance_[vertex]) {
            continue;
        }
        for (auto edge : graph_[vertex]) {
            if (distance_[edge.to] > distance_[vertex] + edge.weight) {
                distance_[edge.to] = distance_[vertex] + edge.weight;
                queue.push({distance_[edge.to], edge.to});
            }
        }
    }
}

void Graph::PrintAnswer(std::vector<int64_t>& virused, int64_t healthy) {
    bool possible = true;
    for (size_t i = 0; i < virused.size(); ++i) {
        if (distance_[virused[i]] <= distance_[healthy]) {
            possible = false;
            break;
        }
    }
    if (distance_[healthy] == kMax) {
        std::cout << "-1\n";
    } else {
        if (possible) {
            std::cout << distance_[healthy] << "\n";
        } else {
            std::cout << "-1\n";
        }
    }
}

void Graph::PrintGraph() {
    for (int64_t vertex = 1ll; vertex < vertex_number_ + 1ll; ++vertex) {
        std::cout << "Neighbours of " << vertex << " are: ";
        for (Edge& edge : graph_[vertex]) {
            std::cout << edge.to << " (" << edge.weight << "), ";
        }
        std::cout << "\n";
    }
}