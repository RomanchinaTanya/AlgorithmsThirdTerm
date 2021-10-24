#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t MAX = 10001;

void FindShortestPath(int64_t from, int64_t to);

void Fill(std::deque<int64_t>& neighbours, int64_t number);

int main() {
    int64_t number_from = 0ll;
    int64_t number_to = 0ll;
    std::cin >> number_from >> number_to;
    FindShortestPath(number_from, number_to);

    return 0;
}

void FindShortestPath(int64_t from, int64_t to) {
    int64_t distance[MAX] = {0ll};
    int64_t parent[MAX] = {0ll};
    bool used[MAX] = {false};
    std::deque<int64_t> neighbours;

    std::queue<int64_t> queue;
    queue.push(from);

    used[from] = true;
    parent[from] = -1ll;

    while (!queue.empty()) {
        int64_t new_number = queue.front();
        queue.pop();
        neighbours.clear();
        Fill(neighbours, new_number);
        for (int64_t neighbour : neighbours) {
            if (!used[neighbour]) {
                used[neighbour] = true;
                queue.push(neighbour);
                distance[neighbour] = distance[new_number] + 1ll;
                parent[neighbour] = new_number;
            }
        }
    }

    if (!used[to]) {
        std::cout << "-1" << std::endl;
    } else {
        std::cout << distance[to] + 1ll << std::endl;
        std::deque<int64_t> right_path;

        while (to != from) {
            right_path.push_front(to);
            to = parent[to];
        }
        right_path.push_front(from);

        for (int64_t number : right_path) {
            std::cout << number << std::endl;
        }
    }
}

void Fill(std::deque<int64_t>& neighbours, int64_t number) {
    if (number / 1000ll != 9ll) {
        neighbours.push_back(number + 1000ll);
    }
    if (number % 10ll != 1ll) {
        neighbours.push_back(number - 1ll);
    }
    int64_t part = number / 1000ll;
    int64_t temp_number = (number % 1000ll)  * 10ll + part;
    neighbours.push_back(temp_number);
    part = number % 10ll;
    temp_number = (number / 10ll) + part * 1000ll;
    neighbours.push_back(temp_number);
}