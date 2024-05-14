#include "StrBlobPtr.h"

std::string& StrBlobPtr::deref() const {
    const auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr() {
    if(check(curr, "increment past end of StrBlobPtr")) {
        ++curr;
    }
    return *this;
}

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(const std::size_t i, const std::string &msg) const {
    auto ret = wptr.lock();
    if (!ret) {
        throw std::runtime_error("unbound StrBlobPtr");
    }
    if (i >= ret->size()) {
        throw std::out_of_range(msg);
    }
    return ret;

}
