#include <iostream>
int b = 5;
int fun(int n);
int main() {
    for (int i = 0; i < 3; i++) {
        std::cout << fun(i) << std::endl;
    }
}
int fun(int n) {
    static int a = 1;
    int c = 10;
    a = a + 100;
    b = b + 100;
    c = c + 100;
    std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;
    return a+b+c;
}
