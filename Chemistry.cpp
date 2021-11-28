#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

class Graph {
private:
    struct Edge {
        int64_t from;
        int64_t to;
        size_t back;
        int64_t capacity;
        int64_t flow;
    };

    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;

    int64_t FindFlowDFS(int64_t source, int64_t sink);
    int64_t VisitDFS(int64_t vertex, int64_t sink, int64_t current_flow, std::vector<bool>& used);

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Fill(std::vector<std::vector<char>>& table, int64_t& left_sum, int64_t& right_sum, int64_t size_n,
              int64_t size_m);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity);
    void PrintGraph();
    int64_t MaxFlow(int64_t source, int64_t sink);
};

int64_t Valence(char element);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t size_n = 0;
    int64_t size_m = 0;
    int64_t left_sum = 0;
    int64_t right_sum = 0;
    std::cin >> size_n >> size_m;

    std::vector<std::vector<char>> table(size_n, std::vector<char>(size_m, '0'));

    for (int64_t i = 0; i < size_n; ++i) {
        for (int64_t j = 0; j < size_m; ++j) {
            std::cin >> table[i][j];
        }
    }

    Graph graph(size_n * size_m + 2);

    graph.Fill(table, left_sum, right_sum, size_n, size_m);

    int max_flow = graph.MaxFlow(size_n * size_m, size_n * size_m + 1);

    if (max_flow == left_sum && max_flow == right_sum && max_flow != 0) {
        std::cout << "Valid\n";
    } else {
        std::cout << "Invalid\n";
    }

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, graph_[to_vertex].size(), capacity, 0ll});
    graph_[to_vertex].push_back({to_vertex, from_vertex, graph_[from_vertex].size() - 1ll, 0ll, 0ll});
}

void Graph::Fill(std::vector<std::vector<char>>& table, int64_t& left_sum, int64_t& right_sum, int64_t size_n,
                 int64_t size_m) {

    for (int64_t i = 0; i < size_n; ++i) {
        for (int64_t j = 0; j < size_m; ++j) {
            if (table[i][j] != '.') {
                if (j < size_m - 1 && table[i][j + 1] != '.') {
                    if (!((i + j) % 2)) {
                        Insert(i * size_m + j, i * size_m + j + 1, 1);
                    }
                }
                if (i < size_n - 1 && table[i + 1][j] != '.') {
                    if (!((i + j) % 2)) {
                        Insert(i * size_m + j, (i + 1) * size_m + j, 1);
                    }
                }
                if (i > 0 && table[i - 1][j] != '.') {
                    if (!((i + j) % 2)) {
                        Insert(i * size_m + j, (i - 1) * size_m + j, 1);
                    }
                }
                if (j > 0 && table[i][j - 1] != '.') {
                    if (!((i + j) % 2)) {
                        Insert(i * size_m + j, i * size_m + j - 1, 1);
                    }
                }
                if (!((i + j) % 2)) {
                    left_sum += Valence(table[i][j]);
                    Insert(size_n * size_m, i * size_m + j, Valence(table[i][j]));
                } else {
                    right_sum += Valence(table[i][j]);
                    Insert(i * size_m + j, size_n * size_m + 1, Valence(table[i][j]));
                }
            }
        }
    }
}

int64_t Graph::MaxFlow(int64_t source, int64_t sink) {
    int64_t max_flow = 0ll;
    int64_t delta = FindFlowDFS(source, sink);
    while (delta > 0ll) {
        max_flow += delta;
        delta = FindFlowDFS(source, sink);
    }
    return max_flow;
}

int64_t Graph::FindFlowDFS(int64_t source, int64_t sink) {
    std::vector<bool> used(vertex_number_ + 1ll, false);
    return VisitDFS(source, sink, LONG_MAX, used);
}

int64_t Graph::VisitDFS(int64_t vertex, int64_t sink, int64_t current_flow, std::vector<bool>& used) {
    if (vertex == sink) {
        return current_flow;
    }
    used[vertex] = true;
    for (Edge& edge : graph_[vertex]) {
        if (!used[edge.to] && (edge.capacity - edge.flow) > 0ll) {
            int64_t flow = VisitDFS(edge.to, sink, std::min(current_flow, edge.capacity - edge.flow), used);
            if (flow > 0ll) {
                edge.flow += flow;
                graph_[edge.to][edge.back].flow -= flow;
                return flow;
            }
        }
    }
    return 0ll;
}

void Graph::PrintGraph() {
    for (int64_t vertex = 0ll; vertex < vertex_number_; ++vertex) {
        std::cout << "Neighbours of " << vertex << " are: ";
        for (Edge& edge : graph_[vertex]) {
            std::cout << edge.to << " (" << edge.capacity << ", " << edge.flow << "), ";
        }
        std::cout << "\n";
    }
}

int64_t Valence(char element) {
    if (element == 'H') {
        return 1;
    }
    if (element == 'O') {
        return 2;
    }
    if (element == 'N') {
        return 3;
    }
    if (element == 'C') {
        return 4;
    }
    return 0;
}