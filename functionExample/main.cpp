#include <iostream>
#include <functional>

void funcWorld(const int i) {
    std::cout << "#" << i << "World" << '\n';
}

struct printnum_args_t {
    void operator()(const int i) const {
        std::cout << "#" << i << "Number is: " << x << ' ' << y << '\n';
    }

    int&x;
    int&y;
};

void repeatTwice(const std::function<void (int)>&func) {
    func(1);
    func(2);
}

#include <functional>
#include <iostream>

struct Foo {
    explicit Foo(const int num) : num_(num) {
    }

    void print_add(const int i) const { std::cout << num_ + i << '\n'; }
    int num_;
};

void print_num(const int i) {
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(const int i) const {
        std::cout << i << '\n';
    }
};

int main() {
    int x = 42, y = 12;
    const printnum_args_t num{x, y};
    repeatTwice([&](const int i) {
        std::cout << "x#x" << i << "Number is: " << x << ',' << y << '\n';
    });
    repeatTwice(funcWorld);
    repeatTwice(num);

    // store a free function
    std::function f_display = print_num;
    f_display(-9);

    // store a lambda
    std::function f_display_42 = [] {
        print_num(42);
    };
    f_display_42();

    // store the result of a call to std::bind
    std::function f_display_31337 = [i = 31337] {
        std::cout << i << '\n';
    };

    f_display_31337();

    // store a call to a member function
    std::function f_add_display = [](const Foo& foo, const int i) {
        foo.print_add(i);
    };

    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(static_cast<Foo>(314159), 1);

    // store a call to a data member accessor
    std::function f_num = [](const Foo& f) -> int {
        return f.num_;
    };
    std::cout << "num_: " << f_num(foo) << '\n';

    // store a call to a member function and object
    auto f_add_display2 = [foo](const int i) {
        foo.print_add(i);
    };
    f_add_display2(2);

    // store a call to a member function and object ptr
    auto f_add_display3 = [&foo](const int i) {
        foo.print_add(i);
    };
    f_add_display3(3);

    // store a call to a function object
    std::function f_display_obj = PrintNum();
    f_display_obj(18);

    auto factorial = [](const int n) {
        // store a lambda object to emulate "recursive lambda"; aware of extra overhead
        std::function<int(int)> fac = [&](const int i) { return (i < 2) ? 1 : i * fac(i - 1); };
        // note that "auto fac = [&](int n) {...};" does not work in recursive calls
        return fac(n);
    };
    for (int i{5}; i != 8; ++i)
        std::cout << i << "! = " << factorial(i) << ";  ";
    std::cout << '\n';
    return 0;
}
