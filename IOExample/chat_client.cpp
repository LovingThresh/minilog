#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.h"

using boost::asio::ip::tcp;

using chat_message_queue = std::deque<chat_message>;

class chat_client {
public:
    chat_client(boost::asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints)
        : m_io_context(io_context),
          m_socket(io_context) {
        do_connect(endpoints);
    }

    void write(const chat_message& msg) {
        boost::asio::post(m_io_context,
                          [this, msg]() {
                              const bool write_in_progress = !m_write_msgs.empty();
                              m_write_msgs.push_back(msg);
                              if (!write_in_progress) {
                                  do_write();
                              }
                          });
    }

    void close() {
        boost::asio::post(m_io_context, [this]() { m_socket.close(); });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
        boost::asio::async_connect(m_socket, endpoints,
                                   [this](const boost::system::error_code&ec, tcp::endpoint) {
                                       if (!ec) {
                                           do_read_header();
                                       }
                                   });
    }

    void do_read_header() {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_msg.data(), chat_message::header_length),
                                [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                                    if (!ec && m_read_msg.decode_header()) {
                                        do_read_body();
                                    }
                                    else {
                                        m_socket.close();
                                    }
                                });
    }

    void do_read_body() {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_msg.body(), m_read_msg.body_length()),
                                [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                                    if (!ec) {
                                        std::cout.write(m_read_msg.body(), m_read_msg.body_length());
                                        std::cout << "\n";
                                        do_read_header();
                                    }
                                    else {
                                        m_socket.close();
                                    }
                                });
    }

    void do_write() {
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(m_write_msgs.front().data(),
                                                     m_write_msgs.front().length()),
                                 [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         m_write_msgs.pop_front();
                                         if (!m_write_msgs.empty()) {
                                             do_write();
                                         }
                                     }
                                     else {
                                         m_socket.close();
                                     }
                                 });
    }

private:
    boost::asio::io_context& m_io_context;
    tcp::socket m_socket;
    chat_message m_read_msg;
    chat_message_queue m_write_msgs;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1)) {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
