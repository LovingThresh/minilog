/*
 * @file: main.cpp
 * @brief:
 * 
 * @author: liuye
 * @date: 2024.09.21
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */
#include <iostream>
#include <thread>
#include <string>
#include <vector>

std::vector<std::thread> pool;

class ThreadPool
{
public:
    void emplace_back(std::thread&& t)
    {
        m_oPool.emplace_back(std::move(t));
    }

    ~ThreadPool()
    {
        for (auto& t: m_oPool)
        {
            t.join();
        }
    }

private:
    std::vector<std::thread> m_oPool;
};

ThreadPool ThreadPoolInstance;

void download(const std::string& file)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Downloading " << file << ": " << i * 10 << "%\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Download Finished\n";
}

void interact()
{
    std::string name;
    std::cin >> name;
    std::cout << "Hello, " << name << "\n";
}

void do_work()
{
    std::thread t1([&]
    {
        download("hello.txt");
    });
    // �ƽ�����Ȩ��ȫ�ֵ�pool�б�����ӳ� t1 ����������
    pool.emplace_back(std::move(t1));
}

int main()
{
    do_work();
    interact();
    for (auto& t: pool)
    {
        t.join();
    }
    return 0;
}
