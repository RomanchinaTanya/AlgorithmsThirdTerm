#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>

using VertexType = int32_t;

using FlowType = int64_t;

using WeightType = int64_t;

struct Neighbour {
    VertexType from;
    VertexType to;
    size_t back;
    FlowType capacity;
    FlowType flow;
};

class IGraph {
public:
    using Edge = Neighbour;

    virtual void InsertEdge(VertexType from_vertex, VertexType to_vertex, WeightType weight) = 0;
    virtual void InsertEdge(VertexType from_vertex, VertexType to_vertex) = 0;
    virtual const std::vector<Edge>& Neighbours(VertexType vertex) const = 0;
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
    void InsertEdge(VertexType from_vertex, VertexType to_vertex, WeightType weight) override;
    void InsertEdge(VertexType from_vertex, VertexType to_vertex) override;
    const std::vector<Edge>& Neighbours(VertexType vertex) const override;
    std::vector<Edge>& Neighbours(VertexType vertex) override;
    size_t Size() override;
};

FlowType MaxFlow(IGraph& graph, VertexType source, VertexType sink);

FlowType FindFlowDFS(IGraph& graph, VertexType source, VertexType sink);

FlowType VisitDFS(IGraph& graph, VertexType vertex, VertexType sink, FlowType current_flow, std::vector<bool>& used);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t vertex_num = 0;
    size_t edges_num = 0;
    VertexType from_vertex = 0;
    VertexType to_vertex = 0;
    FlowType capacity = 0;

    std::cin >> vertex_num >> edges_num;
    GraphList network(vertex_num);

    for (size_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex >> capacity;
        network.InsertEdge(from_vertex, to_vertex, capacity);
    }

    std::cout << MaxFlow(network, 1, vertex_num) << std::endl;

    return 0;
}

GraphList::GraphList(size_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1);
}

void GraphList::InsertEdge(VertexType from_vertex, VertexType to_vertex, WeightType weight) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, graph_[to_vertex].size(), weight, 0});
    graph_[to_vertex].push_back({to_vertex, from_vertex, graph_[from_vertex].size() - 1, 0, 0});
}

void GraphList::InsertEdge(VertexType from_vertex, VertexType to_vertex) {
    graph_[from_vertex].push_back({from_vertex, to_vertex, graph_[to_vertex].size(), 0, 0});
    graph_[to_vertex].push_back({to_vertex, from_vertex, graph_[from_vertex].size() - 1, 0, 0});
}

const std::vector<GraphList::Edge>& GraphList::Neighbours(VertexType vertex) const {
    return graph_[vertex];
}

std::vector<GraphList::Edge>& GraphList::Neighbours(VertexType vertex) {
    return graph_[vertex];
}

size_t GraphList::Size() {
    return vertex_number_;
}

FlowType MaxFlow(IGraph& graph, VertexType source, VertexType sink) {
    FlowType max_flow = 0;
    FlowType delta = FindFlowDFS(graph, source, sink);
    while (delta > 0) {
        max_flow += delta;
        delta = FindFlowDFS(graph, source, sink);
    }
    return max_flow;
}

FlowType FindFlowDFS(IGraph& graph, VertexType source, VertexType sink) {
    std::vector<bool> used(graph.Size() + 1, false);
    return VisitDFS(graph, source, sink, LONG_MAX, used);
}

FlowType VisitDFS(IGraph& graph, VertexType vertex, VertexType sink, FlowType current_flow, std::vector<bool>& used) {
    if (vertex == sink) {
        return current_flow;
    }
    used[vertex] = true;
    for (auto& edge : graph.Neighbours(vertex)) {
        if (!used[edge.to] && (edge.capacity - edge.flow) > 0) {
            FlowType delta = VisitDFS(graph, edge.to, sink, std::min(current_flow, edge.capacity - edge.flow), used);
            if (delta > 0) {
                edge.flow += delta;
                std::vector<IGraph::Edge>& neighbours = graph.Neighbours(edge.to);
                neighbours[edge.back].flow -= delta;
                return delta;
            }
        }
    }
    return 0;
}