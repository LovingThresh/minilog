/*
 * @file: main.cpp
 * @brief:
 *
 * @author: liuye
 * @date: 2025.02.23
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */
#include <format>
#include <iostream>
#include <ranges>
#include <vector>

// 辅助函数，用于输出
template <typename T>
void println(const T& value)
{
    std::cout << value << std::endl;
}

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    auto t = [](int n)
    {
        println(std::format("transform({})", n));
        return n * 2;
    };

    auto f = [](int n)
    {
        println(std::format("filter({})", n));
        return n % 4 == 0;
    };

    auto transformed_filtered_with_output = numbers
                                                        | std::views::transform(t)
                                                        | std::views::filter(f);

    auto filterVector = std::vector(transformed_filtered_with_output.begin(), transformed_filtered_with_output.end());

    return 0;
}
