#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

template <class TElem>
TElem Min(TElem first, TElem second) {
    return first < second ? first : second;
}

bool Comparator(std::pair<int64_t, int64_t> first, std::pair<int64_t, int64_t> second) {
    return (first.second <= second.second);
}

class Graph {
private:

    struct Vertex {
        std::vector<std::pair<int64_t, int64_t>> list;
    };

    int64_t vertex_number;
    int64_t edges_number;
    std::vector<Vertex> graph;

public:

    Graph() = default;
    ~Graph() = default;
    Graph(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>>& edges, int64_t vertex_num);
    void FindShortestPath(int64_t from_vertex, int64_t to_vertex);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int64_t vertex_num = 0ll;
    int64_t edges_num = 0ll;
    int64_t start_vertex = 0ll;
    int64_t finish_vertex = 0ll;
    std::cin >> vertex_num >> edges_num >> start_vertex >> finish_vertex;

    std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> edges;
    edges.reserve(edges_num);
    edges.resize(edges_num);
    for (int64_t i = 0ll; i < edges_num; ++i) {
        std::cin >> edges[i].first >> edges[i].second.first >> edges[i].second.second;
    }

    Graph graph(edges, vertex_num);
    // graph.PrintGraph();
    graph.FindShortestPath(start_vertex, finish_vertex);
}

Graph::Graph(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>>& edges, int64_t vertex_num) : vertex_number(vertex_num), edges_number(edges.size()) {
    graph.reserve(vertex_num + 1ll);
    graph.resize(vertex_num + 1ll);
    int64_t size = static_cast<int64_t>(edges.size());
    for (int64_t i = 0ll; i < size; ++i) {
        auto result = std::find(graph[edges[i].first].list.begin(), graph[edges[i].first].list.end(), edges[i].second);
        if (result == graph[edges[i].first].list.end()) {
            graph[edges[i].first].list.push_back(edges[i].second);
        }
    }
    // for (Vertex& vertex : graph) {
    //     std::sort(vertex.list.begin(), vertex.list.end(), Comparator);
    // }
}

void Graph::FindShortestPath(int64_t from_vertex, int64_t to_vertex) {



    std::vector<int64_t> distance(vertex_number + 1ll, INT_MAX);

    std::vector<int64_t> parent(vertex_number + 1ll, -1ll);

    std::vector<bool> used(vertex_number + 1ll, false); 
    
    distance[from_vertex] = 0ll;

    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        int64_t vert = -1ll;
        for (int64_t j = 1ll; j < vertex_number + 1ll; ++j) {
            if (!used[j] && (vert == -1 || distance[j] < distance[vert])) {
                vert = j;
            }
        }
        if (distance[vert] == INT_MAX) {
            break;
        }
        used[vert] = true;

        for (auto neighbour : graph[vert].list) {
            int64_t to = neighbour.first;
            int64_t dist = neighbour.second;
            if (distance[vert] + dist < distance[to]) {
                distance[to] = distance[vert] + dist;
                parent[to] = vert;
            }
        }
    }
    if (distance[to_vertex] != INT_MAX) {
        std::cout << distance[to_vertex] << std::endl;
    } else {
        std::cout << "-1" << std::endl;
    }
}

void Graph::PrintGraph() {
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (auto neighbour : graph[i].list) {
            std::cout << neighbour.first << " " << "(" << neighbour.second << ")" << " ";
        }
        std::cout << "\n";
    }
}