/*
 * @file: main.cpp
 * @brief:
 *
 * @author: liuye
 * @date: 2024.10.25
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <opencv2/core.hpp>

template <typename T = int, int Size = 10>
class Container;

template <typename T = int, int Size = 10>
class ContainerDeleter
{
public:
    void operator()(const Container<T, Size>* p)
    {
        std::cout << "Custom Deleter" << '\n';
        delete p;
    }
};

template <typename T, int Size>
class Container
{
public:
    Container()
    {
        m_pBuffer = new T[Size];
    }

    ~Container()
    {
        std::cout << "~Container!\n";
    }

private:
    T* m_pBuffer;
};

/*!
 * \brief 基础版本：没有参数时，递归终止
 */
void printX()
{

}

/*!
 * \brief This function template recursively prints each argument passed to it.
 * The base case is an overload with no parameters, which terminates the recursion.
 *
 * \tparam T The type of the first argument.
 * \tparam Types The types of the remaining arguments.
 * \param firstAgr The first argument to be printed.
 * \param args The remaining arguments to be printed.
 */
template <typename T, typename... Types>
void printX(const T& firstAgr, const Types&... args)
{
    std::cout << firstAgr << std::endl;
    printX(args...);
}

template <std::uint64_t N>
struct Fib
{
    static constexpr std::uint64_t Value = Fib<N - 1>::Value + Fib<N - 2>::Value;
};

template <>
struct Fib<1>
{
    static constexpr std::uint64_t Value = 1;
};

template <>
struct Fib<2>
{
    static constexpr std::uint64_t Value = 1;
};

void changePointer(int*& ptr) {
    // 将指针指向一个新的地址
    ptr = new int(42);  // 指向新分配的内存
}

void changePointerByValue(const int* ptr) {
    // 将指针指向一个新的地址
    ptr = new int(420);  // 指向新分配的内存
    delete ptr;
}

int main() {
    int* p = new int (10);    // 初始指针为空
    changePointer(p);     // 将p的指向改为新分配的内存
    std::cout << *p << std::endl;  // 输出42
    changePointerByValue(p);  // 尝试在函数内部修改p
    std::cout << *p << std::endl;  // 输出42
    delete p;            // 释放内存
    return 0;
}