#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>

const size_t kMax = 2009000999;

using VertexType = size_t;

struct Neighbour {
    VertexType from;
    VertexType to;
    size_t weight;
};

struct HeapElement {
    size_t distance;
    VertexType vertex;
};

struct Comparator {
    std::greater<size_t> greater;

    bool operator()(HeapElement first, HeapElement second) {
        return greater(first.distance, second.distance);
    }
};

template <class TElem>
using MinHeap = std::priority_queue<TElem, std::vector<TElem>, Comparator>;

class IGraph {
public:
    using Edge = Neighbour;

    virtual void InsertEdge(VertexType from_vertex, VertexType to_vertex, size_t weight) = 0;
    virtual std::vector<Edge>& Neighbours(VertexType vertex) = 0;
    virtual size_t Size() = 0;
};

class GraphList : public IGraph {
private:
    size_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;

public:
    using Edge = Neighbour;

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(size_t vertex_num);
    void InsertEdge(VertexType from_vertex, VertexType to_vertex, size_t weight) override;
    std::vector<Edge>& Neighbours(VertexType vertex) override;
    size_t Size() override;
};

void FindMaxFloor(std::vector<std::vector<VertexType>>& lifts, size_t lifts_number, size_t& max_floor);

void FillGraphWithEdges(IGraph& graph, std::vector<std::vector<VertexType>>& lifts, size_t lifts_number,
                        size_t max_floor, size_t ledder_up_cost, size_t ledder_down_cost, size_t lift_in_cost,
                        size_t lift_out_cost);

size_t FindShortestPathDijkstra(IGraph& graph, VertexType start, VertexType finish);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t floors_number = 0;
    size_t ledder_up_cost = 0;
    size_t ledder_down_cost = 0;
    size_t lift_in_cost = 0;
    size_t lift_out_cost = 0;
    size_t lifts_number = 0;

    std::cin >> floors_number >> ledder_up_cost >> ledder_down_cost >> lift_in_cost >> lift_out_cost >> lifts_number;
    std::vector<std::vector<VertexType>> lifts(lifts_number + 1);
    size_t max_floor = floors_number;

    FindMaxFloor(lifts, lifts_number, max_floor);

    GraphList graph(max_floor + lifts_number);

    FillGraphWithEdges(graph, lifts, lifts_number, max_floor, ledder_up_cost, ledder_down_cost, lift_in_cost,
                       lift_out_cost);

    size_t distance = FindShortestPathDijkstra(graph, 0, floors_number - 1);

    std::cout << distance << "\n";

    return 0;
}

GraphList::GraphList(size_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1ll);
}

void GraphList::InsertEdge(VertexType from_vertex, VertexType to_vertex, size_t weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, weight});
}

std::vector<GraphList::Edge>& GraphList::Neighbours(VertexType vertex) {
    return graph_[vertex];
}

size_t GraphList::Size() {
    return vertex_number_;
}

size_t FindShortestPathDijkstra(IGraph& graph, VertexType start, VertexType finish) {
    std::vector<size_t> distance(graph.Size() + 1, kMax);
    MinHeap<HeapElement> queue;

    distance[start] = 0ll;
    queue.push({0ll, start});

    while (!queue.empty()) {
        auto[current_dist, vertex] = queue.top();
        queue.pop();
        if (current_dist > distance[vertex]) {
            continue;
        }
        for (auto& edge : graph.Neighbours(vertex)) {
            if (distance[edge.to] > distance[vertex] + edge.weight) {
                distance[edge.to] = distance[vertex] + edge.weight;
                queue.push({distance[edge.to], edge.to});
            }
        }
    }

    return distance[finish];
}

void FindMaxFloor(std::vector<std::vector<VertexType>>& lifts, size_t lifts_number, size_t& max_floor) {
    size_t lift = 0;
    VertexType floor = 0;

    for (size_t i = 0; i < lifts_number; ++i) {
        std::cin >> lift;
        for (size_t j = 0; j < lift; ++j) {
            std::cin >> floor;
            max_floor = std::max(max_floor, floor);
            lifts[i].push_back(floor);
        }
    }
}

void FillGraphWithEdges(IGraph& graph, std::vector<std::vector<VertexType>>& lifts, size_t lifts_number,
                        size_t max_floor, size_t ledder_up_cost, size_t ledder_down_cost, size_t lift_in_cost,
                        size_t lift_out_cost) {
    for (VertexType i = 0; i < lifts_number; ++i) {
        for (VertexType j = 0; j < lifts[i].size(); ++j) {
            graph.InsertEdge(lifts[i][j] - 1, max_floor + i, lift_in_cost);
            graph.InsertEdge(max_floor + i, lifts[i][j] - 1, lift_out_cost);
        }
    }
    for (VertexType vertex = 0; vertex < max_floor - 1; ++vertex) {
        graph.InsertEdge(vertex, vertex + 1, ledder_up_cost);
        graph.InsertEdge(vertex + 1, vertex, ledder_down_cost);
    }
}