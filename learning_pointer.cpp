#include <cassert> // cassert 用于在程序中插入断言
#include <cstdio> // cstdio 用于输入输出
#include <fstream> // 用于文件输入输出操作
#include <iostream> // 用于输入输出
#include <memory> // 用于内存操作
#include <stdexcept> // 用于异常处理
#include <vector>

#include "StrBlob.h"
//用于运行时多态演示的辅助类
struct B {
    virtual ~B() = default;
    virtual void bar() { std::cout << "B::bar()\n"; }
};

struct D final : B {
    D() { std::cout << "D::D\n"; }
    ~D() override { std::cout << "D::~D\n"; }

    void bar() override { std::cout << "D::bar\n"; }
};

// 消费unique_ptr 的辅助函数，能以值或以右值引用接受它
std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
    p->bar();
    return p;
}

// 用于显示下面自定义删除其演示的辅助函数
void close_file(std::FILE* fp) {
    std::fclose(fp);
}

// 基于unique_ptr 的链表演示
struct List {
    struct Node {
        int data;
        std::unique_ptr<Node> next;
    };
    std::unique_ptr<Node> head;

    ~List() {
        while (head) {
            auto next = std::move(head->next); // 释放当前节点，并获取下一个节点
            head = std::move(next); // 将下一个节点设置为当前节点
        }
    }

    void push(const int data) {
        head = std::make_unique<Node>(Node{data, std::move(head)}); // 创建新节点，并将其设置为当前节点
    }
};

int main()
{
    auto ptr = std::make_unique<int>(10);

    std::cout << "1) 独占所有权语义演示\n";
    {
        // 创建一个（独占）资源
        std::unique_ptr<D> p = std::make_unique<D>();
        // 转移所有权给 `pass_through`，而它再通过返回值将所有权转移回来
        std::unique_ptr<D> q = pass_through(std::move(p));
        // p 现在是已被移动的“空”状态，等于 nullptr
        assert(!p);
    }

    std::cout << "\n" "2) 运行时多态演示\n";
    {
        // 创建派生类资源并通过基类指向它
        const std::unique_ptr<B> p = std::make_unique<D>();
        // 动态派发如期工作
        p->bar();
    }

    std::cout << "\n" "3) 自定义删除器演示\n";
    std::ofstream("demo.txt") << 'x'; // 准备要读取的文件
    {
        using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
        const unique_file_t fp(std::fopen("demo.txt", "r"), &close_file);
        if (fp)
            std::cout << static_cast<char>(std::fgetc(fp.get())) << '\n';
    } // `close_file()` 于此调用（若 `fp` 为空）

    std::cout << "\n" "4) 自定义 lambda 表达式删除器和异常安全性演示\n";
    try
    {
        std::unique_ptr<const D, void(*)(const D*)> p(new D, [](const D* ptr)
        {
            std::cout << "由自定义删除器销毁...\n";
            delete ptr;
        });

        throw std::runtime_error(""); // `p` 若为普通指针则此处将泄漏
    }
    catch (const std::exception&)
    {
        std::cout << "捕获到异常\n";
    }

    std::cout << "\n" "5) 数组形式的 unique_ptr 演示\n";
    {
        std::unique_ptr<D[]> p(new D[3]);
    } // `D::~D()` 被调用 3 次

    const std::vector<int> original = {1, 2, 3, 4, 5};
    std::vector<int> sharedCopy = original; // 浅拷贝，共享元素
    sharedCopy[0] = 10;

    std::cout << "Original vector: ";
    for (const int num : original) {
        std::cout << num << " "; // 输出: 1 2 3 4 5
    }
    std::cout << std::endl;

    std::cout << "Shared copy vector: ";
    for (const int num : sharedCopy) {
        std::cout << num << " "; // 输出: 10 2 3 4 5
    }
    std::cout << std::endl;

    StrBlob b1;
    {
        StrBlob b2 = {"a", "an", "the"};
        b1 = b2; // 深拷贝，复制指针
        b2.push_back("about");
        b2.print();
    }
    b1.print();
}
