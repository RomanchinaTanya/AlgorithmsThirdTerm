#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <map>

const int64_t kMax = 1000000001;

struct Edge {
    int64_t from;
    int64_t to;
    int64_t weight;
};

class DSU {
public:
    explicit DSU(int64_t max_size);
    int64_t MakeSet(int64_t x);
    int64_t FindSet(int64_t x);
    void UnioinSets(int64_t x, int64_t y);
    int64_t SetCount();

private:
    std::vector<int64_t> parent_;
    std::vector<int64_t> rang_;
    int64_t set_count_;
};

int64_t Boruvka(DSU& dsu, std::vector<Edge>& edges);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t vertex_num = 0;
    int64_t edges_num = 0;

    std::cin >> vertex_num >> edges_num;
    std::vector<Edge> edges(edges_num);
    for (int64_t i = 0; i < edges_num; ++i) {
        std::cin >> edges[i].from >> edges[i].to >> edges[i].weight;
        --edges[i].from;
        --edges[i].to;
    }
    DSU dsu(vertex_num);
    for (int64_t vertex = 0; vertex < vertex_num; ++vertex) {
        dsu.MakeSet(vertex);
    }

    int64_t total_weight = Boruvka(dsu, edges);

    std::cout << total_weight << "\n";

    return 0;
}

DSU::DSU(int64_t max_size) : set_count_(0) {
    parent_.resize(max_size, -1);
    rang_.resize(max_size, 0);
}

int64_t DSU::MakeSet(int64_t x) {
    ++set_count_;
    return parent_[x] = x;
}

int64_t DSU::FindSet(int64_t x) {
    if (x == parent_[x]) {
        return x;
    }
    return parent_[x] = FindSet(parent_[x]);
}

void DSU::UnioinSets(int64_t x, int64_t y) {
    x = FindSet(x);
    y = FindSet(y);
    if (x != y) {
        if (rang_[x] < rang_[y]) {
            parent_[x] = y;
        } else if (rang_[y] < rang_[x]) {
            parent_[y] = x;
        } else {
            parent_[x] = y;
            ++rang_[y];
        }
        --set_count_;
    }
}

int64_t DSU::SetCount() {
    return set_count_;
}

int64_t Boruvka(DSU& dsu, std::vector<Edge>& edges) {
    int64_t total_weight = 0;
    int64_t edges_num = edges.size();
    std::map<std::pair<int64_t, int64_t>, int64_t> mst;
    while (dsu.SetCount() > 1) {
        std::vector<Edge> min_edge(edges_num, {-1, -1, kMax});
        for (Edge& edge : edges) {
            int64_t first_component = dsu.FindSet(edge.from);
            int64_t second_component = dsu.FindSet(edge.to);
            if (first_component != second_component) {
                if (edge.weight < min_edge[first_component].weight) {
                    min_edge[first_component] = edge;
                }
                if (edge.weight < min_edge[second_component].weight) {
                    min_edge[second_component] = edge;
                }
            }
        }
        for (Edge& edge : min_edge) {
            if (edge.weight == kMax) {
                continue;
            }
            if (edge.to > edge.from) {
                std::swap(edge.to, edge.from);
            }
            mst[{edge.from, edge.to}] = edge.weight;
            dsu.UnioinSets(edge.from, edge.to);
        }
    }
    for (auto& edge : mst) {
        total_weight += edge.second;
    }
    return total_weight;
}