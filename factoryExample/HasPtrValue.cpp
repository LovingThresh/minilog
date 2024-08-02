#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>

class HasPtrPointer;

class HasPtrValue {
public:
    explicit HasPtrValue(const std::string& s = std::string()) : ps(new std::string(s)), i(0) {
    }
    HasPtrValue(const HasPtrValue& hp) : ps(new std::string(*hp.ps)), i(hp.i) {
    }
    HasPtrValue& operator=(const HasPtrValue& rhs) {
        delete ps;
        const auto newp = new std::string(*rhs.ps);
        ps = newp;
        i = rhs.i;
        return *this;
    }
    bool operator<(const HasPtrValue &rhs) const {
        return *ps < *rhs.ps;
    }
    std::string operator*() const {
        return *ps;
    }
    ~HasPtrValue() {
        delete ps;
    }

    friend void swap(HasPtrValue &lhs, HasPtrValue &rhs) noexcept;

private:
    std::string *ps;
    int i;
};

inline void swap(HasPtrValue &lhs, HasPtrValue &rhs) noexcept {
    using std::swap;
    std::cout << "Swapping " << *lhs.ps << " with " << *rhs.ps << std::endl;
    swap(lhs.ps, rhs.ps);
    swap(lhs.i, rhs.i);
}

int main(int argc, char **argv)
{
    const HasPtrValue h("hi mom!");
    HasPtrValue h2(h);
    HasPtrValue h3 = h;
    h2 = HasPtrValue("hi dad!");
    h3 = HasPtrValue("hi son!");
    swap(h2, h3);
    std::cout << "h: " << *h << std::endl;
    std::cout << "h2: " << *h2 << std::endl;
    std::cout << "h3: " << *h3 << std::endl;
    std::vector<HasPtrValue> vh;
    constexpr int n = 32;

    for (int i = 0; i < n; ++i) {
        vh.push_back(HasPtrValue(std::to_string(n - i)));
    }
    // for (const auto p : vh) {
    //     std::cout << *p << std::endl;
    // }
    std::cout << std::endl;

    const auto start_time = std::chrono::high_resolution_clock::now();

    std::sort(vh.begin(), vh.end());

    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // for (const auto p : vh) {
    //     std::cout << *p << std::endl;
    // }
    std::cout << "Sorting time: " << duration << " microseconds" << std::endl;
    std::cout << "End of program" << std::endl;
    return 0;
}

