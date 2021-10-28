#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

enum COLORS { WHITE = 0, GREY = 1, BLACK = 2 };

class Graph {
private:

    struct Vertex {
        std::vector<int64_t> list;
    };

    int64_t vertex_number;
    std::vector<Vertex> graph;

public:

    Graph() = default;
    ~Graph() = default;
    Graph(int64_t vertex_num);
    void Insert(std::pair<int64_t, int64_t> pair);
    void PrintGraph();
    void OrdinaryDFS(int64_t vertex, std::vector<int64_t>& order,
                  std::vector<bool>& used);
    void TransposedDFS(int64_t vertex, std::vector<int64_t>& component,
                   std::vector<bool>& used);
    void FindComponents(Graph& transposed);
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t edges_num = 0ll;
    std::cin >> vertex_num;
    edges_num = vertex_num;

    Graph ordinary_graph(vertex_num);
    Graph not_transposed_graph(vertex_num);

    int64_t from = 0ll;
    for (int64_t to = 1ll; to < edges_num + 1ll; ++to) {
        std::cin >> from;
        ordinary_graph.Insert(std::make_pair(from, to));
        not_transposed_graph.Insert(std::make_pair(from, to));
    }

    // ordinary_graph.PrintGraph();
    // not_transposed_graph.PrintGraph();
    ordinary_graph.FindComponents(not_transposed_graph);

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t> pair) {
    if (!graph[pair.first].list.size()) {
        graph[pair.first].list.push_back(pair.second);
    } else {
        auto result = std::find(graph[pair.first].list.begin(), graph[pair.first].list.end(), pair.second);
        if (result == graph[pair.first].list.end()) {
            graph[pair.first].list.push_back(pair.second);
        }
    }
}

void Graph::OrdinaryDFS(int64_t vertex, std::vector<int64_t>& order,
                     std::vector<bool>& used) {
    used[vertex] = true;
    for (int64_t neighbour : graph[vertex].list) {
        if (!used[neighbour]) {
            OrdinaryDFS(neighbour, order, used);
        }
    }
    order.push_back(vertex);
}

void Graph::TransposedDFS(int64_t vertex, std::vector<int64_t>& component,
                      std::vector<bool>& used) {
    used[vertex] = true;
    component.push_back(vertex);
    for (int64_t neighbour : graph[vertex].list) {
        if (!used[neighbour]) {
            TransposedDFS(neighbour, component, used);
        }
    }
}

void Graph::FindComponents(Graph& transposed) {
    std::vector<int64_t> belongs_to(vertex_number + 1ll, 0ll);
    std::vector<bool> used(vertex_number + 1ll, false);
    std::vector<int64_t> order;
    std::vector<int64_t> component;

    for (int64_t vertex = 1ll; vertex < vertex_number + 1ll; ++vertex) {
        if (!used[vertex]) {
            OrdinaryDFS(vertex, order, used);
        }
    }
    used.assign(vertex_number + 1ll, false);
    int64_t component_number = 1ll;
    for (int64_t index = 0ll; index < vertex_number; ++index) {
        int64_t new_vertex = order[vertex_number - 1ll - index];
        if (!used[new_vertex]) {
            transposed.TransposedDFS(new_vertex, component, used);
            for (int64_t vertex : component) {
                belongs_to[vertex] = component_number;
            }
            component.clear();
            ++component_number;
        }
    }
    std::cout << component_number - 1ll << std::endl;
}

void Graph::PrintGraph() {
    std::cout << "------------Print----graph---------------\n";
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (int64_t neighbour : graph[i].list) {
            std::cout << neighbour << " ";
        }
        std::cout << "\n";
    }
    std::cout << "-----------------------------------------\n";
}