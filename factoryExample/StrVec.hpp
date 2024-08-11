#ifndef STRVEC_HPP
#define STRVEC_HPP

#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>
#include <xmemory>

class StrVec {
public:
    StrVec() = default;
    StrVec(std::initializer_list<std::string>);

    StrVec(const StrVec &);

    StrVec &operator=(const StrVec &);

    ~StrVec();

    void resize(size_t);
    void resize(size_t, const std::string &);
    void reverse(size_t);
    void push_back(const std::string &);

    [[nodiscard]] size_t size() const { return first_free - elements; }
    [[nodiscard]] size_t capacity() const { return cap - elements; }
    [[nodiscard]] std::string *begin() const { return elements; }
    [[nodiscard]] std::string *end() const { return first_free; }

private:
    static std::allocator<std::string> alloc;
    void chk_n_alloc() {
        if (size() == capacity())
            reallocate();
    }; // 确保有空间容纳新元素
    static std::pair<std::string *, std::string *> alloc_n_copy(const std::string *,
                                                                const std::string *); // 分配内存并拷贝给定范围的元素
    void free(); // 销毁元素并释放内存
    void reallocate(); // 获得更多内存并拷贝已有元素
    void reallocate(size_t); // 获得更多内存并拷贝已有元素


    std::string *elements = nullptr;
    std::string *first_free = nullptr;
    std::string *cap = nullptr;
};

inline std::pair<std::string *, std::string *> StrVec::alloc_n_copy(const std::string *b, const std::string *e) {
    if (b != nullptr && e != nullptr) {
        auto data = alloc.allocate(e - b);
        return {data, uninitialized_copy(b, e, data)};
    }
    return {nullptr, nullptr};
}

inline void StrVec::free() {
    // 不能传递给deallocate一个空指针，如果elements为0，函数什么也不做
    if (elements) {
        // 逆序销毁元素
        for (auto p = first_free; p != elements; /* 空 */) {
            --p;
            alloc.destroy(p);
        }
        alloc.deallocate(elements, cap - elements);
    }
}

inline StrVec::StrVec(const std::initializer_list<std::string> il) {
    const auto newdata = alloc_n_copy(il.begin(), il.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

inline StrVec::StrVec(const StrVec &s) {
    std::cout << "Copy constructor" << std::endl;
    const auto newData = alloc_n_copy(s.begin(), s.end());
    elements = newData.first;
    first_free = cap = newData.second;
}

inline StrVec::~StrVec() { free(); }

inline void StrVec::resize(const size_t n) {
    // 如果n小于当前大小，则删除尾部元素
    // 如果n大于当前大小，则添加默认构造的元素
    if (n > size()) {
        while (size() < n)
            push_back("");
    }
    else if (n < size()) {
        while (size() > n) {
            first_free--;
            alloc.destroy(first_free);
        }
    }
}

inline void StrVec::resize(const size_t n, const std::string &s) {
    // 如果n小于当前大小，则删除尾部元素
    // 如果n大于当前大小，则添加默认构造的元素
    if (n > size()) {
        while (size() < n)
            push_back(s);
    }
    else if (n < size()) {
        while (size() > n) {
            first_free--;
            alloc.destroy(first_free);
        }
    }
}

inline void StrVec::reverse(const size_t n) {
    if (n > capacity())
        reallocate(n);
}

inline void StrVec::reallocate(const size_t n) {
    const auto newData = alloc.allocate(n);
    auto dest = newData;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i) {
        alloc.construct(dest, std::move(*elem));
        dest++;
        elem++;
    }
    free();
    elements = newData;
    first_free = dest;
    cap = elements + n;
}

inline void StrVec::push_back(const std::string &s) {
    chk_n_alloc();
    alloc.construct(first_free, s);
    first_free++;
    // std::allocator_traits<std::allocator<std::string>>::construct(alloc, first_free++, s); // For C++17 and later
}

inline StrVec &StrVec::operator=(const StrVec &rhs) {
    std::cout << "Copy assignment" << std::endl;
    const auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

inline void StrVec::reallocate() {
    const auto newcapacity = size() ? 2 * size() : 1;
    const auto newData = alloc.allocate(newcapacity);
    auto dest = newData;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i) {
        alloc.construct(dest, std::move(*elem));
        dest++;
        elem++;
    }
    free();
    elements = newData;
    first_free = dest;
    cap = elements + newcapacity;
}

std::allocator<std::string> StrVec::alloc;

#endif //STRVEC_HPP
