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

class Graph {
private:
    struct Edge {
        int64_t from;
        int64_t to;
        int64_t weight;
    };

    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<bool> used_;
    std::vector<int64_t> distance_;
    std::vector<int64_t> prev_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    void Dijkstra(int64_t start, int64_t finish);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t weight = 0ll;
    int64_t start = 0ll;
    int64_t finish = 0ll;

    std::cin >> vertex_num >> start >> finish;

    Graph graph(vertex_num);
    for (int64_t from = 1ll; from < vertex_num + 1ll; ++from) {
        for (int64_t to = 1ll; to < vertex_num + 1ll; ++to) {
            std::cin >> weight;
            if (from == to || weight < 0) {
                continue;
            }
            graph.Insert(from, to, weight);
        }
    }

    graph.Dijkstra(start, finish);

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
    used_.resize(vertex_number_ + 1ll);
    used_.assign(vertex_number_ + 1ll, false);
    distance_.resize(vertex_number_ + 1ll);
    distance_.assign(vertex_number_ + 1ll, kMax);
    prev_.resize(vertex_number_ + 1ll);
    prev_.assign(vertex_number_ + 1ll, -1ll);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
}

void Graph::Dijkstra(int64_t start, int64_t finish) {
    distance_[start] = 0ll;
    for (int64_t vertex = 1ll; vertex < vertex_number_ + 1ll; ++vertex) {
        int64_t to = -1ll;
        for (int64_t j = 1ll; j < vertex_number_ + 1ll; ++j) {
            if (!used_[j] && (to == -1ll || distance_[j] < distance_[to])) {
                to = j;
            }
        }
        if (distance_[to] == kMax) {
            break;
        }
        used_[to] = true;
        for (auto edge : graph_[to]) {
            if (distance_[to] + edge.weight < distance_[edge.to]) {
                distance_[edge.to] = distance_[to] + edge.weight;
                prev_[edge.to] = to;
            }
        }
    }

    if (!used_[finish]) {
        std::cout << "-1" << std::endl;
    } else {
        std::cout << distance_[finish] << std::endl;
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