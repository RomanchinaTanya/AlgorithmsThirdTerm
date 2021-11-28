#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t kMaxWeight = 100000;

const int64_t kMax = 100000001;

class Graph {
private:
    struct Edge {
        int64_t from;
        int64_t to;
        int64_t weight;
    };

    int64_t cycle_start_ = -1;
    int64_t vertex_number_;
    int64_t edges_number_;
    std::vector<Edge> graph_;
    std::vector<int64_t> distance_;
    std::vector<int64_t> prev_;

    bool Relax(Edge edge);

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    std::deque<int64_t> FordBellman();
    void PrintGraph();
};

void PrintAnswer(std::deque<int64_t>& cycle);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t weight = 0ll;

    std::cin >> vertex_num;
    Graph graph(vertex_num);

    for (int64_t i = 1ll; i < vertex_num + 1ll; ++i) {
        for (int64_t j = 1ll; j < vertex_num + 1ll; ++j) {
            std::cin >> weight;
            if (weight == kMaxWeight) {
                continue;
            }
            graph.Insert(i, j, weight);
        }
    }

    std::deque<int64_t> cycle = graph.FordBellman();

    PrintAnswer(cycle);

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.reserve((vertex_number_ + 1ll) * (vertex_number_ + 1ll));
    distance_.resize(vertex_number_ + 1ll, 0ll);
    prev_.resize(vertex_number_ + 1ll, -1ll);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_.push_back({from_vertex, to_vertex, weight});
}

std::deque<int64_t> Graph::FordBellman() {
    for (int64_t vertex = 1ll; vertex < vertex_number_; ++vertex) {
        for (Edge edge : graph_) {
            Relax(edge);
        }
    }

    for (Edge edge : graph_) {
        if (Relax(edge)) {
            cycle_start_ = edge.to;
        }
    }

    std::deque<int64_t> cycle;

    if (cycle_start_ != -1) {
        for (int64_t i = 1ll; i < vertex_number_; ++i) {
            cycle_start_ = prev_[cycle_start_];
        }
        cycle.push_front(cycle_start_);
        for (int64_t vertex = prev_[cycle_start_]; vertex != cycle_start_; vertex = prev_[vertex]) {
            cycle.push_front(vertex);
        }
        cycle.push_front(cycle_start_);
    }

    return cycle;
}

bool Graph::Relax(Edge edge) {
    if (distance_[edge.from] < kMax) {
        if (distance_[edge.to] > distance_[edge.from] + edge.weight) {
            distance_[edge.to] = distance_[edge.from] + edge.weight;
            prev_[edge.to] = edge.from;
            return true;
        }
        return false;
    }
    return false;
}

void Graph::PrintGraph() {
    for (Edge& edge : graph_) {
        std::cout << edge.from << " " << edge.to << " (" << edge.weight << "), ";
    }
    std::cout << "\n";
}

void PrintAnswer(std::deque<int64_t>& cycle) {
    if (cycle.empty()) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
        std::cout << cycle.size() << "\n";
        for (int64_t vertex : cycle) {
            std::cout << vertex << " ";
        }
        std::cout << "\n";
    }
}