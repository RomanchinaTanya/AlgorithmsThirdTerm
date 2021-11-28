#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>

class DSU {
public:
    explicit DSU(int64_t max_size);
    int64_t MakeSet(int64_t x);
    int64_t FindSet(int64_t x);
    int64_t UnioinSets(int64_t x, int64_t y);

private:
    std::vector<int64_t> parent_;
    std::vector<int64_t> rang_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t islands_num = 0;
    int64_t bridges_num = 0;
    int64_t bridges_min = 0;
    int64_t from = 0;
    int64_t to = 0;

    std::cin >> islands_num >> bridges_num;
    DSU islands(islands_num + 1);

    for (int64_t i = 0; i < islands_num; ++i) {
        islands.MakeSet(i);
    }
    for (int64_t i = 0; i < bridges_num; ++i) {
        std::cin >> from >> to;
        islands_num -= islands.UnioinSets(from, to);
        ++bridges_min;
        if (islands_num == 1) {
            break;
        }
    }

    std::cout << bridges_min << std::endl;

    return 0;
}

DSU::DSU(int64_t max_size) {
    parent_.resize(max_size, -1);
    rang_.resize(max_size, 0);
}

int64_t DSU::MakeSet(int64_t x) {
    return parent_[x] = x;
}

int64_t DSU::FindSet(int64_t x) {
    if (x == parent_[x]) {
        return x;
    }
    return parent_[x] = FindSet(parent_[x]);
}

int64_t DSU::UnioinSets(int64_t x, int64_t y) {
    x = FindSet(x);
    y = FindSet(y);
    if (x == y) {
        return 0;
    }
    if (rang_[x] < rang_[y]) {
        parent_[x] = y;
        return 1;
    }
    if (rang_[y] < rang_[x]) {
        parent_[y] = x;
        return 1;
    }
    parent_[x] = y;
    ++rang_[y];
    return 1;
}