#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <cassert>

enum BOUNDS { MIN_NUMBER = 1111, MAX_NUMBER = 10000 };

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
    void PrintAnswer(Graph& graph, bool path_exists);
    int32_t Size();
};

std::deque<Vertex> FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex);

std::deque<Vertex> NormalizePath(std::vector<int32_t>& distance, std::vector<bool>& used,
                                 std::vector<Vertex>& parent, Vertex from_vertex, Vertex to_vertex);

void PrintAnswer(std::deque<Vertex>& right_path);

void FillGraphWithEdges(Graph& graph);

Vertex FirstDigit(Vertex number);

Vertex SecondDigit(Vertex number);

Vertex ThirdDigit(Vertex number);

Vertex LastDigit(Vertex number);

Vertex IncreaseFirstDigit(Vertex number);

Vertex DecreaseLastDigit(Vertex number);

Vertex LeftCycleShifted(Vertex number);

Vertex RightCycleShifted(Vertex number);

int main() {
    Vertex start_vertex = 0;
    Vertex finish_vertex = 0;
    std::cin >> start_vertex >> finish_vertex;

    GraphList graph(MAX_NUMBER);

    FillGraphWithEdges(graph);

    std::deque<Vertex> right_path = FindShortestPath(graph, start_vertex, finish_vertex);

    PrintAnswer(right_path);
}

Vertex FirstDigit(Vertex number) {
    return number / 1000;
}

Vertex SecondDigit(Vertex number) {
    return (number / 100) % 10;
}

Vertex ThirdDigit(Vertex number) {
    return (number / 10) % 10;
}

Vertex LastDigit(Vertex number) {
    return number % 10;
}

Vertex IncreaseFirstDigit(Vertex number) {
    return number + 1000;
}

Vertex DecreaseLastDigit(Vertex number) {
    return number - 1;
}

Vertex LeftCycleShifted(Vertex number) {
    Vertex part = number / 1000;
    Vertex temp_number = (number % 1000)  * 10 + part;
    return temp_number;
}

Vertex RightCycleShifted(Vertex number) {
    Vertex part = number % 10;
    Vertex temp_number = (number / 10) + part * 1000;
    return temp_number;
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

void FillGraphWithEdges(Graph& graph) {
    for (Vertex number = MIN_NUMBER; number < MAX_NUMBER; ++number) {
        if (FirstDigit(number) != 0 && SecondDigit(number) != 0 &&
            ThirdDigit(number) != 0 && LastDigit(number) != 0) {

            if (FirstDigit(number) != 9) {
                graph.InsertEdge(number, IncreaseFirstDigit(number));
            }
            if (LastDigit(number) != 1) {
                graph.InsertEdge(number, DecreaseLastDigit(number));
            }
            graph.InsertEdge(number, LeftCycleShifted(number));
            graph.InsertEdge(number, RightCycleShifted(number));
        }
    }
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
    assert(right_path.size() > 1);
    if (right_path.size()) {
        std::cout << right_path.size() << std::endl;
        for (Vertex vertex : right_path) {
            std::cout << vertex % 10000 << "\n";
        }
    }
}