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
    virtual int32_t Size() = 0;
};

class GraphList : public Graph {
private:

    int32_t vertex_number;
    std::vector<std::vector<Vertex>> graph_;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int32_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex);
    std::vector<Vertex>& Neighbours(Vertex vertex);
    int32_t Size();
};

std::deque<Vertex> FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex);

std::deque<Vertex> NormalizePath(std::vector<int32_t>& distance, std::vector<bool>& used,
                                 std::vector<Vertex>& parent, Vertex from_vertex, Vertex to_vertex);

void PrintAnswer(std::deque<Vertex>& right_path);

int main() {
    int32_t vertex_num = 0;
    int32_t edges_num = 0;
    Vertex start_vertex = 0;
    Vertex finish_vertex = 0;
    Vertex from_vertex = 0;
    Vertex to_vertex = 0;
    std::cin >> vertex_num >> edges_num >> start_vertex >> finish_vertex;

    GraphList graph(vertex_num);

    for (int32_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex;
        graph.InsertEdge(from_vertex, to_vertex);
    }

    std::deque<Vertex> right_path = FindShortestPath(graph, start_vertex, finish_vertex);

    PrintAnswer(right_path);
}

GraphList::GraphList(int32_t vertex_num) : vertex_number(vertex_num) {
    graph_.resize(vertex_num + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    graph_[from_vertex].push_back(to_vertex);
    graph_[to_vertex].push_back(from_vertex);
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return graph_[vertex];
}

int32_t GraphList::Size() {
    return vertex_number;
}

std::deque<int32_t> FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex) {
    std::vector<int32_t> distance(graph.Size() + 1, 0);
    std::vector<Vertex> parent(graph.Size() + 1, 0);
    std::vector<bool> used(graph.Size() + 1, false);
    
    std::queue<Vertex> queue;
    queue.push(from_vertex);

    used[from_vertex] = true;
    parent[from_vertex] = -1;

    while (!queue.empty()) {
        Vertex new_vertex = queue.front();
        queue.pop();
        for (Vertex& neighbour : graph.Neighbours(new_vertex)) {
            if (!used[neighbour]) {
                used[neighbour] = true;
                queue.push(neighbour);
                distance[neighbour] = distance[new_vertex] + 1;
                parent[neighbour] = new_vertex;
            }
        }
    }

    return NormalizePath(distance, used, parent, from_vertex, to_vertex);
}

std::deque<Vertex> NormalizePath(std::vector<int32_t>& distance, std::vector<bool>& used,
                                 std::vector<Vertex>& parent, Vertex from_vertex, Vertex to_vertex) {
    std::deque<Vertex> right_path;
    if (used[to_vertex]) {
        while (to_vertex != from_vertex) {
            right_path.push_front(to_vertex);
            to_vertex = parent[to_vertex];
        }
        right_path.push_front(from_vertex);
    }
    return right_path;
}

void PrintAnswer(std::deque<Vertex>& right_path) {
    if (!right_path.size()) {
        std::cout << "-1\n";
    } else {
        std::cout << right_path.size() - 1 << "\n";
        for (Vertex vertex : right_path) {
            std::cout << vertex << " ";
        }
        std::cout << "\n";
    }
}