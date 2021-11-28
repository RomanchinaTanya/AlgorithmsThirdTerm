#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <map>

class Graph {
private:
    struct Edge {
        int64_t capacity;
        int64_t flow;
    };

    int64_t vertex_number_;
    std::vector<std::map<int64_t, Edge>> graph_;

public:
    Graph() = default;
    ~Graph() = default;
    explicit Graph(int64_t vertex_num);
    void Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity);
    int64_t FindFlowBFS(int64_t source, int64_t sink, std::vector<int64_t>& prev);
    int64_t MaxFlow(int64_t source, int64_t sink);
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
    int64_t capacity = 0ll;

    std::cin >> vertex_num >> edges_num;

    Graph network(vertex_num);

    for (int64_t i = 0ll; i < edges_num; ++i) {
        std::cin >> from >> to >> capacity;
        network.Insert(from, to, capacity);
    }

    std::cout << network.MaxFlow(1ll, vertex_num) << "\n";

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number_(vertex_num), graph_(vertex_num + 1ll) {
}

void Graph::Insert(int64_t from_vertex, int64_t to_vertex, int64_t capacity) {
    graph_[from_vertex][to_vertex] = {capacity, 0ll};
}

int64_t Graph::FindFlowBFS(int64_t source, int64_t sink, std::vector<int64_t>& prev) {
    prev.assign(vertex_number_ + 1ll, 0ll);
    std::queue<std::pair<int64_t, int64_t>> queue;
    queue.push({source, LONG_MAX});
    prev[source] = -1ll;

    while (!queue.empty()) {
        auto[vertex, flow] = queue.front();
        queue.pop();
        for (auto& neighbour : graph_[vertex]) {
            if (prev[neighbour.first] == 0ll && (neighbour.second.capacity - neighbour.second.flow) > 0ll) {
                prev[neighbour.first] = vertex;
                if (neighbour.first == sink) {
                    return std::min(flow, neighbour.second.capacity - neighbour.second.flow);
                }
                queue.push({neighbour.first, std::min(flow, neighbour.second.capacity - neighbour.second.flow)});
            }
        }
    }
    return 0;
}

int64_t Graph::MaxFlow(int64_t source, int64_t sink) {
    std::vector<int64_t> prev(vertex_number_ + 1ll, 0ll);
    int64_t flow = FindFlowBFS(source, sink, prev);
    int64_t max_flow = 0ll;
    while (flow > 0ll) {
        max_flow += flow;
        for (int64_t vertex = sink; vertex != source; vertex = prev[vertex]) {
            graph_[prev[vertex]][vertex].flow += flow;
            graph_[vertex][prev[vertex]].flow -= flow;
        }
        flow = FindFlowBFS(source, sink, prev);
    }
    return max_flow;
}

void Graph::PrintGraph() {
    for (int64_t vertex = 1ll; vertex < vertex_number_ + 1ll; ++vertex) {
        std::cout << "Neighbours of " << vertex << " are: ";
        for (auto edge : graph_[vertex]) {
            std::cout << edge.first << " (" << edge.second.capacity << ", " << edge.second.flow << "), ";
        }
        std::cout << "\n";
    }
}