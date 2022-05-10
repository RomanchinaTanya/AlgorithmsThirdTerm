#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <set>

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

std::set<Vertex> FindArticulationPoints(Graph& graph);

void ArticulationPointsDFS(Graph& graph, Vertex vertex,
                           Vertex parent, int64_t& timer,
                           std::set<Vertex>& articulation_points,
                           std::vector<int64_t>& time_in,
                           std::vector<int64_t>& time_up,
                           std::vector<bool>& used);

void PrintAnswer(std::set<Vertex>& articulation_points);

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

    std::set<Vertex> articulation_points = FindArticulationPoints(graph);

    PrintAnswer(articulation_points);

    return 0;
}

GraphList::GraphList(int64_t vertex_num) : vertex_number(vertex_num) {
    list.resize(vertex_num + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex) {
    list[from_vertex].push_back(to_vertex);
    list[to_vertex].push_back(from_vertex);
}

std::vector<Vertex>& GraphList::Neighbours(Vertex vertex) {
    return list[vertex];
}

int64_t GraphList::Size() {
    return vertex_number;
}

std::set<Vertex> FindArticulationPoints(Graph& graph) {
    int64_t timer = 0;
    std::set<Vertex> articulation_points;
    std::vector<int64_t> time_in(graph.Size() + 1, 0);
    std::vector<int64_t> time_up(graph.Size() + 1, 0);
    std::vector<bool> used(graph.Size() + 1, false);

    for (Vertex vertex = 1; vertex < graph.Size() + 1; ++vertex) {
        if (!used[vertex]) {
            ArticulationPointsDFS(graph, vertex, -1, timer,
                                  articulation_points,
                                  time_in, time_up, used);
        }
    }

    return articulation_points;
}

void ArticulationPointsDFS(Graph& graph, Vertex vertex,
                           Vertex parent, int64_t& timer,
                           std::set<Vertex>& articulation_points,
                           std::vector<int64_t>& time_in,
                           std::vector<int64_t>& time_up,
                           std::vector<bool>& used) {
    used[vertex] = true;
    time_up[vertex] = time_in[vertex] = timer++;
    int64_t children_number = 0;
    for (Vertex neighbour : graph.Neighbours(vertex)) {
        if (neighbour == parent) {
            continue;
        }
        if (used[neighbour]) {
            time_up[vertex] = std::min(time_up[vertex], time_in[neighbour]);
        } else if (!used[neighbour]) {
            ++children_number;
            ArticulationPointsDFS(graph, neighbour, vertex, timer,
                                   articulation_points, time_in,
                                   time_up, used);
            time_up[vertex] = std::min(time_up[vertex], time_up[neighbour]);
            if ((time_up[neighbour] >= time_in[vertex]) && (parent != -1)) {
                articulation_points.insert(vertex);
            }
        }
    }
    if ((parent == -1) && (children_number > 1)) {
        articulation_points.insert(vertex);
    }
}

void PrintAnswer(std::set<Vertex>& articulation_points) {
    std::cout << articulation_points.size() << "\n";
    for (Vertex vertex : articulation_points) {
        std::cout << vertex << "\n";
    }
}