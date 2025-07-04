#pragma once

#include <iterator>
#include <vector>
#include <cstdint>

namespace linq {
namespace impl {

// forward declarations
template <typename P>
class enumerator;
template <typename It>
class range_enumerator;
template <typename P, typename F>
class select_enumerator;
template <typename P>
class drop_enumerator;
template <typename P>
class take_enumerator;

template <typename P>
class enumerator {
public:
    P begin() const { return static_cast<const P&>(*this); }
    P end() const { return static_cast<const P&>(*this); }

    template <typename F>
    select_enumerator<P, F> select(F&& func) {
        return select_enumerator<P, F>(
            std::move(static_cast<P&>(*this)), std::forward<F>(func)
        );
    }

    drop_enumerator<P> drop(uint32_t offset) {
        return drop_enumerator<P>(std::move(static_cast<P&>(*this)), offset);
    }

    take_enumerator<P> take(uint32_t offset) {
        return take_enumerator<P>(std::move(static_cast<P&>(*this)), offset);
    }

    auto to_vector() {
        std::vector<typename P::value_type> out;
        const auto& self = static_cast<const P&>(*this);
        for (auto it = self.begin(), end = self.end(); it != end; ++it) {
            out.push_back(*it);
        }
        return out;
    }
};

template <typename It>
class range_enumerator : public enumerator<range_enumerator<It>> {
public:
    using value_type = std::iterator_traits<It>::value_type;

    range_enumerator(const It& iterator, const It& end)
        : m_iterator(iterator), m_end(end) {}

    bool operator!=(const range_enumerator& other) const {
        return m_iterator != other.m_end;
    }

    value_type operator*() { return *m_iterator; }

    range_enumerator& operator++() {
        ++this->m_iterator;
        return *this;
    }
private:
    It m_iterator, m_end;
};

template <typename P, typename F>
class select_enumerator : public enumerator<select_enumerator<P, F>> {
public:
    using value_type = P::value_type;

    template <typename P_, typename F_>
    select_enumerator(P_&& parent, F_&& func)
        : m_parent(std::forward<P_>(parent)), m_func(std::forward<F_>(func)) {}

    bool operator!=(const select_enumerator& other) const {
        return m_parent.begin() != other.m_parent.end();
    }

    value_type operator*() { return m_func(*m_parent); }

    select_enumerator& operator++() {
        ++m_parent;
        return *this;
    }
private:
    P m_parent;
    F m_func;
};

template <typename P>
class drop_enumerator : public enumerator<drop_enumerator<P>> {
public:
    using value_type = P::value_type;

    template <typename P_>
    drop_enumerator(P_&& parent, uint32_t offset)
        : m_parent(std::forward<P_>(parent)), m_offset(offset) {
        for (uint32_t i = 0; i < m_offset && m_parent != m_parent.end(); ++i) {
            ++m_parent;
        }
    }

    bool operator!=(const drop_enumerator& other) const {
        return m_parent.begin() != other.m_parent.end();
    }

    value_type operator*() { return *m_parent; }

    drop_enumerator& operator++() {
        ++m_parent;
        return *this;
    }
private:
    P m_parent;
    uint32_t m_offset;
};

template <typename P>
class take_enumerator : public enumerator<take_enumerator<P>> {
public:
    using value_type = P::value_type;

    template <typename P_>
    take_enumerator(P_&& parent, uint32_t counter)
        : m_parent(std::forward<P_>(parent)), m_counter(counter) {}

    bool operator!=(const take_enumerator& other) const {
        if (!m_counter) return false;
        return m_parent.begin() != other.m_parent.end();
    }

    value_type operator*() {
        --m_counter;
        return *m_parent;
    }

    take_enumerator& operator++() {
        ++m_parent;
        return *this;
    }
private:
    P m_parent;
    uint32_t m_counter;
};

}  // namespace impl

template <typename It>
auto from(It begin, It end) {
    return impl::range_enumerator<It>(begin, end);
}

}  // namespace linq