#ifndef STRVEC_HPP
#define STRVEC_HPP

#include <string>
#include <xmemory>
#include <utility>

class StrVec {
public:
    StrVec();

    StrVec(const StrVec&);

    StrVec& operator=(const StrVec&);

    ~StrVec();

    void push_back(const std::string&);

    [[nodiscard]] size_t size() const { return first_free - elements; }
    [[nodiscard]] size_t capacity() const { return cap - elements; }
    [[nodiscard]] std::string* begin() const { return elements; }
    [[nodiscard]] std::string* end() const { return first_free; }

private:
    static std::allocator<std::string> alloc;
    void chk_n_alloc() { if (size() == capacity()) reallocate(); }; // 确保有空间容纳新元素
    static std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*); // 分配内存并拷贝给定范围的元素
    void free(); // 销毁元素并释放内存
    void reallocate(); // 获得更多内存并拷贝已有元素

    std::string* elements = nullptr;
    std::string* first_free = nullptr;
    std::string* cap = nullptr;
};

inline std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string* b, const std::string* e) {
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
}

inline void StrVec::free() {
    // 不能传递给deallocate一个空指针，如果elements为0，函数什么也不做
    if (elements) {
        // 逆序销毁元素
        for (auto p = first_free; p != elements; /* 空 */ )
            alloc.destroy(--p);
        alloc.deallocate(elements, cap - elements);
    }
}

inline void StrVec::push_back(const std::string& s) {
    chk_n_alloc();
    alloc.construct(first_free++, s);
    // std::allocator_traits<std::allocator<std::string>>::construct(alloc, first_free++, s); // For C++17 and later
}
#endif //STRVEC_HPP
