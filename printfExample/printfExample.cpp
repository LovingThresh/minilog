#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
template<typename T1, typename T2>
struct common_type_two {
    using type = decltype(true ? std::declval<T1>() : std::declval<T2>());
};

template<typename... Ts>
struct common_type {};

template<typename T0>
struct common_type<T0> {
    using type = T0;
};

template<typename T0, typename T1>
struct common_type<T0, T1> {
    using type = typename common_type_two<T0, T1>::type;
};

template<typename T0, typename T1, typename... Ts>
struct common_type<T0, T1, Ts...> {
    using type = typename common_type_two<T0, typename common_type<T1, Ts...>::type>::type;
};

template<typename T0, typename... Ts>
constexpr auto get_common_type(T0 t0, Ts... ts) {
    if constexpr (sizeof...(Ts) == 0) {
        return t0;
    }
    else {
        return true ? t0 : get_common_type(ts...);
    }
}
// template <typename T0, typename ...Ts>
// struct common_type<T0, Ts...> {
//     using type = typename common_type_two<T0, typename common_type<Ts...>::type>::type;
// };

template<typename T>
static void print(T t) {
    std::cout << t << '\n';
}

template<typename... Ts>
auto func(Ts... ts) {
    (print(ts), ...);
    // return (0 + ... + ts);
    // this will expand to:
    // return (ts + ...); // This will not work because the first element is 0
}

// 在C++中判断一个字符串数组的首字母是否为大写
bool is_captial(const std::string &s) { return std::isupper(s[0]); }

bool is_captial(const char *s) { return std::isupper(s[0]); }

// 将单词的首字母转换为大写
void capitalizeFirstLetter(std::string &word) {
    if (!word.empty()) {
        if (!std::isupper(word[0])) {
            word[0] = std::toupper(word[0]);
        }
    }
}

// 处理整个字符串
std::string capitalizeWords(const std::string &input) {
    std::string result;
    std::string word;

    for (const char c: input) {
        if (std::isspace(c)) {
            if (!word.empty()) {
                capitalizeFirstLetter(word);
                result += word;
                word.clear();
            }
            result += c;
        }
        else {
            word += c;
        }
    }
    // 添加最后一个单词
    if (!word.empty()) {
        capitalizeFirstLetter(word);
        result += word;
    }

    return result;
}


template<typename T1, typename T2>
double myRound(const T1 nValue, const T2 dValue, const int nPlace) {
    double dBase = 1.0;
    for (int i = 0; i < nPlace; i++) {
        dBase *= 10.0;
    }
    return(nValue * dBase + static_cast<int>(dValue * dBase)) / dBase;
}


class myLog;

template<typename T>
class Complex {
public:
    friend myLog;
    Complex() = default;
    Complex(T r, T i) : real(r), image(i) {}
    Complex operator+(const Complex<T> &);

private:
    T real{0};
    T image{0};
};

template<typename T>
Complex<T> Complex<T>::operator+(const Complex<T> &c) {
    Complex<T> result;
    result.real = real + c.real;
    result.image = image + c.image;
    return result;
}
// 实现一个自定义myLog
class myLog {
public:
    template<typename T>
    myLog &operator<<(const T &value) {
        buffer_ << value;
        return *this;
    }

    template<typename T>
    myLog &operator<<(const Complex<T> &value) {
        buffer_ << value.real;
        if (value.image >= 0) {
            buffer_ << "+";
        }
        buffer_ << value.image << "i";
        return *this;
    }

    myLog& operator<<(std::ostream& (*func)(std::ostream&)) {
        // 处理 std::endl
        buffer_ << "\n";
        return *this;
    }

    void saveLog() { std::cout << "Save log: " << buffer_.str() << std::endl; }

private:
    std::stringstream buffer_; // 用于存储日志信息的缓冲区
};

int main() {
    const std::string test = "hello world, this is an example.";
    const std::string result = capitalizeWords(test);

    std::cout << "Original: " << test << std::endl;
    std::cout << "Modified: " << result << std::endl;

    const double d = myRound(5, 1.1234, 3);
    //assert(d == 6.123);

    myLog log;
    const Complex<int> c1(1, -1);
    log << 'A' << 123 << 3.14 << "Hello, World!" << std::endl;
    log.saveLog();
    log << c1;
    log.saveLog();

    return 0;
}
