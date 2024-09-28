#pragma once
#include <utility>

template <typename T>
struct SpControlBlock
{
    int m_refCount;
    T* m_data;

    explicit SpControlBlock(T* data) : m_refCount(1), m_data(data) {}

    SpControlBlock(const SpControlBlock&) = delete;
    SpControlBlock& operator=(const SpControlBlock&) const = delete;
    SpControlBlock(SpControlBlock&&) = delete;
    SpControlBlock&& operator=(SpControlBlock&&) = delete;

    ~SpControlBlock() { delete m_data; }
};

template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T* ptr) : m_pcb(new SpControlBlock<T>(m_pcb)) {}

    SharedPtr(const SharedPtr& other) : m_pcb(other.m_pcb) { ++m_pcb->m_refCount; }
    ~SharedPtr()
    {
        --m_pcb->m_refCount;
        if (m_pcb->m_refCount == 0)
        {
            delete m_pcb;
        }
    }
    

private:
    SpControlBlock<T>* m_pcb;
};

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}