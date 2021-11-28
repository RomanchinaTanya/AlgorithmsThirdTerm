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

template <class TElem>
using MinHeap = std::priority_queue<TElem, std::vector<TElem>, std::greater<TElem>>;

class Graph {
private:
    struct Edge {
        int64_t from;
        int64_t to;
        int64_t weight;
        int64_t time;
    };

    struct Parent {
        int64_t parent;
        int64_t time;
    };

    int64_t vertex_number_;
    int64_t max_time_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<std::vector<Parent>> parent_;
    std::vector<std::vector<int64_t>> distance_;

public:
    Graph() = default;
    ~Graph() = default;
    Graph(int64_t vertex_num, int64_t max_time);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight, int64_t time);
    void FindDistanceWithTime();
    void Dijkstra(int64_t time);
    void PrintAnswer();
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_number = 0;
    int64_t edges_number = 0;
    int64_t max_time = 0;
    int64_t from = 0;
    int64_t to = 0;
    int64_t weight = 0;
    int64_t time = 0;
    std::cin >> vertex_number >> edges_number >> max_time;

    Graph graph(vertex_number, max_time);

    for (int64_t i = 0; i < edges_number; ++i) {
        std::cin >> from >> to >> weight >> time;
        graph.Insert(--from, --to, weight, time);
    }

    // graph.PrintGraph();

    graph.FindDistanceWithTime();

    graph.PrintAnswer();
}

Graph::Graph(int64_t vertex_num, int64_t max_time) : vertex_number_(vertex_num), max_time_(max_time + 1) {
    graph_.resize(vertex_number_);
    parent_.resize(max_time_, std::vector<Parent>(vertex_number_, {-1, -1}));
    distance_.resize(max_time_, std::vector<int64_t>(vertex_number_, kMax));
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t weight, int64_t time) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight, time});
    graph_[to_vertex].push_back({to_vertex, from_vertex, weight, time});
}

void Graph::Dijkstra(int64_t time) {
    MinHeap<std::pair<int64_t, int64_t>> queue;
    for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
        if (distance_[time][vertex] == kMax) {
            continue;
        }
        queue.push({distance_[time][vertex], vertex});
    }
    while (!queue.empty()) {
        auto[current_dist, vertex] = queue.top();
        queue.pop();
        if (current_dist > distance_[time][vertex]) {
            continue;
        }
        for (auto edge : graph_[vertex]) {
            if (edge.time == 0) {
                if (distance_[time][edge.to] > distance_[time][vertex] + edge.weight) {
                    distance_[time][edge.to] = distance_[time][vertex] + edge.weight;
                    parent_[time][edge.to] = {vertex, edge.time};
                    queue.push({distance_[time][edge.to], edge.to});
                }
            }
        }
    }
}

void Graph::FindDistanceWithTime() {
    distance_[0][0] = 0;
    for (int64_t time = 0; time < max_time_; ++time) {
        Dijkstra(time);
        for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
            for (Edge edge : graph_[vertex]) {
                if (edge.time + time < max_time_ && distance_[time][edge.from] < kMax) {
                    if (distance_[edge.time + time][edge.to] > distance_[time][edge.from] + edge.weight) {
                        distance_[edge.time + time][edge.to] = distance_[time][edge.from] + edge.weight;
                        parent_[edge.time + time][edge.to] = {edge.from, edge.time};
                    }
                }
            }
        }
    }
}

void Graph::PrintAnswer() {
    int64_t min_distance = kMax;
    int64_t min_time = kMax;
    for (int64_t time = 0; time < max_time_; ++time) {
        if (distance_[time][vertex_number_ - 1] < min_distance) {
            min_distance = distance_[time][vertex_number_ - 1];
            min_time = time;
        }
    }
    if (min_time == kMax) {
        std::cout << "-1\n";
    } else {
        int64_t vertex = vertex_number_ - 1;
        std::deque<int64_t> answer;
        while (vertex != -1) {
            answer.push_front(vertex + 1);
            int64_t old_vertex = vertex;
            vertex = parent_[min_time][vertex].parent;
            min_time -= parent_[min_time][old_vertex].time;
        }
        std::cout << min_distance << "\n";
        std::cout << answer.size() << "\n";
        for (int64_t vertex : answer) {
            std::cout << vertex << " ";
        }
        std::cout << "\n";
    }
}

void Graph::PrintGraph() {
    for (int64_t vertex = 0; vertex < vertex_number_; ++vertex) {
        std::cout << "Neighbours of " << vertex << " are: ";
        for (Edge& edge : graph_[vertex]) {
            std::cout << edge.to << " (" << edge.weight << ", " << edge.time << "), ";
        }
        std::cout << "\n";
    }
}