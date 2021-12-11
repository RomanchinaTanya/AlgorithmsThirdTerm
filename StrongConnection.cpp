#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

using Vertex = int32_t;

class Graph {
public:

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex) = 0;
    virtual std::vector<Vertex>& Neighbours(Vertex vertex) = 0;
    virtual int64_t Size() = 0;
};

class GraphList : public Graph {
private:

    int64_t vertex_number;
    std::vector<std::vector<Vertex>> list;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int64_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex);
    std::vector<Vertex>& Neighbours(Vertex vertex);
    int64_t Size();
};

std::vector<int64_t> FindComponents(Graph& ordinary_graph, int64_t& component_number);

void OrdinaryDFS(Graph& ordinary_graph, Vertex vertex, std::vector<Vertex>& order,
              std::vector<bool>& used);

void TransposedDFS(Graph& transposed_graph, Vertex vertex, std::vector<Vertex>& component,
               std::vector<bool>& used);

void FillTransposedGraph(Graph& ordinary_graph, Graph& transposed_graph);

void PrintAnswer(int64_t component_number, std::vector<int64_t>& belongs_to);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0;
    int64_t edges_num = 0;
    Vertex from_vertex = 0;
    Vertex to_vertex = 0;
    std::cin >> vertex_num >> edges_num;

    GraphList graph(vertex_num);
    for (int64_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex;
        graph.InsertEdge(from_vertex, to_vertex);
    }

    int64_t component_number = 1;
    std::vector<int64_t> belongs_to = FindComponents(graph, component_number);

    PrintAnswer(component_number, belongs_to);

    return 0;
}

GraphList::GraphList(int64_t vertex_num) : vertex_number(vertex_num) {
    list.resize(vertex_num + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    list[from_vertex].push_back(to_vertex);
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return list[vertex];
}

int64_t GraphList::Size() {
    return vertex_number;
}

std::vector<int64_t> FindComponents(Graph& ordinary_graph, int64_t& component_number) {
    GraphList transposed_graph(ordinary_graph.Size());
    FillTransposedGraph(ordinary_graph, transposed_graph);

    std::vector<int64_t> belongs_to(ordinary_graph.Size() + 1, 0);
    std::vector<bool> used(ordinary_graph.Size() + 1, false);
    std::vector<Vertex> order;
    std::vector<Vertex> component;

    for (Vertex vertex = 1; vertex < ordinary_graph.Size() + 1; ++vertex) {
        if (!used[vertex]) {
            OrdinaryDFS(ordinary_graph, vertex, order, used);
        }
    }

    used.assign(ordinary_graph.Size() + 1, false);
    
    std::reverse(order.begin(), order.end());
    for (int64_t index = 0; index < transposed_graph.Size(); ++index) {
        Vertex new_vertex = order[index];
        if (!used[new_vertex]) {
            TransposedDFS(transposed_graph, new_vertex, component, used);
            for (Vertex vertex : component) {
                belongs_to[vertex] = component_number;
            }
            component.clear();
            ++component_number;
        }
    }

    return belongs_to;
}

void OrdinaryDFS(Graph& ordinary_graph, Vertex vertex, std::vector<Vertex>& order,
                 std::vector<bool>& used) {
    used[vertex] = true;
    for (Vertex neighbour : ordinary_graph.Neighbours(vertex)) {
        if (!used[neighbour]) {
            OrdinaryDFS(ordinary_graph, neighbour, order, used);
        }
    }
    order.push_back(vertex);
}

void TransposedDFS(Graph& transposed_graph, Vertex vertex, std::vector<Vertex>& component,
                   std::vector<bool>& used) {
    used[vertex] = true;
    component.push_back(vertex);
    for (Vertex neighbour : transposed_graph.Neighbours(vertex)) {
        if (!used[neighbour]) {
            TransposedDFS(transposed_graph, neighbour, component, used);
        }
    }
}

void FillTransposedGraph(Graph& ordinary_graph, Graph& transposed_graph) {
    for (Vertex vertex = 1; vertex < ordinary_graph.Size() + 1; ++vertex) {
        for (Vertex neighbour : ordinary_graph.Neighbours(vertex)) {
            transposed_graph.InsertEdge(neighbour, vertex);
        }
    }
}

void PrintAnswer(int64_t component_number, std::vector<int64_t>& belongs_to) {
    std::cout << component_number - 1 << "\n";
    for (Vertex vertex = 1; vertex < belongs_to.size(); ++vertex) {
        std::cout << belongs_to[vertex] << " ";
    }
    std::cout << "\n";
}