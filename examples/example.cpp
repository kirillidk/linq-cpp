#include "linq.hpp"
#include <assert.h>
#include <vector>
#include <iostream>

using linq::from;

void from_to_vector() {
    std::vector<int> xs = {1, 2, 3};
    std::vector<int> res = from(xs.begin(), xs.end()).to_vector();
    assert(res == xs);
}

void from_select() {
    const int xs[] = {1, 2, 3};
    std::vector<int> res =
        from(xs, xs + 3).select([](int x) { return x + 5; }).to_vector();
    std::vector<int> expected = {6, 7, 8};
    assert(res == expected);
}

void from_drop_select() {
    const int xs[] = {1, 2, 3};
    std::vector<int> res = from(xs, xs + 3)
                               .drop(1)
                               .select([](int x) { return x + 5; })
                               .to_vector();
    std::vector<int> expected = {7, 8};
    assert(res == expected);
}

void range() {
    const int xs[] = {1, 2, 3};
    auto res = from(xs, xs + 3).select([](int x) { return x * x; });
    std::vector<int> res_vec;

    for (auto&& el : res) {
        res_vec.emplace_back(el);
    }

    std::vector<int> expected = {1, 4, 9};
    assert(res_vec == expected);
}

void range_to_vector() {
    const int xs[] = {1, 2, 3};
    auto res = from(xs, xs + 3).select([](int x) { return x * x; }).to_vector();

    std::vector<int> expected = {1, 4, 9};
    assert(res == expected);
}

int main() {
    from_to_vector();
    from_select();
    from_drop_select();
    range();
    range_to_vector();

    return 0;
}
