#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

enum COLOR { WHITE = 0, GREY = 1, BLACK = 2 };

using Vertex = int32_t;

class Graph {
public:

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex) = 0;
    virtual std::vector<Vertex>& Neighbours(Vertex vertex) = 0;
    virtual int32_t Size() = 0;
};

class GraphList : public Graph {
private:

    int32_t vertex_number;
    std::vector<std::vector<Vertex>> list;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int32_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex);
    std::vector<Vertex>& Neighbours(Vertex vertex);
    int32_t Size();
};

std::vector<Vertex> FindCycle(Graph& graph);

bool HasCycleDFS(Graph& graph, Vertex vertex, Vertex& cycleBeg, Vertex& cycleEnd,
                 std::vector<Vertex>& parent, std::vector<char>& color);

std::vector<Vertex> NormalizeCycle(std::vector<Vertex>& parent,
                                   Vertex& cycleBeg, Vertex& cycleEnd);

void PrintAnswer(std::vector<Vertex>& cycle);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int32_t vertex_num = 0;
    int32_t edges_num = 0;
    Vertex from_vertex = 0;
    Vertex to_vertex = 0;
    std::cin >> vertex_num >> edges_num;

    GraphList graph(vertex_num);
    for (int32_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex;
        graph.InsertEdge(--from_vertex, --to_vertex);
    }

    std::vector<Vertex> cycle = FindCycle(graph);

    PrintAnswer(cycle);

    return 0;
}

GraphList::GraphList(int32_t vertex_num) : vertex_number(vertex_num) {
    list.resize(vertex_num + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    list[from_vertex].push_back(to_vertex);
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return list[vertex];
}

int32_t GraphList::Size() {
    return vertex_number;
}

std::vector<Vertex> FindCycle(Graph& graph) {
    Vertex cycleBeg = -1;
    Vertex cycleEnd = -1;
    std::vector<Vertex> parent(graph.Size() + 1, -1);
    std::vector<char> color(graph.Size() + 1, WHITE);

    for (Vertex vertex = 0; vertex < graph.Size(); ++vertex) {
        if (HasCycleDFS(graph, vertex, cycleBeg, cycleEnd, parent, color)) {
            break;
        }
    }

    return NormalizeCycle(parent, cycleBeg, cycleEnd);
}

bool HasCycleDFS(Graph& graph, Vertex vertex, Vertex& cycleBeg, Vertex& cycleEnd,
                 std::vector<Vertex>& parent, std::vector<char>& color) {
    color[vertex] = GREY;
    for (Vertex neighbour : graph.Neighbours(vertex)) {
        if (color[neighbour] == WHITE) {
            parent[neighbour] = vertex;
            if (HasCycleDFS(graph, neighbour, cycleBeg, cycleEnd, parent, color)) {
                return true;
            }
        } else if (color[neighbour] == GREY) {
            cycleBeg = neighbour;
            cycleEnd = vertex;
            return true;
        }
    }
    color[vertex] = BLACK;
    return false;
}

std::vector<Vertex> NormalizeCycle(std::vector<Vertex>& parent,
                                   Vertex& cycleBeg, Vertex& cycleEnd) {
    std::vector<Vertex> cycle;

    if (cycleBeg != -1) {
        for (Vertex vertex = cycleEnd; vertex != cycleBeg; vertex = parent[vertex]) {
            cycle.push_back(vertex);
        }

        cycle.push_back(cycleBeg);
        std::reverse(cycle.begin(), cycle.end());
    }

    return cycle;
}

void PrintAnswer(std::vector<Vertex>& cycle) {
    if (!cycle.size()) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
        for (Vertex vertex : cycle) {
            ++vertex;
            std::cout << vertex << " ";
        }
        std::cout << "\n";
    }
}