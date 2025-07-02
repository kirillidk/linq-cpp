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

    enumerator(It iterator) : m_iterator(iterator) {}

    bool operator!=(const enumerator<It, P>& other) const {
        return m_iterator != other.m_iterator;
    }

    It get_iterator() const noexcept { return m_iterator; }

    template <typename F>
    select_enumerator<It, P, F> select(F func) {
        return select_enumerator<It, P, F>(
            m_iterator, static_cast<const P&>(*this), func
        );
    }

    auto to_vector() {
        std::vector<value_type> out;
        const auto& self = static_cast<const P&>(*this);
        for (auto it = self.begin(), end = self.end(); it != end; ++it) {
            out.push_back(*it);
        }
        return out;
    }
protected:
    It m_iterator;
};

template <typename It>
class range_enumerator : public enumerator<It, range_enumerator<It>> {
public:
    using value_type = enumerator<It, range_enumerator<It>>::value_type;

    range_enumerator(const It& begin, const It& end)
        : enumerator<It, range_enumerator<It>>(begin),
          m_begin(begin),
          m_end(end) {}

    range_enumerator begin() const { return range_enumerator(m_begin, m_end); }
    range_enumerator end() const { return range_enumerator(m_end, m_end); }

    value_type operator*() const { return *this->get_iterator(); }

    range_enumerator& operator++() {
        ++this->m_iterator;
        return *this;
    }
private:
    It m_begin, m_end;
};

template <typename It, typename P, typename F>
class select_enumerator : public enumerator<It, select_enumerator<It, P, F>> {
public:
    using value_type = enumerator<It, select_enumerator<It, P, F>>::value_type;

    select_enumerator(const It& iterator, const P& parent, const F& func)
        : enumerator<It, select_enumerator<It, P, F>>(iterator),
          m_parent(parent),
          m_func(func) {}

    select_enumerator begin() const {
        return select_enumerator(
            m_parent.begin().get_iterator(), m_parent, m_func
        );
    }
    select_enumerator end() const {
        return select_enumerator(
            m_parent.end().get_iterator(), m_parent, m_func
        );
    }

    value_type operator*() const { return m_func(*m_parent); }

    select_enumerator& operator++() {
        ++this->m_iterator;
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