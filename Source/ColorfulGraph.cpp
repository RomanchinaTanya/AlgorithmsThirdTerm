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
    bool HasCycle(int64_t vertex, std::vector<char>& color);
    void PrintAnswer();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    std::cin >> vertex_num;

    Graph graph(vertex_num);

    char color = '0';
    for (int64_t from = 1ll; from < vertex_num; ++from) {
        for (int64_t to  = from + 1ll; to < vertex_num + 1ll; ++to) {
            std::cin >> color;
            if (color == 'B') {
                graph.Insert(std::make_pair(from, to));
            } else {
                graph.Insert(std::make_pair(to, from));
            }
        }
    }

    graph.PrintAnswer();

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t> pair) {
    graph[pair.first].list.push_back(pair.second);
}

bool Graph::HasCycle(int64_t vertex, std::vector<char>& color) {
    color[vertex] = GREY;
    for (int64_t neighbour : graph[vertex].list) {
        if (color[neighbour] == WHITE) {
            if (HasCycle(neighbour, color)) {
                return true;
            }
        } else if (color[neighbour] == GREY) {
            return true;
        }
    }
    color[vertex] = BLACK;
    return false;
}

void Graph::PrintAnswer() {
    std::vector<char> color(vertex_number + 1ll, WHITE);

    int64_t result = false;
    for (int64_t vertex = 1ll; vertex < vertex_number + 1ll; ++vertex) {
        if (color[vertex] == WHITE) {
            if (HasCycle(vertex, color)) {
                result = true;
                break;
            }
        }
    }

    if (result) {
        std::cout << "NO" << std::endl;
    } else {
        std::cout << "YES" << std::endl;
    }
}