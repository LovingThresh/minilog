/*
 * @file: test.cpp
 * @brief:
 * 
 * @author: liuye
 * @date: 2025.03.01
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */
#include <algorithm>
#include <format>
#include <iostream>
#include <string_view>
#include <utility>
#include <type_traits>
#include <gtest/gtest.h>

TEST(LambdaCapture, ValueCapture) {
    int id = 0;
    auto lambda_value_capture = [id]() mutable {
        const int temp = id;
        id++;
        return temp;
    };
    id = 42;
    EXPECT_EQ(0, lambda_value_capture());
    EXPECT_EQ(1, lambda_value_capture());
    EXPECT_EQ(2, lambda_value_capture());
    EXPECT_EQ(42, id); // Verify that the original 'id' is unchanged.
}

TEST(LambdaCapture, ReferenceCapture) {
    int id = 0;
    auto lambda_reference_capture = [&id]() {
        const int temp = id;
        id++;
        return temp;
    };
    id = 42;
    EXPECT_EQ(42, lambda_reference_capture());
    EXPECT_EQ(43, lambda_reference_capture());
    EXPECT_EQ(44, lambda_reference_capture());
    EXPECT_EQ(45, id); // Verify that the original 'id' is modified.
}

void printX()
{
    std::cout << "there is nothing.\n";
}

template <typename T, typename... Types>
void printX(T firstArg, Types... args)
{
    std::cout << firstArg << "\n";
    printX(args...);
}

TEST(MetaProgram, AiasMetaProgram)
{
    printX(1, 2.0, std::string("hello world"));
}

// 基础情况 - 没有参数时
void format_print(const std::string_view fmt) {
    std::cout << fmt;
}

// 使用编译时检查的格式字符串
template <typename... Args>
void format_print(const std::format_string<Args...> fmt, Args&&... args)
{
    try
    {
        std::cout << std::format(fmt, std::forward<Args>(args)...);
    }
    catch (const std::format_error&)
    {
        throw std::runtime_error("Invalid format string or arguments");
    }
}

TEST(FormatPrint, TypeSafeFormatting)
{
    format_print("Integer: {}, Double: {}, String: {}", 1, 2.0, std::string("hello world"));
}

// 定义concept检查类型是否可比较
template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

// 检查两个类型是否有共同类型且该共同类型也是Comparable
template <typename T, typename U>
concept CommonComparable = requires {
    typename std::common_type_t<T, U>;
} && Comparable<std::common_type_t<T, U>>;

// 基本情况：单个参数
template <Comparable T, Comparable U>
auto maximum(const T n, const U m) {
    using CommonType = std::common_type_t<T, U>;
    return std::max<CommonType>(n, m);
}

// 递归情况：确保所有类型都有公共可比较类型
template <Comparable T, Comparable U, Comparable... Rest>
requires CommonComparable<T, U>
auto maximum(T firstArg, U secondArg, Rest... rest) {
    using CommonType = std::common_type_t<T, U>;

    if constexpr (sizeof...(rest) == 0) {
        return firstArg > secondArg ? static_cast<CommonType>(firstArg) : static_cast<CommonType>(secondArg);
    } else {
        // 递归比较剩余的参数
        auto maxRest = maximum(secondArg, rest...);
        using FinalType = std::common_type_t<T, decltype(maxRest)>;
        return firstArg > maxRest ? static_cast<FinalType>(firstArg) : static_cast<FinalType>(maxRest);
    }
}

TEST(MetaMaxm, MetaProgram_AiasMetaProgram_Test)
{
    std::cout << maximum(1, 2.0, 3.9, 2.1, 0.4, 5, 6.9, 12.1, 'a');
}

TEST(RValue, RValueForString)
{
    std::string s1 ("hello");
    std::string s2 ("world");
    s1 + s2 = s2;
    std::cout << s1;
    std::cout << s2;
    std::string() = "hello";
    std::vector<int> v1 = {1, 2, 3, 4};
    v1.insert(v1.end(), 2);
}

struct Test1
{
    int f;
};

struct Test2
{
    static int f()
    {
        return 42;
    };
};

struct Test3
{

};

template <typename T>
concept HasMemF = requires(T t) {
    { t.f() } -> std::same_as<int>;
};

template <typename T>
requires HasMemF<T>
void func(const T& t)
{

}

TEST(HasMemF, HasMemF)
{
    constexpr Test2 t;
    func(t);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}