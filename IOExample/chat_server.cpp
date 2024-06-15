#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "chat_message.h"

using boost::asio::ip::tcp;
//----------------------------------------------------------------------
void concatenateCharArrays(char* dest, const char* src1, const char* src2) {
    // 首先，计算src1的长度（不包括结束符）
    const size_t len_src1 = std::strlen(src1);

    // 将src1的内容复制到dest
    std::memcpy(dest, src1, len_src1);

    // 更新dest的位置，指向src1内容的末尾
    dest += len_src1;

    // 计算src2的长度（同样不包括结束符）
    const size_t len_src2 = std::strlen(src2);

    // 将src2的内容复制到dest现在指向的位置
    std::memcpy(dest, src2, len_src2);

    // 更新dest的位置，再次指向src2内容的末尾
    dest += len_src2;

    // 添加结束符
    *dest = '\0';
}

//----------------------------------------------------------------------

using chat_message_queue = std::deque<chat_message>;

//----------------------------------------------------------------------

class chat_participant {
public:
    virtual ~chat_participant() {
    }

    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room {
public:
    void join(const chat_participant_ptr& participant) {
        m_participants.insert(participant);
        for (auto msg: m_recent_msgs)
            participant->deliver(msg);
    }

    void leave(const chat_participant_ptr& participant) {
        m_participants.erase(participant);
    }

    void deliver(const chat_message& msg) {
        m_recent_msgs.push_back(msg);
        while (m_recent_msgs.size() > max_recent_msgs)
            m_recent_msgs.pop_front();

        for (const auto participant: m_participants)
            participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> m_participants;

    enum { max_recent_msgs = 100 };

    chat_message_queue m_recent_msgs;
};

//----------------------------------------------------------------------

class chat_session final
        : public chat_participant,
          public std::enable_shared_from_this<chat_session> {
public:
    chat_session(tcp::socket socket, chat_room&room)
        : m_socket(std::move(socket)),
          m_room(room) {
    }

    void start() {
        m_room.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg) override {

        char result_line[msg.max_body_length];
        const size_t length = std::stoi(std::string(msg.data(), msg.header_length), nullptr, 10);
        std::vector<char> sub_client_line(length + 1);
        for (int running_point = msg.header_length; running_point < msg.header_length + length; running_point++) {
            sub_client_line[running_point - msg.header_length] =  msg.data()[running_point];
        }

        sub_client_line[length] = '\0';
        concatenateCharArrays(result_line, server_line, sub_client_line.data());

        chat_message new_message;
        new_message.body_length(std::strlen(result_line)); // 设置消息体的长度
        std::memcpy(new_message.body(), result_line, new_message.body_length()); // 复制消息体数据
        new_message.encode_header(); // 编码消息头

        const bool write_in_progress = !m_write_msgs.empty();
        m_write_msgs.push_back(new_message);

        if (!write_in_progress) {
            do_write();
        }
    }

private:
    void do_read_header() {
        auto self(shared_from_this());
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_msg.data(), chat_message::header_length),
                                [this, self](const boost::system::error_code&ec, std::size_t /*length*/) {
                                    if (!ec && m_read_msg.decode_header()) {
                                        do_read_body();
                                    }
                                    else {
                                        m_room.leave(shared_from_this());
                                    }
                                });
    }

    void do_read_body() {
        auto self(shared_from_this());
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_msg.body(), m_read_msg.body_length()),
                                [this, self](const boost::system::error_code&ec, std::size_t /*length*/) {
                                    if (!ec) {
                                        m_room.deliver(m_read_msg);
                                        do_read_header();
                                    }
                                    else {
                                        m_room.leave(shared_from_this());
                                    }
                                });
    }

    void do_write() {
        auto self(shared_from_this());
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(m_write_msgs.front().data(),
                                                     m_write_msgs.front().length()),
                                 [this, self](const boost::system::error_code&ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         m_write_msgs.pop_front();
                                         if (!m_write_msgs.empty()) {
                                             do_write();
                                         }
                                     }
                                     else {
                                         m_room.leave(shared_from_this());
                                     }
                                 });
    }

    tcp::socket m_socket;
    chat_room&m_room;
    chat_message m_read_msg;
    chat_message_queue m_write_msgs;
    static constexpr char server_line[] = "This is from Server! ";
};

//----------------------------------------------------------------------

class chat_server {
public:
    chat_server(boost::asio::io_context& io_context,
                const tcp::endpoint& endpoint)
        : m_acceptor(io_context, endpoint) {
        do_accept();
    }

private:
    void do_accept() {
        m_acceptor.async_accept(
            [this](const boost::system::error_code& ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<chat_session>(std::move(socket), m_room)->start();
                }
                do_accept();
            });
    }
    tcp::acceptor m_acceptor;
    chat_room m_room;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i) {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
