#include <iostream>
#include <string>
#include "Message.hpp"

int main() {
    // 创建一些Message对象
    Message msg1("Hello, world!");
    Message msg2("Good morning!");
    Message msg3("Good evening!");

    // 创建一些Folder对象
    Folder folder1;
    Folder folder2;

    // 将消息保存到文件夹中
    msg1.save(folder1);
    msg2.save(folder1);
    msg3.save(folder2);

    // 使用拷贝构造函数创建新的Message
    Message msg4(msg1); // 应该复制msg1的内容，并且存在于相同的文件夹中
    msg4.save(folder2); // 将msg4添加到folder2


    return 0;
}