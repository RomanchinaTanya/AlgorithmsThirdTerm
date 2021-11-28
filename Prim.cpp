#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <queue>

template <class TElem>
using MinHeap = std::priority_queue<TElem, std::vector<TElem>, std::greater<TElem>>;

struct Edge {
    int64_t from;
    int64_t to;
    int64_t weight;
};

class Graph {
private:
    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<bool> used_;
    std::vector<int64_t> dist_;
    std::vector<int64_t> prev_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    int64_t Prim();
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t edges_num = 0ll;
    int64_t from = 0ll;
    int64_t to = 0ll;
    int64_t weight = 0ll;
    int64_t total_weight = 0ll;

    std::cin >> vertex_num >> edges_num;
    Graph graph(vertex_num);

    for (int64_t i = 0ll; i < edges_num; ++i) {
        std::cin >> from >> to >> weight;
        graph.Insert(from, to, weight);
    }

    total_weight = graph.Prim();

    std::cout << total_weight << std::endl;

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
    used_.resize(vertex_number_ + 1ll);
    used_.assign(vertex_number_ + 1ll, false);
    dist_.resize(vertex_number_ + 1ll);
    dist_.assign(vertex_number_ + 1ll, LONG_MAX);
    prev_.resize(vertex_number_ + 1ll);
    prev_.assign(vertex_number_ + 1ll, -1ll);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
    graph_[to_vertex].push_back({to_vertex, from_vertex, weight});
}

int64_t Graph::Prim() {
    MinHeap<std::pair<int64_t, int64_t>> queue;
    queue.push({0ll, 1ll});
    int64_t total_weight = 0ll;

    while (!queue.empty()) {
        auto[weight, vertex] = queue.top();
        queue.pop();
        if (!used_[vertex]) {
            used_[vertex] = true;
            if (prev_[vertex] != -1ll) {
                total_weight += weight;
            }
            for (auto edge : graph_[vertex]) {
                if (!used_[edge.to] && dist_[edge.to] > edge.weight) {
                    prev_[edge.to] = vertex;
                    dist_[edge.to] = edge.weight;
                    queue.push({edge.weight, edge.to});
                }
            }
        }
    }
    return total_weight;
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