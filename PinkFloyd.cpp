#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <queue>

const int64_t kMax = LLONG_MAX;

class Graph {
private:
    struct Edge {
        int64_t weight;
        int64_t edge_id;
    };

    int64_t initial_edge_id_ = 0;
    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<std::vector<int64_t>> parent_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight);
    void FloydWarshall();
    void PrintAnswer(std::vector<int64_t>& must_visit);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t towns_number = 0;
    int64_t flights_number = 0;
    int64_t concerts_number = 0;
    int64_t from = 0;
    int64_t to = 0;
    int64_t weight = 0;
    std::cin >> towns_number >> flights_number >> concerts_number;

    std::vector<int64_t> must_visit(concerts_number);

    Graph graph(towns_number);

    for (int64_t i = 0; i < flights_number; ++i) {
        std::cin >> from >> to >> weight;
        graph.Insert(--from, --to, -weight);
    }

    for (int64_t i = 0; i < concerts_number; ++i) {
        std::cin >> must_visit[i];
        --must_visit[i];
    }

    // std::cout << "First:\n";
    // graph.PrintGraph();

    graph.FloydWarshall();

    // std::cout << "Second\n";
    // graph.PrintGraph();

    graph.PrintAnswer(must_visit);
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_, std::vector<Edge>(vertex_number_, {kMax, -1}));
    parent_.resize(vertex_number_, std::vector<int64_t>(vertex_number_, -1));
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight) {
    graph_[from_vertex][to_vertex] = {weight, ++initial_edge_id_};
    parent_[from_vertex][to_vertex] = from_vertex;
}

void Graph::FloydWarshall() {
    for (int64_t k = 0; k < vertex_number_; ++k) {
        for (int64_t from = 0; from < vertex_number_; ++from) {
            for (int64_t to = 0; to < vertex_number_; ++to) {
                if (graph_[from][k].weight != kMax && graph_[k][to].weight != kMax) {
                    if (graph_[from][k].weight + graph_[k][to].weight < graph_[from][to].weight) {
                        graph_[from][to].weight = graph_[from][k].weight + graph_[k][to].weight;
                        parent_[from][to] = parent_[k][to];
                    }
                }
            }
        }
    }
}

void Graph::PrintAnswer(std::vector<int64_t>& must_visit) {
    bool infinitely_kind = false;
    for (int64_t vertex : must_visit) {
        if (graph_[vertex][vertex].weight < 0) {
            infinitely_kind = true;
            break;
        }
    }
    if (infinitely_kind) {
        std::cout << "infinitely kind\n";
    } else {
        infinitely_kind = false;
        std::deque<int64_t> answer;
        for (auto i = static_cast<int64_t>(must_visit.size() - 2); i >= 0; --i) {
            for (int64_t vertex = must_visit[i + 1]; vertex != must_visit[i]; vertex = parent_[must_visit[i]][vertex]) {
                if (parent_[must_visit[i]][vertex] == -1) {
                    infinitely_kind = true;
                    break;
                }
                answer.push_front(graph_[parent_[must_visit[i]][vertex]][vertex].edge_id);
            }
            if (infinitely_kind) {
                break;
            }
        }
        if (infinitely_kind) {
            std::cout << "infinitely kind\n";
        } else {
            std::cout << answer.size() << "\n";
            for (int64_t edge : answer) {
                std::cout << edge << " ";
            }
            std::cout << "\n";
        }
    }
}

void Graph::PrintGraph() {
    for (int64_t i = 0; i < vertex_number_; ++i) {
        for (int64_t j = 0; j < vertex_number_; ++j) {
            if (graph_[i][j].weight == kMax) {
                std::cout << "(-1, -1)   ";
            } else {
                std::cout << "(" << graph_[i][j].weight << ", " << graph_[i][j].edge_id << ")   ";
            }
        }
        std::cout << "\n";
    }
    for (int64_t i = 0; i < vertex_number_; ++i) {
        for (int64_t j = 0; j < vertex_number_; ++j) {
            std::cout << parent_[i][j] << " ";
        }
        std::cout << "\n";
    }
}