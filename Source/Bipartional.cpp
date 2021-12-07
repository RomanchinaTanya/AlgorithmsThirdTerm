#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

enum COLOR { NONE = -1, WHITE = 0, BLACK = 1 };

COLOR AnotherColor(int32_t color) {
    if (color == WHITE) {
        return BLACK;
    }
    return WHITE;
}

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
    std::vector<std::vector<Vertex>> graph;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int32_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex);
    std::vector<Vertex>& Neighbours(Vertex vertex);
    void PrintGraph();
    int32_t Size();
};

bool IsRightColored(Graph& graph, Vertex from_vertex);

bool IsBipartional(Graph& graph);

int main() {
    int32_t vertex_num = 0;
    int32_t edges_num = 0;
    Vertex from_vertex = 0;
    Vertex to_vertex = 0;
    std::cin >> vertex_num >> edges_num;

    GraphList graph(vertex_num);
    for (int32_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex;
        graph.InsertEdge(from_vertex, to_vertex);
    }

    if (IsBipartional(graph)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}

GraphList::GraphList(int32_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_number + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    graph[from_vertex].push_back(to_vertex);
    graph[to_vertex].push_back(from_vertex);
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return graph[vertex];
}

int32_t GraphList::Size() {
    return vertex_number;
}

bool IsRightColored(Graph& graph, Vertex from_vertex) {
    std::vector<int32_t> color(graph.Size() + 1, NONE);
    color[from_vertex] = BLACK;
    std::queue<Vertex> queue;
    queue.push(from_vertex);

    while (!queue.empty()) {
        Vertex new_vertex = queue.front();
        queue.pop();
        for (int32_t to : graph.Neighbours(new_vertex)) {
            if (color[to] == NONE) {
                color[to] = AnotherColor(color[new_vertex]);
                queue.push(to);
            } else if (color[to] == color[new_vertex]) {
                return false;
            }
        }
    }
    return true;
}

bool IsBipartional(Graph& graph) {
    for (Vertex i = 1; i < graph.Size() + 1; ++i) {
        if (IsRightColored(graph, i)) {
            continue;
        }
        return false;
    }
    return true;
}