#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>
#include <queue>

using Vertex = int64_t;

struct Neighbour {
    Vertex from;
    Vertex to;
    int64_t weight;
};

template <class TElem>
using MinHeap = std::priority_queue<TElem, std::vector<TElem>, std::greater<TElem>>;

class IGraph {
public:
    using Edge = Neighbour;

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) = 0;
    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex) = 0;
    virtual const std::vector<Edge>& Neighbours(Vertex vertex) = 0;
    virtual int64_t Size() = 0;
};

class GraphList : public IGraph {
private:
    int64_t vertex_number_;
    std::vector<std::vector<Edge>> graph_;

public:
    using Edge = Neighbour;

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int64_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex) override;
    void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) override;
    const std::vector<Edge>& Neighbours(Vertex vertex) override;
    int64_t Size() override;
};

void FillGraphWithEdges(IGraph& graph, int64_t vertex_num);

std::vector<IGraph::Edge> PrimFindWeightOfMST(IGraph& graph);

void PrintAnswer(std::vector<IGraph::Edge>& mst);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0;
    std::cin >> vertex_num;

    GraphList graph(vertex_num + 1);

    FillGraphWithEdges(graph, vertex_num);

    std::vector<GraphList::Edge> mst = PrimFindWeightOfMST(graph);

    PrintAnswer(mst);

    return 0;
}

GraphList::GraphList(int64_t vertex_num) : vertex_number_(vertex_num) {
    graph_.resize(vertex_number_ + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) {
    graph_[from_vertex].push_back(Edge{from_vertex, to_vertex, weight});
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    graph_[from_vertex].push_back(Edge{from_vertex, to_vertex, 0});
}

const std::vector<GraphList::Edge>& GraphList::Neighbours(Vertex vertex) {
    return graph_[vertex];
}

int64_t GraphList::Size() {
    return vertex_number_;
}

void FillGraphWithEdges(IGraph& graph, int64_t vertex_num) {
    int64_t weight = 0;
    for (Vertex from = 0; from < vertex_num; ++from) {
        for (Vertex to = 0; to < vertex_num; ++to) {
            std::cin >> weight;
            if (from != to) {
                graph.InsertEdge(from, to, weight);
            }
        }
    }
    for (Vertex vertex = 0; vertex < vertex_num; ++vertex) {
        std::cin >> weight;
        graph.InsertEdge(vertex_num, vertex, weight);
        graph.InsertEdge(vertex, vertex_num, weight);
    }
}

std::vector<IGraph::Edge> PrimFindWeightOfMST(IGraph& graph) {
    std::vector<int64_t> weights(graph.Size() + 1, LONG_MAX);
    std::vector<Vertex> parent(graph.Size() + 1, -1);
    std::vector<bool> used(graph.Size() + 1, false);
    MinHeap<std::pair<int64_t, Vertex>> queue;
    std::vector<IGraph::Edge> mst;
    mst.reserve(graph.Size() + 1);
    queue.push({0, 0});

    while (!queue.empty()) {
        auto[weight, vertex] = queue.top();
        queue.pop();
        if (!used[vertex]) {
            used[vertex] = true;
            if (parent[vertex] != -1) {
                mst.push_back(IGraph::Edge{parent[vertex], vertex, weight});
            }
            for (const auto& edge : graph.Neighbours(vertex)) {
                if (!used[edge.to] && weights[edge.to] > edge.weight) {
                    parent[edge.to] = vertex;
                    weights[edge.to] = edge.weight;
                    queue.push({edge.weight, edge.to});
                }
            }
        }
    }

    return mst;
}

void PrintAnswer(std::vector<IGraph::Edge>& mst) {
    int64_t total_weight = 0;
    for (size_t i = 0; i < mst.size(); ++i) {
        total_weight += mst[i].weight;
    }
    std::cout << total_weight << "\n";
}