#pragma once

#include <vector>
#include <functional>

namespace linq {

// forward declarations
template <typename It>
class range_enumerator;
template <typename It, typename F>
class select_enumerator;

template <typename It>
auto from(It begin, It end) {
    return range_enumerator(begin, end);
}

template <typename It>
class range_enumerator {
public:
    range_enumerator(It begin, It end) : m_begin(begin), m_end(end) {}

    template <typename F>
    select_enumerator<It, F> select(F&& func) {
        return select_enumerator<It, F>(*this, std::forward<F>(func));
    }

    auto to_vector() { return std::vector(m_begin, m_end); }
protected:
    It m_begin, m_end;
};

template <typename It, typename F>
class select_enumerator : public range_enumerator<It> {
public:
    select_enumerator(const range_enumerator<It>& parent, F&& func)
        : range_enumerator<It>(parent), m_func(std::move(func)) {}

    auto to_vector() {
        auto res = std::vector(
            range_enumerator<It>::m_begin, range_enumerator<It>::m_end
        );
        for (auto it = res.begin(); it != res.end(); ++it) *it = m_func(*it);

        return res;
    }
private:
    F m_func;
};

}
