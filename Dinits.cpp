#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t kMax = LLONG_MAX;

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
    std::vector<int64_t> level_;
    std::vector<int64_t> start_;
    std::vector<std::vector<Edge>> graph_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity);
    int64_t DinitsMaxFlow(int64_t source, int64_t sink);
    int64_t FindFlowDFS(int64_t source, int64_t sink);
    int64_t VisitDFS(int64_t vertex, int64_t sink, int64_t current_flow);
    bool BuildNetworkBFS(int64_t source, int64_t sink);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_number = 0;
    int64_t edges_number = 0;
    int64_t from_vertex = 0;
    int64_t to_vertex = 0;
    int64_t capacity = 0;
    std::cin >> vertex_number >> edges_number;

    Graph graph(vertex_number);

    for (int64_t i = 0; i < edges_number; ++i) {
        std::cin >> from_vertex >> to_vertex >> capacity;
        graph.Insert(--from_vertex, --to_vertex, capacity);
    }

    std::cout << graph.DinitsMaxFlow(0, vertex_number - 1) << "\n";
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1);
    level_.resize(vertex_number_ + 1);
    start_.resize(vertex_number_ + 1);
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, graph_[to_vertex].size(), capacity, 0});
    graph_[to_vertex].push_back({to_vertex, from_vertex, graph_[from_vertex].size() - 1, 0, 0});
}

int64_t Graph::DinitsMaxFlow(int64_t source, int64_t sink) {
    int64_t max_flow = 0;
    int64_t flow = 0;

    while (BuildNetworkBFS(source, sink) && (flow = FindFlowDFS(source, sink)) > 0) {
        max_flow += flow;
    }

    return max_flow;
}

int64_t Graph::FindFlowDFS(int64_t source, int64_t sink) {
    start_.assign(vertex_number_, 0);
    int64_t flow = 0;
    int64_t delta = VisitDFS(source, sink, kMax);

    while (delta > 0) {
        flow += delta;
        delta = VisitDFS(source, sink, kMax);
    }

    return flow;
}

int64_t Graph::VisitDFS(int64_t vertex, int64_t sink, int64_t current_flow) {
    if (vertex == sink) {
        return current_flow;
    }

    for (auto& edge_id = start_[vertex]; edge_id < static_cast<int64_t>(graph_[vertex].size()); ++edge_id) {
        Edge& edge = graph_[vertex][edge_id];
        if ((edge.capacity - edge.flow) > 0 && level_[edge.to] == level_[vertex] + 1) {
            int64_t delta = VisitDFS(edge.to, sink, std::min(current_flow, edge.capacity - edge.flow));
            if (delta > 0) {
                edge.flow += delta;
                graph_[edge.to][edge.back].flow -= delta;
                return delta;
            }
        }
    }

    return 0;
}

bool Graph::BuildNetworkBFS(int64_t source, int64_t sink) {
    level_.assign(vertex_number_, -1);
    level_[source] = 0;

    std::queue<int64_t> queue;
    queue.push(source);

    while (!queue.empty()) {
        int64_t vertex = queue.front();
        queue.pop();

        for (Edge edge : graph_[vertex]) {
            if ((edge.capacity - edge.flow) > 0 && level_[edge.to] == -1) {
                level_[edge.to] = level_[edge.from] + 1;
                queue.push(edge.to);
            }
        }
    }

    return level_[sink] != -1;
}

void Graph::PrintGraph() {
    for (int64_t vertex = 1; vertex < vertex_number_ + 1; ++vertex) {
        std::cout << "Neighbours of " << vertex << " are: ";
        for (Edge& edge : graph_[vertex]) {
            std::cout << edge.to << " (" << edge.capacity << ", " << edge.flow << "), ";
        }
        std::cout << "\n";
    }
}