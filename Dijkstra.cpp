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
    std::vector<int64_t> prev_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    void Dijkstra(int64_t start);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t blocks_num = 0ll;
    int64_t vertex_num = 0ll;
    int64_t edge_num = 0ll;
    int64_t from_vertex = 0ll;
    int64_t to_vertex = 0ll;
    int64_t weight = 0ll;
    int64_t start = 0ll;

    std::cin >> blocks_num;
    for (int64_t i = 0ll; i < blocks_num; ++i) {
        std::cin >> vertex_num >> edge_num;
        Graph graph(vertex_num);
        for (int64_t i = 0ll; i < edge_num; ++i) {
            std::cin >> from_vertex >> to_vertex >> weight;
            graph.Insert(from_vertex + 1ll, to_vertex + 1ll, weight);
        }
        std::cin >> start;
        graph.Dijkstra(start + 1ll);
    }

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
    distance_.resize(vertex_number_ + 1ll);
    distance_.assign(vertex_number_ + 1ll, kMax);
    prev_.resize(vertex_number_ + 1ll);
    prev_.assign(vertex_number_ + 1ll, -1ll);
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
                prev_[edge.to] = vertex;
                queue.push({distance_[edge.to], edge.to});
            }
        }
    }
    for (int64_t vertex = 1ll; vertex < vertex_number_ + 1ll; ++vertex) {
        std::cout << distance_[vertex] << " ";
    }
    std::cout << std::endl;
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