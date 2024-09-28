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

class TextFinder
{
public:
    // 左值引用构造函数，处理 const std::string& 情况
    explicit TextFinder(const std::string& sInputText) : m_sText(sInputText) {}

    // 右值引用构造函数，处理 std::string&& 情况
    explicit TextFinder(std::string&& sInputText) : m_sText(std::move(sInputText)) {}

    // 查找并打印字符位置的方法
    void findAndPrintCharPosition(const char nTargetChar) const
    {
        const auto pCharPos = std::find(m_sText.cbegin(), m_sText.cend(), nTargetChar);
        if (pCharPos != m_sText.cend())
        {
            std::cout << "Character '" << nTargetChar
                      << "' found at position: " << std::distance(m_sText.cbegin(), pCharPos) << '\n';
        }
        else
        {
            std::cout << "Character '" << nTargetChar << "' not found.\n";
        }
    }

    // 查找并打印子字符串位置的方法
    void searchAndPrintSubstringPosition(const std::string& sTargetSubstring) const
    {
        const auto pSubstringPos =
            std::search(m_sText.cbegin(), m_sText.cend(), sTargetSubstring.cbegin(), sTargetSubstring.cend());

        if (pSubstringPos != m_sText.cend())
        {
            std::cout << "Substring \"" << sTargetSubstring
                      << "\" found at position: " << std::distance(m_sText.cbegin(), pSubstringPos) << '\n';
        }
        else
        {
            std::cout << "Substring \"" << sTargetSubstring << "\" not found.\n";
        }
    }

private:
    // 私有成员变量，使用 m_ 前缀
    std::string m_sText;
};

int main()
{
    processStandardFind(); // 调用标准查找函数
    processParallelFind(); // 调用并行查找函数

    const std::string sText = "The quick brown fox jumps over the lazy dog.";
    constexpr char cNTargetChar = 'o';
    const std::string sTargetSubstring = "lazy";

    // 创建 TextFinder 对象
    const TextFinder textFinder(sText);

    // 调用方法查找字符和子字符串的位置
    textFinder.findAndPrintCharPosition(cNTargetChar);
    textFinder.searchAndPrintSubstringPosition(sTargetSubstring);

    return 0;
}
