#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <queue>
#include <deque>

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

void GetVector(std::vector<std::vector<int32_t>>& vector) {
    int64_t number_n = static_cast<int64_t>(vector.size());
    int64_t number_m = static_cast<int64_t>(vector[0].size());
    for (int64_t i = 0ll; i < number_n; ++i) {
        for (int64_t j = 0ll; j < number_m; ++j) {
            std::cin >> vector[i][j];
        }
    }
}

void FindShortestPath(std::vector<std::vector<int32_t>>& map,
                      std::vector<std::vector<int64_t>>& distance);

void Fill(std::deque<Point>& neighbours, Point point,
          int64_t size_n, int64_t size_m);

template <class TElem>
void FillVector(std::vector<std::vector<TElem>>& vector, TElem value);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t number_n = 0ll;
    int64_t number_m = 0ll;
    std::cin >> number_n >> number_m;
    
    std::vector<std::vector<int32_t>> map(number_n);
    std::vector<std::vector<int64_t>> distance(number_n);

    for (int64_t i = 0ll; i < number_n; ++i) {
        distance[i].reserve(number_m + 1ll);
        map[i].reserve(number_m + 1ll);
        distance[i].resize(number_m);
        map[i].resize(number_m);
    }
    GetVector(map);
    FillVector(distance, static_cast<int64_t>(INT_MAX));
    FindShortestPath(map, distance);

    for (int64_t i = 0ll; i < number_n; ++i) {
        for (int64_t j = 0ll; j < number_m; ++j) {
            std::cout << distance[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

Point::Point(int64_t new_x, int64_t new_y) : x(new_x), y(new_y) {
}

Point& Point::operator=(const Point& other) {
    x = other.x;
    y = other.y;
    return *this;
}

void FindShortestPath(std::vector<std::vector<int32_t>>& map,
                      std::vector<std::vector<int64_t>>& distance) {

    int64_t size_n = map.size();
    int64_t size_m = map[0].size();

    std::vector<Point> sab;
    for (int64_t i = 0ll; i < size_n; ++i) {
        for (int64_t j = 0ll; j < size_m; ++j) {
            if (map[i][j]) {
                sab.push_back(Point(i, j));
            }
        }
    }

    Point parent[size_n][size_m] = {Point(0ll, 0ll)};
    std::vector<std::vector<bool>> used(size_n);
    for (int64_t i = 0ll; i < size_n; ++i) {
        used[i].reserve(size_m + 1ll);
        used[i].resize(size_m);
    }

    FillVector(used, false);
    std::deque<Point> neighbours;
    std::queue<Point> queue;

    for (Point point : sab) {
        distance[point.x][point.y] = 0ll;
        used[point.x][point.y] = true;
        parent[point.x][point.y] = Point(-1ll, -1ll);
        queue.push(point);
    }

    while (!queue.empty()) {
        Point new_point = queue.front();
        queue.pop();
        neighbours.clear();

        Fill(neighbours, new_point, size_n, size_m);
        for (Point neighbour : neighbours) {
            if (!used[neighbour.x][neighbour.y]) {
                used[neighbour.x][neighbour.y] = true;
                queue.push(neighbour);

                distance[neighbour.x][neighbour.y] =
                    Min(distance[neighbour.x][neighbour.y], distance[new_point.x][new_point.y] + 1l);
                parent[neighbour.x][neighbour.y] = new_point;
            }
        }
    }
}

void Fill(std::deque<Point>& neighbours, Point point,
          int64_t size_n, int64_t size_m) {
    if (point.x + 1ll < size_n) {
        neighbours.push_back(Point(point.x + 1ll, point.y));
    }
    if (point.x - 1ll >= 0ll) {
        neighbours.push_back(Point(point.x - 1ll, point.y));
    }
    if (point.y + 1ll < size_m) {
        neighbours.push_back(Point(point.x, point.y + 1ll));
    }
    if (point.y - 1ll >= 0ll) {
        neighbours.push_back(Point(point.x, point.y - 1ll));
    }
}

template <class TElem>
void FillVector(std::vector<std::vector<TElem>>& vector, TElem value) {
    for (int64_t i = 0ll; i < vector.size(); ++i) {
        for (int64_t j = 0ll; j < vector[0].size(); ++j) {
            vector[i][j] = value;
        }
    }
}