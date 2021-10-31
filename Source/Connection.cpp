#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <set>

enum COLORS { WHITE = 0, GREY = 1, BLACK = 2 };

template <class TElem>
TElem Min(TElem first, TElem second) {
    return first < second ? first : second;
}

class Graph {
private:

    struct Vertex {
        int64_t to;
        int64_t edgeId;
    };

    int64_t idCount;
    int64_t vertex_number;
    std::vector<std::vector<Vertex>> graph;

public:

    Graph();
    ~Graph() = default;
    Graph(int64_t vertex_num);
    void Insert(std::pair<int64_t, int64_t>& pair);
    void BridgesDFS(int64_t vertex, int64_t edgeFrom, int64_t& timer,
                                std::set<std::pair<int64_t, int64_t>>& bridges,
                                std::vector<int64_t>& timeIn,
                                std::vector<int64_t>& timeUp,
                                std::vector<bool>& used);

    void FindComponentsDFS(int64_t vertex, int64_t currentColor, int64_t& maxColor,
                           std::vector<int64_t>& color,
                           std::vector<int64_t>& timeIn,
                           std::vector<int64_t>& timeUp);
    void Solve(std::set<std::pair<int64_t, int64_t>>& bridges,
               std::vector<int64_t>& color);
    void PrintGraph();
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

    int64_t timer = 0ll;
    std::set<std::pair<int64_t, int64_t>> bridges;
    std::vector<int64_t> timeIn(vertex_num + 1ll, 0ll);
    std::vector<int64_t> timeUp(vertex_num + 1ll, 0ll);
    std::vector<bool> used(vertex_num + 1ll, false);

    graph.BridgesDFS(1ll, -1ll, timer, bridges, timeIn, timeUp, used);

    int64_t maxColor = 0ll;
    std::vector<int64_t> color(vertex_num + 1ll, maxColor);

    for (int64_t vertex = 1ll; vertex < vertex_num + 1ll; ++vertex) {
        if (color[vertex] == 0ll) {
            ++maxColor;
            graph.FindComponentsDFS(vertex, maxColor, maxColor, color, timeIn, timeUp);
        }
    }

    graph.Solve(bridges, color);

    return 0;
}

Graph::Graph(int64_t vertex_num) : vertex_number(vertex_num), idCount(0ll) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t>& pair) {
    ++idCount;
    graph[pair.first].push_back(Vertex{pair.second, idCount});
    graph[pair.second].push_back(Vertex{pair.first, idCount});
}

void Graph::BridgesDFS(int64_t vertex, int64_t edgeFrom, int64_t& timer,
                       std::set<std::pair<int64_t, int64_t>>& bridges,
                       std::vector<int64_t>& timeIn,
                       std::vector<int64_t>& timeUp,
                       std::vector<bool>& used) {
    used[vertex] = true;
    timeUp[vertex] = timeIn[vertex] = timer++;
    for (auto neighbour : graph[vertex]) {
        if (neighbour.edgeId == edgeFrom) {
            continue;
        }
        if (used[neighbour.to]) {
            timeUp[vertex] = Min(timeUp[vertex], timeIn[neighbour.to]);
        } else if (!used[neighbour.to]) {
            BridgesDFS(neighbour.to, neighbour.edgeId, timer, bridges, timeIn, timeUp, used);
            timeUp[vertex] = Min(timeUp[vertex], timeUp[neighbour.to]);
            if (timeUp[neighbour.to] > timeIn[vertex]) {
                bridges.insert(std::make_pair(vertex, neighbour.to));
            }
        }
    }
}

void Graph::FindComponentsDFS(int64_t vertex, int64_t currentColor, int64_t& maxColor,
                              std::vector<int64_t>& color,
                              std::vector<int64_t>& timeIn,
                              std::vector<int64_t>& timeUp) {
    color[vertex] = currentColor;
    for (auto neighbour : graph[vertex]) {
        if (color[neighbour.to] == 0ll) {
            if (timeUp[neighbour.to] > timeIn[vertex]) {
                ++maxColor;
                FindComponentsDFS(neighbour.to, maxColor, maxColor, color, timeIn, timeUp);
            } else {
                FindComponentsDFS(neighbour.to, currentColor, maxColor, color, timeIn, timeUp);
            }
        }
    }
}

void Graph::Solve(std::set<std::pair<int64_t, int64_t>>& bridges,
                  std::vector<int64_t>& color) {
    std::vector<int64_t> count(vertex_number + 1ll, 0ll);
    for (auto bridge : bridges) {
        ++count[color[bridge.first]];
        ++count[color[bridge.second]];
    }
    int64_t leavesNum = 0ll;
    for (int64_t comp = 1ll; comp < vertex_number + 1ll; ++comp) {
        if (count[comp] == 1) {
            ++leavesNum;
        }
    }   
    std::cout << (leavesNum + 1ll) / 2ll << std::endl;
}

void Graph::PrintGraph() {
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (auto neighbour : graph[i]) {
            std::cout << neighbour.to << " (edgeId = " << neighbour.edgeId << ") ";
        }
        std::cout << "\n";
    }
}