#ifndef STRBLOBPTR_H
#define STRBLOBPTR_H
#include <memory>
#include <vector>
#include <string>

#include "StrBlob.h"

class StrBlobPtr {
public:
    StrBlobPtr(): curr(0) {};
    explicit StrBlobPtr(const StrBlob &a, const std::size_t sz = 0): wptr(a.data), curr(sz) {};
    std::string& deref() const;
    StrBlobPtr& incr();

private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};
#endif //STRBLOBPTR_H
