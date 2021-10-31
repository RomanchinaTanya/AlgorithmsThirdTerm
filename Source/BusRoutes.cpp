#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <map>
 
class Graph {
private:
 
    int32_t vertex_number;
    int32_t edges_number;
    std::vector<std::vector<int32_t>> graph;
    std::vector<int32_t> degIn;
    std::vector<int32_t> degOut;
    int32_t start_vertex = -1ll;

    std::map<std::pair<int32_t, int32_t>, int64_t> count;
 
public:
 
    Graph() = default;
    ~Graph() = default;
    Graph(int32_t vertex_num);
    void Insert(std::pair<int32_t, int32_t> pair);
    bool CheckEuler();
    void FindEulerCycle();
    void DFS(int32_t vertex, std::vector<bool>& used);
    void Solve();
    void PrintGraph();
};
 
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
 
    int32_t route_num = 0ll;
    int32_t vertex_num = 0ll;
    int32_t route_length = 0ll;
    int32_t first = 0ll;
    int32_t second = 0ll;
    std::vector<int32_t> degIn(vertex_num + 1ll, 0ll);
    std::vector<int32_t> degOut(vertex_num + 1ll, 0ll);
 
    std::cin >> route_num >> vertex_num;
    Graph graph(vertex_num);
    for (int32_t i = 0ll; i < route_num; ++i) {
        std::cin >> route_length;
        std::cin >> first;
        for (int32_t j = 0ll; j < route_length; ++j) {
            std::cin >> second;
            graph.Insert(std::make_pair(first, second));
            first = second;
        }
    }
 
    graph.Solve();
 
    return 0;
}
 
Graph::Graph(int32_t vertex_num) : vertex_number(vertex_num) {
    graph.resize(vertex_num + 1ll);
    degIn.resize(vertex_num + 1ll);
    degOut.resize(vertex_num + 1ll);
    degIn.assign(vertex_num + 1ll, 0ll);
    degOut.assign(vertex_num + 1ll, 0ll);
}
 
void Graph::Insert(std::pair<int32_t, int32_t> pair) {
    ++degIn[pair.second];
    ++degOut[pair.first];
    if ((std::find(graph[pair.first].begin(), graph[pair.first].end(), pair.second)) == graph[pair.first].end()) {
        graph[pair.first].push_back(pair.second);
        count[pair] = 1;
    } else {
        ++count[pair];
    }
}
 
bool Graph::CheckEuler() {
    std::vector<bool> used(vertex_number + 1ll, false);
    for (int32_t vertex = 1; vertex < vertex_number + 1; ++vertex) {
        if (graph[vertex].size() && start_vertex == -1) {
            start_vertex = vertex;
        }
        if (degIn[vertex] != degOut[vertex]) {
            return false;
        }
    }
    if (start_vertex == -1) {
        return false;
    }
    DFS(start_vertex, used);
    for (int32_t vertex = 1; vertex < vertex_number + 1; ++vertex) {
        if (graph[vertex].size() && !used[vertex]) {
            return false;
        }
    }
    return true;
}
 
void Graph::FindEulerCycle() {
    std::stack<int32_t> stack;
    std::vector<int32_t> euler_cycle;
    euler_cycle.reserve((vertex_number + 1ll) * 2ll);
    
    stack.push(start_vertex);
    while (stack.size()) {
        int32_t vertex = stack.top();
        bool found_edge = false;
        for (int32_t neighbour : graph[vertex]) {
            if (count[std::make_pair(vertex, neighbour)] > 0) {
                stack.push(neighbour);
                --count[std::make_pair(vertex, neighbour)];
                found_edge = true;
                break;
            }
        }
        if (!found_edge) {
            stack.pop();
            euler_cycle.push_back(vertex);
        }
    }
 
    std::cout << euler_cycle.size() << " ";
    for (int32_t vertex : euler_cycle) {
        std::cout << vertex << " ";
    }
}
 
void Graph::DFS(int32_t vertex, std::vector<bool>& used) {
    used[vertex] = true;
    for (auto neighbour : graph[vertex]) {
        if (!used[neighbour]) {
            DFS(neighbour, used);
        }
    }
}
 
void Graph::Solve() {

    if (CheckEuler()) {
        FindEulerCycle();
    } else {
        std::cout << "0";
    }
    std::cout << std::endl;
}
 
void Graph::PrintGraph() {
    for (int32_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << "Neighbours of " << i << ": ";
        for (auto neighbour : graph[i]) {
            std::cout << neighbour << " " ;
        }
        std::cout << "\n";
    }
 
    for (int32_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << degIn[i] << " ";
    } 
    std::cout << std::endl;
 
    for (int32_t i = 1ll; i < vertex_number + 1ll; ++i) {
        std::cout << degOut[i] << " ";
    } 
    std::cout << std::endl;
}
