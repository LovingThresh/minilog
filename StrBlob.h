#ifndef STRBLOB_H
#define STRBLOB_H

#include <vector>
#include <string>
#include <memory>

class StrBlob {
public:
    typedef std::vector<std::string>::size_type size_type; // 无符号整型
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);

    [[nodiscard]] size_type size() const { return data->size(); }
    [[nodiscard]] bool empty() const { return data->empty(); }
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    [[nodiscard]] std::string &front();
    [[nodiscard]] std::string &back();
    [[nodiscard]] const std::string& front() const;
    [[nodiscard]] const std::string& back() const;
    void print() const;
private:
    std::shared_ptr<std::vector<std::string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type i, const std::string & msg) const;
};



#endif //STRBLOB_H
