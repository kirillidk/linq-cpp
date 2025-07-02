#pragma once

#include <iterator>
#include <vector>

namespace linq {
namespace impl {

// forward declarations
template <typename It, typename P>
class enumerator;
template <typename It>
class range_enumerator;
template <typename It, typename P, typename F>
class select_enumerator;

template <typename It, typename P>
class enumerator {
public:
    using value_type = std::iterator_traits<It>::value_type;

    P begin() const { return static_cast<const P&>(*this); }
    P end() const { return static_cast<const P&>(*this); }

    template <typename F>
    select_enumerator<It, P, F> select(F func) {
        return select_enumerator<It, P, F>(static_cast<const P&>(*this), func);
    }

    auto to_vector() {
        std::vector<value_type> out;
        const auto& self = static_cast<const P&>(*this);
        for (auto it = self.begin(), end = self.end(); it != end; ++it) {
            out.push_back(*it);
        }
        return out;
    }
};

template <typename It>
class range_enumerator : public enumerator<It, range_enumerator<It>> {
public:
    using value_type = enumerator<It, range_enumerator<It>>::value_type;

    range_enumerator(const It& iterator, const It& end)
        : m_iterator(iterator), m_end(end) {}

    bool operator!=(const range_enumerator& other) const {
        return m_iterator != m_end;
    }

    value_type operator*() const { return *m_iterator; }

    range_enumerator& operator++() {
        ++this->m_iterator;
        return *this;
    }
private:
    It m_iterator, m_end;
};

template <typename It, typename P, typename F>
class select_enumerator : public enumerator<It, select_enumerator<It, P, F>> {
public:
    using value_type = enumerator<It, select_enumerator<It, P, F>>::value_type;

    select_enumerator(const P& parent, const F& func)
        : m_parent(parent), m_func(func) {}

    bool operator!=(const select_enumerator& other) const {
        return m_parent.begin() != m_parent.end();
    }

    value_type operator*() const { return m_func(*m_parent); }

    select_enumerator& operator++() {
        ++m_parent;
        return *this;
    }
private:
    P m_parent;
    F m_func;
};

}  // namespace impl

template <typename It>
auto from(It begin, It end) {
    return impl::range_enumerator<It>(begin, end);
}

}  // namespace linq