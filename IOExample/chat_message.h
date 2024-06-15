#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
1. Header（报头）: 在网络编程中，报头通常是数据包或消息开始的一段固定长度的数据，用于携带控制信息或描述跟随其后的数据体（body）的属性。
    例如，这里的`header_length = 4`表明每个消息都有一个4字节长的头部，可能用于存储消息体的长度信息。
2. Body（消息体）: 消息体是网络消息的核心内容部分，即用户实际想要传输的数据。
    在这个类中，`max_body_length = 512`定义了单个消息所能容纳的最大数据量。
3. Encode/Decode（编码/解码）: 这两个术语指的是准备数据以便于传输（编码）和接收后恢复原始形式（解码）的过程。
    `encode_header()`方法将消息体长度转换成字符串形式并存入报头，而`decode_header()`则执行相反的操作，从报头提取并解析出消息体的长度。
    在网络通信中，编码和解码是必不可少的步骤，确保数据能在不同系统间正确传输和理解。
4. Buffer（缓冲区）: 虽然代码中没有直接出现“缓冲区”这个词，但`char m_data[header_length + max_body_length]`实际上就是一个缓冲区，
    用于暂存待发送或刚接收到的数据，包括报头和消息体。
    缓冲区在网络编程中至关重要，因为它帮助管理数据的收发，尤其是在数据可能分批到达或需要按块处理的场景下。
5. Length（长度）: 在网络消息处理中，了解数据的精确长度是必要的，以便正确读取和解析。
    `length()`方法提供了整个消息（报头加消息体）的总长度，这对于在网络上传输和重组消息片段非常关键。
6. Message（消息）: 整个`chat_message`类设计围绕着处理单一的消息实体，这是网络编程中的基本单位，
    无论是文本聊天、文件传输还是其他任何形式的数据交换，都是通过一系列消息的发送和接收来完成的。
*/

class chat_message
{
public:
    static constexpr std::size_t header_length = 4;
    static constexpr std::size_t max_body_length = 512;

    chat_message()
      : m_data{"Hello"}, m_body_length{0} {
    }

    const char* data() const
    {
        return m_data;
    }

    char* data()
    {
        return m_data;
    }

    std::size_t length() const
    {
        return header_length + m_body_length;
    }

    const char* body() const
    {
        return m_data + header_length;
    }

    char* body()
    {
        return m_data + header_length;
    }

    std::size_t body_length() const
    {
        return m_body_length;
    }

    void body_length(const std::size_t new_length)
    {
        m_body_length = new_length;
        if (m_body_length > max_body_length)
            m_body_length = max_body_length;
    }

    bool decode_header()
    {
        char header[header_length + 1] = "";
        strncat_s(header, m_data, header_length);
        m_body_length = std::atoi(header);
        if (m_body_length > max_body_length)
        {
            m_body_length = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        char header[header_length + 1] = "";
        sprintf_s(header, "%4d", static_cast<int>(m_body_length));
        std::memcpy(m_data, header, header_length);
    }

private:
    char m_data[header_length + max_body_length];
    std::size_t m_body_length;
};

#endif //CHAT_MESSAGE_H
