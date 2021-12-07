#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

struct Cell {
    int32_t x;
    int32_t y;

    Cell operator+(const Cell& other) {
        return Cell{x + other.x, y + other.y};
    }
};

using Vertex = int32_t;

class Graph {
public:

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex) = 0;
    virtual std::vector<Vertex>& Neighbours(Vertex vertex) = 0;
    virtual int32_t Size() = 0;
};

class GraphList : public Graph{
private:

    int32_t vertex_number;
    std::vector<std::vector<Vertex>> list;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int32_t vertex_num);
    std::vector<Vertex>& Neighbours(Vertex vertex);
    void PrintAnswer(Graph& graph, bool path_exists);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex);
    int32_t Size();
};

Vertex ToVertex(Cell cell, int32_t width);

Cell ToCell(Vertex vertex, int32_t width);

std::deque<Vertex> FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex);

std::deque<Vertex> NormalizePath(std::vector<int32_t>& distance, std::vector<bool>& used,
                                 std::vector<Vertex>& parent, Vertex from_vertex, Vertex to_vertex);

void FillGraphWithEdges(Graph& graph, int32_t width);

void PrintAnswer(Graph& graph, std::deque<Vertex>& right_path, int32_t vertex_num);

bool BelongsToGrid(Cell cell, int32_t width);

int main() {
    Cell start_vertex = {0, 0};
    Cell finish_vertex = {0, 0};
    int32_t vertex_num = 0;
    std::cin >> vertex_num >> start_vertex.x >> start_vertex.y >> finish_vertex.x >> finish_vertex.y;
    --start_vertex.x;
    --start_vertex.y;
    --finish_vertex.x;
    --finish_vertex.y;

    GraphList graph(vertex_num);

    FillGraphWithEdges(graph, vertex_num);

    std::deque<Vertex> right_path = FindShortestPath(graph, ToVertex(start_vertex, vertex_num), ToVertex(finish_vertex, vertex_num));

    PrintAnswer(graph, right_path, vertex_num);
}

Vertex ToVertex(Cell cell, int32_t width) {
    return cell.x * width + cell.y;
}

Cell ToCell(Vertex vertex, int32_t width) {
    return Cell{vertex / width, vertex % width};
}

GraphList::GraphList(int32_t vertex_num) : vertex_number(vertex_num) {
    list.resize(vertex_num * vertex_num);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    list[from_vertex].push_back(to_vertex);
    list[to_vertex].push_back(from_vertex);
}

bool BelongsToGrid(Cell cell, int32_t width) {
    return (0 <= cell.x && cell.x < width &&
            0 <= cell.y && cell.y < width);
}

void FillGraphWithEdges(Graph& graph, int32_t width) {
    std::vector<Cell> moves(8, {0, 0});
    moves = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

    for (Vertex vertex = 0; vertex < graph.Size(); ++vertex) {
        Cell cell = ToCell(vertex, width);
        for (Cell& move : moves) {
            Cell next_cell = ToCell(vertex, width) + move;
            if (BelongsToGrid(next_cell, width)) {
                graph.InsertEdge(vertex, ToVertex(next_cell, width));
            }
        }
    }
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return list[vertex];
}

int32_t GraphList::Size() {
    return vertex_number * vertex_number;
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
        for (Vertex neighbour : graph.Neighbours(new_vertex)) {
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

void PrintAnswer(Graph& graph, std::deque<Vertex>& right_path, int32_t vertex_num) {
    if (!right_path.size()) {
        std::cout << "-1\n";
    } else {
        std::cout << right_path.size() - 1 << "\n";
        for (Vertex vertex : right_path) {
            Cell cell = ToCell(vertex, vertex_num);
            std::cout << cell.x + 1 << " " << cell.y + 1 << "\n";
        }
    }
}