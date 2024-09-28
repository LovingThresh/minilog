#include <functional>
#include <iostream>

void funcWorld(const int nIndex)
{
    std::cout << "#" << nIndex << " World" << '\n';
}

struct PrtPrintNumArgs
{
    void operator()(const int nIndex) const
    {
        std::cout << "#" << nIndex << " Number is: " << m_nX << " " << m_nY << '\n';
    }

    int m_nX;
    int m_nY;
};

void repeatTwice(const std::function<void(int)>& func)
{
    func(1);
    func(2);
}

struct Foo
{
    explicit Foo(const int nNum)
        : m_nNum(nNum) {}

    void printAdd(const int nValue) const
    {
        std::cout << m_nNum + nValue << '\n';
    }

    int m_nNum;
};

void printNum(const int nNum)
{
    std::cout << nNum << '\n';
}

struct PrtPrintNum
{
    void operator()(const int nIndex) const
    {
        std::cout << nIndex << '\n';
    }
};

int main()
{
    constexpr int nX = 42;
    constexpr int nY = 12;
    constexpr PrtPrintNumArgs prtNum {nX, nY};

    repeatTwice([&](const int nIndex)
    {
        std::cout << "x#" << nIndex << " Number is: " << nX << ", " << nY << '\n';
    });
    repeatTwice(funcWorld);
    repeatTwice(prtNum);

    const std::function<void(int)> fDisplay = printNum;
    fDisplay(-9);

    const std::function<void()> fDisplay42 = []
    {
        printNum(42);
    };
    fDisplay42();

    const std::function<void()> fDisplay31337 = [nVal = 31337]
    {
        std::cout << nVal << '\n';
    };
    fDisplay31337();

    const std::function<void(const Foo&, int)> fAddDisplay = [](const Foo& foo, const int nValue)
    {
        foo.printAdd(nValue);
    };

    const Foo foo(314159);
    fAddDisplay(foo, 1);
    fAddDisplay(static_cast<Foo>(314159), 1);

    const std::function<int(const Foo&)> fNum = [](const Foo& f) -> int
    {
        return f.m_nNum;
    };
    std::cout << "m_nNum: " << fNum(foo) << '\n';

    auto fAddDisplay2 = [foo](const int nValue)
    {
        foo.printAdd(nValue);
    };
    fAddDisplay2(2);

    auto fAddDisplay3 = [&foo](const int nValue)
    {
        foo.printAdd(nValue);
    };
    fAddDisplay3(3);

    const std::function<void(int)> fDisplayObj = PrtPrintNum();
    fDisplayObj(18);

    auto factorial = [](const int n)
    {
        std::function<int(int)> fac = [&](const int nIndex)
        {
            return (nIndex < 2) ? 1 : nIndex * fac(nIndex - 1);
        };
        return fac(n);
    };

    for (int nI = 5; nI != 8; ++nI)
    {
        std::cout << nI << "! = " << factorial(nI) << ";  ";
    }
    std::cout << '\n';

    return 0;
}
