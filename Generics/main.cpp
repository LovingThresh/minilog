/*
 * @file: main.cpp
 * @brief:
 * 
 * @author: liuye
 * @date: 2024.10.25
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/scope_exit.hpp>
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

    static T m_variable;

private:
    T* m_pBuffer;
};

// 模板类的静态成员变量属于类的所有特化，因此需要单独定义。
template <typename T = int, int Size = 10>
T Container<T, Size>::m_variable;


int main()
{
    int data = 42;

    // 使用 Lambda 表达式在作用域退出时执行清理操作
    BOOST_SCOPE_EXIT(&data) {
        std::cout << "Scope exit: Cleaning up, data = " << data << std::endl;
    } BOOST_SCOPE_EXIT_END

    BOOST_SCOPE_EXIT(void)
    {
        std::cout << "Scope exit: Cleaning up" << std::endl;
    } BOOST_SCOPE_EXIT_END
    
    const std::vector<int> vec {1, 3, 5, 7, 2, 9};
    std::ranges::for_each(vec,
                  [](const int n) -> void
                  {
                      std::cout << n << ',';
                  });
    // std::unique_ptr<Container<double, 12>, ContainerDeleter<double, 12>> pContainer(new Container<double, 12>);
    return 0;
}