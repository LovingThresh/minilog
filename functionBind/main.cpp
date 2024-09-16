#include <functional>
#include <iostream>
#include <vector>

/*!
 * \brief
 */
template<typename... T>
struct Signal
{
    std::vector<std::function<void(T...)>> m_callbacks;

    void connect(const std::function<void(T...)> &callback)
    {
        m_callbacks.push_back(callback);
    }

    void emit(T... t)
    {
        for (auto &&callback: m_callbacks)
        {
            callback(t...);
        }
    }
};

struct Foo
{
    void on_input(const int i) const
    {
        std::cout << "Foo age: " << age << "Foo got: " << i << std::endl;
    }

    int age = 14;
};

struct Bar
{
    void on_input(const int i) const
    {
        std::cout << "Bar age: " << age << "Bar got: " << i << std::endl;
    }

    static void on_exit()
    {
        std::cout << "Bar got exit event" << std::endl;
    }

    int age = 42;
};


struct Input
{
    void main_loop()
    {
        int i;
        while (std::cin >> i)
        {
            on_input.emit(i);
        }
        on_exit.emit();
    }

    Signal<int> on_input;
    Signal<> on_exit;
};

int main()
{
    Input input;
    Foo foo;
    Bar bar;
    input.on_input.connect([&foo](const int i) { foo.on_input(i); });
    input.on_input.connect([&bar](const int i) { bar.on_input(i); });
    input.on_exit.connect([&bar]() { bar.on_exit(); });
}
