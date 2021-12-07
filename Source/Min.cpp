#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t MAX_WEIGHT = 11;

struct Neighbour;

using Vertex = int32_t;

using Edge = Neighbour;

struct Neighbour {
    Vertex to;
    int64_t weight;
};

class Graph {
public:

    virtual void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) = 0;
    virtual std::vector<Edge>& Neighbours(Vertex vertex) = 0;
    virtual int64_t Size() = 0;
};

class GraphList : public Graph {
private:

    int64_t vertex_number;
    std::vector<std::vector<Edge>> list;

public:

    GraphList() = default;
    ~GraphList() = default;
    explicit GraphList(int64_t vertex_num);
    void InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight);
    std::vector<Edge>& Neighbours(Vertex vertex);
    int64_t Size();
};

int64_t FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0;
    int64_t edges_num = 0;
    Vertex start_vertex = 0;
    Vertex finish_vertex = 0;
    Vertex from_vertex = 0;
    Vertex to_vertex = 0 ;
    int64_t weight = 0;
    std::cin >> vertex_num >> edges_num >> start_vertex >> finish_vertex;

    GraphList graph(vertex_num);
    for (int64_t i = 0; i < edges_num; ++i) {
        std::cin >> from_vertex >> to_vertex >> weight;
        graph.InsertEdge(from_vertex, to_vertex, weight);
    }

    int64_t distance = FindShortestPath(graph, start_vertex, finish_vertex);
    std::cout << distance << "\n";
}

GraphList::GraphList(int64_t vertex_num) : vertex_number(vertex_num) {
    list.resize(vertex_number + 1);
}

void GraphList::InsertEdge(Vertex from_vertex, Vertex to_vertex, int64_t weight) {
    list[from_vertex].push_back({to_vertex, weight});
}

std::vector<Edge>& GraphList::Neighbours(Vertex vertex) {
    return list[vertex];
}

int64_t GraphList::Size() {
    return vertex_number;
}

int64_t FindShortestPath(Graph& graph, Vertex from_vertex, Vertex to_vertex) {
    std::vector<int64_t> distance(graph.Size() + 1, LONG_MAX);
    std::vector<std::queue<Vertex>> queue(MAX_WEIGHT);

    distance[from_vertex] = 0; 
    queue[0].push(from_vertex);

    for (int64_t i = 0; i <= (MAX_WEIGHT - 1) * graph.Size(); ++i) {
        while (!queue[i % MAX_WEIGHT].empty()) {
            Vertex new_vertex = queue[i % MAX_WEIGHT].front();
            queue[i % MAX_WEIGHT].pop();

            for (auto& edge : graph.Neighbours(new_vertex)) {
                if (distance[new_vertex] + edge.weight < distance[edge.to]) {
                    distance[edge.to] = i + edge.weight;
                    queue[(i + edge.weight) % MAX_WEIGHT].push(edge.to);
                }
            }
        }
    }
    if (distance[to_vertex] == LONG_MAX) {
        return -1;
    }
    return distance[to_vertex];
}