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
    void Insert(std::pair<int64_t, int64_t>& pair);
    bool HasCycle(int64_t vertex, int64_t& cycleBeg, int64_t& cycleEnd,
                  std::vector<int64_t>& parent, std::vector<char>& color);
    bool Cyclic();
    void PrintGraph();
    void TopSort();
    void TopSortDFS(int64_t vertex, std::deque<int64_t>& sorted,
                    std::vector<bool>& used);
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t edges_num = 0ll;
    std::cin >> vertex_num >> edges_num;

    Graph graph(vertex_num);
    std::pair<int64_t, int64_t> pair;
    for (int64_t i = 0ll; i < edges_num; ++i) {
        std::cin >> pair.first >> pair.second;
        graph.Insert(pair);
    }

    // graph.PrintGraph();
    graph.TopSort();

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t>& pair) {
    if (!graph[pair.first].list.size()) {
        graph[pair.first].list.push_back(pair.second);
    } else {
        auto result = std::find(graph[pair.first].list.begin(), graph[pair.first].list.end(), pair.second);
        if (result == graph[pair.first].list.end()) {
            graph[pair.first].list.push_back(pair.second);
        }
    }
}

void Graph::TopSort() {
    if (Cyclic()) {
        std::cout << "-1" << std::endl;
    } else {
        std::deque<int64_t> sorted;
        std::vector<bool> used(vertex_number + 1ll, false);
        sorted.clear();

        for (int64_t vertex = 1ll; vertex < vertex_number + 1ll; ++vertex) {
            if (!used[vertex]) {
                TopSortDFS(vertex, sorted, used);
            }
        }
        // std::reverse(sorted.begin(), sorted.end());
        for (int64_t vertex : sorted) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::TopSortDFS(int64_t vertex, std::deque<int64_t>& sorted,
                       std::vector<bool>& used) {
    used[vertex] = true;
    for (int64_t neighbour : graph[vertex].list) {
        if (!used[neighbour]) {
            TopSortDFS(neighbour, sorted, used);
        }
    }
    sorted.push_front(vertex);
}

bool Graph::HasCycle(int64_t vertex, int64_t& cycleBeg, int64_t& cycleEnd,
                     std::vector<int64_t>& parent, std::vector<char>& color) {
    color[vertex] = GREY;
    for (int64_t neighbour : graph[vertex].list) {
        if (color[neighbour] == WHITE) {
            parent[neighbour] = vertex;
            if (HasCycle(neighbour, cycleBeg, cycleEnd, parent, color)) {
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

bool Graph::Cyclic() {
    int64_t cycleBeg = -1ll;
    int64_t cycleEnd = -1ll;
    std::vector<int64_t> parent(vertex_number + 1ll, -1ll);
    std::vector<char> color(vertex_number + 1ll, WHITE);
    for (int64_t vertex = 1ll; vertex < vertex_number + 1ll; ++vertex) {
        if (HasCycle(vertex, cycleBeg, cycleEnd, parent, color)) {
            break;
        }
    }
    if (cycleBeg == -1ll) {
        return false;
    } else {
        return true;
    }
}

void Graph::PrintGraph() {
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (int64_t neighbour : graph[i].list) {
            std::cout << neighbour << " ";
        }
        std::cout << "\n";
    }
}