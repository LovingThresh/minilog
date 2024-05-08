#include "round_robin_example.h"

template<class T>
RoundRobin<T>::RoundRobin(size_t numExcepted) {
    m_elemnts.reserve(numExcepted);
    m_nextElement = std::begin(m_elemnts);
}

template<class T>
void RoundRobin<T>::add(const T& element) {
    ptrdiff_t pos {m_nextElement - std::begin(m_elemnts)};
    m_elemnts.push_back(element);
    m_nextElement = std::begin(m_elemnts) + pos;
}

template<class T>
void RoundRobin<T>::remove(const T& element) {
    for (auto it {std::begin(m_elemnts)} ; it != std::end(m_elemnts); ++it) {
        if (*it == element) {
            ptrdiff_t newPos;
            if (m_nextElement == std::end(m_elemnts) - 1 && m_nextElement == it) {
                newPos = 0;
            } else if (m_nextElement <= it) {
                newPos = m_nextElement = std::begin(m_elemnts);
            } else {
                newPos = m_nextElement - std::begin(m_elemnts) - 1;
            }
            m_elemnts.erase(it);
            m_nextElement = std::begin(m_elemnts) + newPos;
            return;
        }
    }
}

template<class T>
T& RoundRobin<T>::getNext() {
    if (m_elemnts.empty()) {
        throw std::out_of_range("No elements in the list");
    }
    auto& toReturn {*m_nextElement};
    ++m_nextElement;
    if (m_nextElement == std::end(m_elemnts)) {
        m_nextElement = std::begin(m_elemnts);
    }
    return toReturn;
}
