#ifndef STRBLOB_H
#define STRBLOB_H

#include <vector>
#include <string>
#include <memory>

#include "StrBlobPtr.h"

class StrBlobPtr;

class StrBlob {
    friend class StrBlobPtr;
public:
    typedef std::vector<std::string>::size_type size_type; // 无符号整型
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);

    [[nodiscard]] size_type size() const { return data->size(); }
    [[nodiscard]] bool empty() const { return data->empty(); }
    void push_back(const std::string &t) const { data->push_back(t); }
    void pop_back() const ;
    [[nodiscard]] std::string &front();
    [[nodiscard]] std::string &back();
    [[nodiscard]] const std::string& front() const;
    [[nodiscard]] const std::string& back() const;
    void print() const;
    StrBlobPtr begin() {return StrBlobPtr(*this);}
    StrBlobPtr end() {
        auto ret = StrBlobPtr(*this, data->size());
        return ret;
    }
private:
    mutable std::shared_ptr<std::vector<std::string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type i, const std::string & msg) const;
};



#endif //STRBLOB_H
