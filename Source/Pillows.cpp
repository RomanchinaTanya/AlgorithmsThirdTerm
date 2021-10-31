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

    int64_t vertex_number;
    int64_t real_vertex_number;
    std::vector<std::vector<int64_t>> graph;

public:

    Graph() = default;
    ~Graph() = default;
    Graph(int64_t vertex_num, int64_t real_vertex_num);
    void Insert(std::pair<int64_t, int64_t> pair);
    void ArticulattionPointsDFS(int64_t vertex, int64_t parent, int64_t& timer,
                                std::set<int64_t>& articulationPoints,
                                std::vector<int64_t>& timeIn,
                                std::vector<int64_t>& timeUp,
                                std::vector<bool>& used);
    void PrintGraph();
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0ll;
    int64_t pillows_num = 0ll;
    int64_t max_vertex = 0ll;
    int64_t first = 0ll;
    int64_t second = 0ll;
    int64_t third = 0ll;
    std::cin >> vertex_num >> pillows_num;
    max_vertex = vertex_num;

    Graph graph(vertex_num + pillows_num, vertex_num);

    for (int64_t i = 0ll; i < pillows_num; ++i) {
        std::cin >> first >> second >> third;
        ++max_vertex;
        graph.Insert(std::make_pair(max_vertex, first));
        graph.Insert(std::make_pair(max_vertex, second));
        graph.Insert(std::make_pair(max_vertex, third));
    }

    int64_t timer = 0ll;
    std::set<int64_t> articulationPoints;
    std::vector<int64_t> timeIn(max_vertex + 1ll, 0ll);
    std::vector<int64_t> timeUp(max_vertex + 1ll, 0ll);
    std::vector<bool> used(max_vertex + 1ll, false);

    // graph.PrintGraph();
    graph.ArticulattionPointsDFS(1ll, -1ll, timer, articulationPoints,
                                 timeIn, timeUp, used);

    std::cout << articulationPoints.size() << std::endl;
    for (int64_t elem : articulationPoints) {
        std::cout << elem - vertex_num << std::endl;
    }

    return 0;
}

Graph::Graph(int64_t vertex_num, int64_t real_vertex_num) : vertex_number(vertex_num), real_vertex_number(real_vertex_num) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t> pair) {
    graph[pair.first].push_back(pair.second);
    graph[pair.second].push_back(pair.first);
}

void Graph::ArticulattionPointsDFS(int64_t vertex, int64_t parent, int64_t& timer,
                                   std::set<int64_t>& articulationPoints,
                                   std::vector<int64_t>& timeIn,
                                   std::vector<int64_t>& timeUp,
                                   std::vector<bool>& used) {
    used[vertex] = true;
    timeUp[vertex] = timeIn[vertex] = timer++;
    int64_t childrenNum = 0ll;
    for (int64_t neighbour : graph[vertex]) {
        if (neighbour == parent) {
            continue;
        }
        if (used[neighbour]) {
            timeUp[vertex] = Min(timeUp[vertex], timeIn[neighbour]);
        } else if (!used[neighbour]) {
            ++childrenNum;
            ArticulattionPointsDFS(neighbour, vertex, timer, articulationPoints, timeIn, timeUp, used);
            timeUp[vertex] = Min(timeUp[vertex], timeUp[neighbour]);
            if ((timeUp[neighbour] >= timeIn[vertex]) && (parent != -1ll)) {
                if (vertex > real_vertex_number) {
                    articulationPoints.insert(vertex);   
                }
                // articulationPoints.insert(vertex);
            }
        }
    }
    if ((parent == -1ll) && (childrenNum > 1)) {
        if (vertex > real_vertex_number) {
            articulationPoints.insert(vertex);
        }
    }
}

void Graph::PrintGraph() {
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (int64_t neighbour : graph[i]) {
            std::cout << neighbour << " ";
        }
        std::cout << "\n";
    }
}