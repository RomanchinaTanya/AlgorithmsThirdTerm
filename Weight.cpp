#include <iostream>
#include <vector>
#include <unordered_map>

enum REQUEST { INSERT_EDGE = 1, FIND_TOTAL_WEIGHT = 2 };

template <class SetType>
class DisjointSetUnion {
public:
    explicit DisjointSetUnion(size_t max_size);
    void Initialize(size_t max_size);
    bool UnioinSets(SetType first_element, SetType second_element, size_t weight);
    size_t FindTotalWeight(SetType element);

private:
    std::unordered_map<SetType, SetType> parent_;
    std::unordered_map<SetType, size_t> rank_;
    std::unordered_map<SetType, size_t> weight_;

    SetType MakeSet(SetType element);
    SetType FindSet(SetType element);
};

template <class SetType>
void RunTimeSolver(DisjointSetUnion<SetType>& components, size_t request_num);

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t vertex_num = 0;
    size_t request_num = 0;

    std::cin >> vertex_num >> request_num;

    DisjointSetUnion<int32_t> components(vertex_num + 1);
    components.Initialize(vertex_num + 1);

    RunTimeSolver(components, request_num);

    return 0;
}

template <class SetType>
DisjointSetUnion<SetType>::DisjointSetUnion(size_t max_size) {
    parent_.reserve(max_size);
    rank_.reserve(max_size);
    weight_.reserve(max_size);
}

template <class SetType>
void DisjointSetUnion<SetType>::Initialize(size_t max_size) {
    for (size_t element = 1; element < max_size; ++element) {
        MakeSet(static_cast<SetType>(element));
    }
}

template <class SetType>
SetType DisjointSetUnion<SetType>::MakeSet(SetType element) {
    parent_.insert({element, element});
    weight_.insert({element, 0});
    rank_.insert({element, 0});
    return parent_[element];
}

template <class SetType>
SetType DisjointSetUnion<SetType>::FindSet(SetType element) {
    if (element == parent_[element]) {
        return element;
    }
    return parent_[element] = FindSet(parent_[element]);
}

template <class SetType>
bool DisjointSetUnion<SetType>::UnioinSets(SetType first_element, SetType second_element, size_t weight) {
    first_element = FindSet(first_element);
    second_element = FindSet(second_element);
    if (first_element == second_element) {
        weight_[first_element] += weight;
        return false;
    }
    if (rank_[first_element] < rank_[second_element]) {
        std::swap(first_element, second_element);
    }
    parent_[second_element] = first_element;
    weight_[first_element] += weight + weight_[second_element];
    if (rank_[first_element] == rank_[second_element]) {
        ++rank_[first_element];
    }
    return true;
}

template <class SetType>
size_t DisjointSetUnion<SetType>::FindTotalWeight(SetType element) {
    return weight_[FindSet(element)];
}

template <class SetType>
void RunTimeSolver(DisjointSetUnion<SetType>& components, size_t request_num) {
    SetType requsted_vertex = 0;
    SetType from_vertex = 0;
    SetType to_vertex = 0;
    int32_t request = 0;
    size_t weight = 0;

    for (size_t i = 0; i < request_num; ++i) {
        std::cin >> request;
        if (request == INSERT_EDGE) {
            std::cin >> from_vertex >> to_vertex >> weight;
            components.UnioinSets(from_vertex, to_vertex, weight);
        } else if (request == FIND_TOTAL_WEIGHT) {
            std::cin >> requsted_vertex;
            std::cout << components.FindTotalWeight(requsted_vertex) << "\n";
        }
    }
}