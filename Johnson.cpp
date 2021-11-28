#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>
#include <queue>

const int64_t kMax = 10000001;

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
    int64_t edges_number_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<int64_t> phi_;
    std::vector<std::vector<int64_t>> distance_;

    void Relax(Edge edge);

public:
    Graph() = default;
    ~Graph() = default;
    Graph(int64_t vertex_num, int64_t edge_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    void FordBellman();
    int64_t FindMaxShortestPathDijkstra();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0;
    int64_t edges_num = 0;
    int64_t weight = 0;
    int64_t from = 0;
    int64_t to = 0;
    std::cin >> vertex_num >> edges_num;
    Graph graph(vertex_num, edges_num);

    for (int64_t i = 0; i < edges_num; ++i) {
        std::cin >> from >> to >> weight;
        graph.Insert(from, to, weight);
    }

    graph.FordBellman();

    std::cout << graph.FindMaxShortestPathDijkstra() << "\n";

    return 0;
}

Graph::Graph(int64_t vertex_num, int64_t edge_num) : vertex_number_(vertex_num), edges_number_(edge_num) {
    graph_.resize(vertex_number_ + 1);
    phi_.resize(vertex_number_ + 1, kMax);
    distance_.resize(vertex_number_ + 1, std::vector<int64_t>(vertex_number_ + 1, kMax));
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
}

void Graph::FordBellman() {
    for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
        phi_[vertex] = 0;
    }
    for (int64_t i = 0; i < vertex_number_ - 1; ++i) {
        for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
            for (Edge edge : graph_[vertex]) {
                Relax(edge);
            }
        }
    }
}

void Graph::Relax(Edge edge) {
    if (phi_[edge.from] < kMax) {
        if (phi_[edge.to] > phi_[edge.from] + edge.weight) {
            phi_[edge.to] = phi_[edge.from] + edge.weight;
        }
    }
}

int64_t Graph::FindMaxShortestPathDijkstra() {
    int64_t max_shortest_path = -kMax;
    for (int64_t start = 0; start < vertex_number_; ++start) {
        MinHeap<std::pair<int64_t, int64_t>> queue;
        distance_[start][start] = 0;
        queue.push({0, start});
        while (!queue.empty()) {
            auto[current_dist, vertex] = queue.top();
            queue.pop();
            if (current_dist > distance_[start][vertex]) {
                continue;
            }
            for (auto edge : graph_[vertex]) {
                if (distance_[start][edge.to] >
                    distance_[start][vertex] + edge.weight + (phi_[edge.from] - phi_[edge.to])) {
                    distance_[start][edge.to] =
                        distance_[start][vertex] + edge.weight + (phi_[edge.from] - phi_[edge.to]);
                    queue.push({distance_[start][edge.to], edge.to});
                }
            }
        }
        for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
            if (vertex != start && distance_[start][vertex] != kMax) {
                max_shortest_path =
                    std::max(max_shortest_path, distance_[start][vertex] - (phi_[start] - phi_[vertex]));
            }
        }
    }
    return max_shortest_path;
}