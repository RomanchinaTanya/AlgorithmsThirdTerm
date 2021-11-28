#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>

struct Edge {
    int32_t from;
    int32_t to;
    int32_t weight;
};

bool Comparator(Edge first, Edge second) {
    return first.weight < second.weight;
}

class DSU {
public:
    explicit DSU(int32_t max_size);
    int32_t MakeSet(int32_t x);
    int32_t FindSet(int32_t x);
    void UnioinSets(int32_t x, int32_t y);

private:
    std::vector<int32_t> parent_;
    std::vector<int32_t> rang_;
    int32_t set_count_;
};

int32_t Kruskal(DSU& dsu, std::vector<Edge>& edges);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int32_t vertex_num = 0;
    int32_t edges_num = 0;

    std::cin >> vertex_num >> edges_num;
    std::vector<Edge> edges(edges_num);
    for (int32_t i = 0; i < edges_num; ++i) {
        std::cin >> edges[i].from >> edges[i].to >> edges[i].weight;
        --edges[i].from;
        --edges[i].to;
    }
    std::sort(edges.begin(), edges.end(), Comparator);
    DSU dsu(vertex_num);
    for (int32_t vertex = 0; vertex < vertex_num; ++vertex) {
        dsu.MakeSet(vertex);
    }

    int32_t total_weight = Kruskal(dsu, edges);

    std::cout << total_weight << "\n";

    return 0;
}

DSU::DSU(int32_t max_size) : set_count_(0) {
    parent_.resize(max_size, -1);
    rang_.resize(max_size, 0);
}

int32_t DSU::MakeSet(int32_t x) {
    ++set_count_;
    return parent_[x] = x;
}

int32_t DSU::FindSet(int32_t x) {
    if (x == parent_[x]) {
        return x;
    }
    return parent_[x] = FindSet(parent_[x]);
}

void DSU::UnioinSets(int32_t x, int32_t y) {
    x = FindSet(x);
    y = FindSet(y);
    if (rang_[x] < rang_[y]) {
        parent_[x] = y;
    } else if (rang_[y] < rang_[x]) {
        parent_[y] = x;
    } else {
        parent_[x] = y;
        ++rang_[y];
    }
}

int32_t Kruskal(DSU& dsu, std::vector<Edge>& edges) {
    int32_t total_weight = 0;
    int32_t edges_num = edges.size();
    for (int32_t i = 0; i < edges_num; ++i) {
        if (dsu.FindSet(edges[i].from) != dsu.FindSet(edges[i].to)) {
            total_weight += edges[i].weight;
            dsu.UnioinSets(edges[i].from, edges[i].to);
        }
    }
    return total_weight;
}