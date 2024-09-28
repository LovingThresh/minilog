#include <algorithm>
#include <chrono> // 计时库
#include <execution> // 并行执行策略
#include <iostream>
#include <vector>

// 按照规范将函数名以小写驼峰命名
void processStandardFind()
{
    std::vector<int> vnNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 使用 std::find 查找值为 5 的元素
    auto pit = std::ranges::find(vnNumbers, 5);
    if (pit != vnNumbers.end())
    {
        std::cout << "找到了元素: " << *pit << '\n';
    }
    else
    {
        std::cout << "没有找到元素 5" << '\n';
    }

    // 使用 std::find_if 查找第一个偶数
    pit = std::ranges::find_if(vnNumbers, [](int nX) { return nX % 2 == 0; });
    if (pit != vnNumbers.end())
    {
        std::cout << "找到了第一个偶数: " << *pit << '\n';
    }

    // 使用 std::find_if_not 查找第一个非偶数
    pit = std::ranges::find_if_not(vnNumbers, [](int nX) { return nX % 2 == 0; });
    if (pit != vnNumbers.end())
    {
        std::cout << "找到了第一个奇数: " << *pit << '\n';
    }
}

void processParallelFind()
{
    // 创建一个包含 1 到 1,000,000 的向量
    std::vector<int> vnLargeNumbers(1000000);
    std::iota(vnLargeNumbers.begin(), vnLargeNumbers.end(), 1); // 填充1到1000000

    constexpr int nTargetValue = 999999;

    // 使用并行执行策略进行查找
    const auto start = std::chrono::high_resolution_clock::now();
    const auto pit = std::find(std::execution::par, vnLargeNumbers.begin(), vnLargeNumbers.end(), nTargetValue);
    const auto end = std::chrono::high_resolution_clock::now();

    if (pit != vnLargeNumbers.end())
    {
        std::cout << "找到了元素: " << *pit << '\n';
    }
    else
    {
        std::cout << "未找到元素" << '\n';
    }

    const std::chrono::duration<double> dDuration = end - start;
    std::cout << "并行查找耗时: " << dDuration.count() << " 秒" << '\n';
}

int main()
{
    processStandardFind(); // 调用标准查找函数
    processParallelFind(); // 调用并行查找函数
    return 0;
}
