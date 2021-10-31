#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

const int64_t MAX_HEIGHT = 10001ll;

template <class TElem>
TElem Min(TElem first, TElem second) {
    return first < second ? first : second;
}

struct Point {
    int64_t x;
    int64_t y;

    Point() = default;
    ~Point() = default;
    Point(int64_t new_x, int64_t new_y);
    Point& operator=(const Point& other);
};

template <class TElem>
void GetVector(std::vector<std::vector<TElem>>& vector, int64_t sizeN, int64_t sizeM);

void Fill(std::deque<Point>& neighbours, Point point,
          int64_t sizeN, int64_t sizeM);

void DrainDFS(Point point, bool& drains,
              std::vector<std::vector<bool>>& used,
              std::vector<std::vector<int64_t>>& height,
              int64_t sizeN, int64_t sizeM);

void Solve(std::vector<std::vector<int64_t>>& height, int64_t sizeN, int64_t sizeM);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t sizeN = 0ll;
    int64_t sizeM = 0ll;
    std::cin >> sizeN >> sizeM;
    std::vector<std::vector<int64_t>> height(sizeN + 2ll,
                std::vector<int64_t>(sizeM + 2ll, MAX_HEIGHT));
    GetVector(height, sizeN, sizeM);

    Solve(height, sizeN, sizeM);

    return 0;
}

Point::Point(int64_t new_x, int64_t new_y) : x(new_x), y(new_y) {
}

Point& Point::operator=(const Point& other) {
    x = other.x;
    y = other.y;
    return *this;
}

template <class TElem>
void GetVector(std::vector<std::vector<TElem>>& vector, int64_t sizeN, int64_t sizeM) {
    for (int64_t i = 1ll; i < sizeN + 1ll; ++i) {
        for (int64_t j = 1ll; j < sizeM + 1ll; ++j) {
            std::cin >> vector[i][j];
        }
    }
}

void Fill(std::deque<Point>& neighbours, Point point,
          int64_t sizeN, int64_t sizeM) {
    if (point.x + 1ll <= sizeN) {
        neighbours.push_back(Point(point.x + 1ll, point.y));
    }
    if (point.x - 1ll >= 0ll) {
        neighbours.push_back(Point(point.x - 1ll, point.y));
    }
    if (point.y + 1ll <= sizeM) {
        neighbours.push_back(Point(point.x, point.y + 1ll));
    }
    if (point.y - 1ll >= 0ll) {
        neighbours.push_back(Point(point.x, point.y - 1ll));
    }
}

void DrainDFS(Point point, bool& drains,
              std::vector<std::vector<bool>>& used,
              std::vector<std::vector<int64_t>>& height,
              int64_t sizeN, int64_t sizeM) {
    
    used[point.x][point.y] = true;
    bool heightAroundIsLess = (height[point.x][point.y] > height[point.x + 1ll][point.y]) ||
                              (height[point.x][point.y] > height[point.x - 1ll][point.y]) ||
                              (height[point.x][point.y] > height[point.x][point.y + 1ll]) ||
                              (height[point.x][point.y] > height[point.x][point.y - 1ll]);
    if (heightAroundIsLess) {
        drains = true;
    }

    std::deque<Point> neighbours;
    Fill(neighbours, point, sizeN, sizeM);
    for (Point to : neighbours) {
        if ((height[point.x][point.y] == height[to.x][to.y]) && (!used[to.x][to.y])) {
            DrainDFS(to, drains, used, height, sizeN, sizeM);
        }
    }
}

void Solve(std::vector<std::vector<int64_t>>& height, int64_t sizeN, int64_t sizeM) {
    std::vector<std::vector<bool>> used(sizeN + 2ll,
                std::vector<bool>(sizeM + 2ll, false));
    int64_t drainsCount = 0ll;
    for (int64_t i = 1ll; i < sizeN + 1ll; ++i) {
        for (int64_t j = 1ll; j < sizeM + 1ll; ++j) {
            if (!used[i][j]) {
                bool drains = false;
                DrainDFS(Point(i, j), drains, used, height, sizeN, sizeM);
                if (!drains) {
                    ++drainsCount;
                }
            }
        }
    }
    std::cout << drainsCount << std::endl;
}