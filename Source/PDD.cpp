#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t MAX_WEIGHT = 3;

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
        int64_t to;
        int64_t weight;
    };

    int64_t vertex_number;
    int64_t edges_number;
    std::vector<std::vector<Vertex>> graph;

public:

    Graph() = default;
    ~Graph() = default;
    Graph(int64_t vertex_num);
    void Insert(std::pair<int64_t, int64_t> pair);
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
    int64_t request_num = 0ll;
    std::cin >> vertex_num >> edges_num;

    int64_t first = 0ll;
    int64_t second = 0ll;
    Graph graph(vertex_num);
    for (int64_t i = 0ll; i < edges_num; ++i) {
        std::cin >> first >> second;
        graph.Insert(std::make_pair(first, second));
    }

    graph.PrintGraph();

    std::cin >> request_num;
    for (int64_t i = 0ll; i < request_num; ++i) {
        std::cin >> start_vertex >> finish_vertex;
        graph.FindShortestPath(start_vertex, finish_vertex);
    }
}

Graph::Graph(int64_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_num + 1ll);
}

void Graph::Insert(std::pair<int64_t, int64_t> pair) {
    graph[pair.first].push_back(Vertex{pair.second, 0ll});
    graph[pair.second].push_back(Vertex{pair.first, 1ll});
}

void Graph::FindShortestPath(int64_t from_vertex, int64_t to_vertex) {
    std::vector<int64_t> distance(vertex_number + 1ll, LONG_MAX);
    std::vector<std::queue<int64_t>> queue(MAX_WEIGHT);
    distance[from_vertex] = 0ll;
    queue[0].push(from_vertex);

    for (int64_t i = 0ll; i <= (MAX_WEIGHT - 1ll) * vertex_number; ++i) {
        while (!queue[i % MAX_WEIGHT].empty()) {
            int64_t new_vertex = queue[i % MAX_WEIGHT].front();
            queue[i % MAX_WEIGHT].pop();

            for (auto neighbour : graph[new_vertex]) {
                int64_t to = neighbour.to;
                int64_t weight = neighbour.weight;

                if (distance[new_vertex] + weight < distance[to]) {
                    distance[to] = i + weight;
                    queue[(i + weight) % MAX_WEIGHT].push(to);
                }
            }
        }
    }
    if (distance[to_vertex] == LONG_MAX) {
        std::cout << "-1" << std::endl;
    } else {
        std::cout << distance[to_vertex] << std::endl;
    }
}

void Graph::PrintGraph() {
    for (int64_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (auto neighbour : graph[i]) {
            std::cout << neighbour.to << " " << "(" << neighbour.weight << ")" << " ";
        }
        std::cout << "\n";
    }
}