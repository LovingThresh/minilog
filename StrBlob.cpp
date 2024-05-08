#include "StrBlob.h"
#include <stdexcept>
#include <iostream>

StrBlob::StrBlob() : data(std::make_shared<std::vector<std::string>>()) {}

StrBlob::StrBlob(std::initializer_list<std::string> il) : data(std::make_shared<std::vector<std::string>>(il)) {}

void StrBlob::pop_back() {
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

std::string& StrBlob::front() {
    check(0, "front on empty StrBlob");
    return data->front();
}

std::string& StrBlob::back() {
    check(0, "back on empty StrBlob");
    return data->back();
}

const std::string& StrBlob::front() const {
    check(0, "front on empty StrBlob");
    return data->front();
}

const std::string& StrBlob::back() const {
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::print() const {
    for (const auto &elem : *data) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void StrBlob::check(const size_type i, const std::string&msg) const {
    if ( i >= data->size() ) throw std::out_of_range(msg);
}

