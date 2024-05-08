#ifndef ROUND_ROBIN_EXAMPLE_H
#define ROUND_ROBIN_EXAMPLE_H
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

template <class T>
class RoundRobin final
{
public:
    // RoundRobin() = delete;
    explicit RoundRobin(size_t numExcepted = 0);
    ~RoundRobin() = default;
    // Prevent assignment and pass-by-value
    RoundRobin(const RoundRobin& src) = delete;
    RoundRobin& operator=(const RoundRobin& rhs) = delete;
    // Explicitly defalut a move constructor and move assignment operator
    RoundRobin(const RoundRobin&& src) noexcept = default;
    RoundRobin& operator=(const RoundRobin&& rhs) noexcept = default;

    void add(const T& element);
    void remove(const T& element);
    T& getNext();
private:
    std::vector<T> m_elemnts;
    typename std::vector<T>::iterator m_nextElement; // 当达到向量末尾时，能自动回到向量的开头继续分配或访问元素

};

class Process final {
public:
    explicit Process(std::string_view name): m_name {std::move(name)};
    void doWorkDuringTimeSlice() {
        std::cout << "Process " << m_name << " performing work during time slice." << std::endl;
    }

    bool operator==(const Process&) const = default;
private:
    std::string m_name;
};

class Scheduler final {
public:
    explicit Scheduler(const std::vector<Process>& processes) {
        for (auto& process : processes) {
            m_processes.add(process);
        }
    }
    void scheduleTimeSlice() {
        try {
            m_processes.getNext().doWorkDuringTimeSlice();
        } catch (const std::out_of_range&) {
            std::cerr << "No more processes to schedule." << std::endl;
        }
    }
    void removeProcess(const Process& process) {
        m_processes.remove(process);
    }
private:
    RoundRobin<Process> m_processes {5};
    // RoundRobin<Process> mprocesses = RoundRobin<Process>(5);

};
