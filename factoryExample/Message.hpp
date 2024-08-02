// 每个Message都保存一个它所在Folder的指针的set
// 每个Folder都保存一个它包含的Message1的指针的set
// Message提供save与remove操作，向给定的Folder添加或删除自己
// 为了创建一个新的Message，需要指明内容，但是不需要指定Floder，将通过save操作来完成
// 当拷贝Message时，副本与原对象应该是不同的对象，但是应该在相同的Folder中
// 当销毁一个Message时，应该从它所在的Folder中删除它
// 当一个Message对象赋予给另一个Message对象时，左侧的内容会被右侧的内容替代，
// 还需要更新Folder集合，删除原来包含左侧Message的Folder，添加右侧Message的Folder
#ifndef MESSAGE_H
#define MESSAGE_H

#include <set>
#include <string>

class Message;

class Folder {
public:
    Folder() = default;

    Folder(const Folder& f) : messages(f.messages) {
        add_to_Messages(f);
    }

    Folder& operator=(const Folder& f) {
        remove_to_Messages();
        messages = f.messages;
        add_to_Messages(f);
        return *this;
    }

    ~Folder() {
        remove_to_Messages();
    }

    void addMsg(Message*);

    void remMsg(Message*);

    void add_to_Messages(const Folder&);

    void remove_to_Messages();

private:
    std::set<Message *> messages;
};

class Message {
    friend class Folder;

public:
    explicit Message(const std::string& str = "") : contents(str) {
    }

    Message(const Message&); // 拷贝构造函数
    Message& operator=(const Message&); // 拷贝赋值运算符
    ~Message(); // 析构函数

    void save(Folder&);

    void remove(Folder&);

    void addFldr(Folder*);

    void remFldr(Folder*);

    friend void swap(Message&, Message&) noexcept;

private:
    std::string contents;
    std::set<Folder *> folders;

    void add_to_Folders(const Message&);

    void remove_from_Folders();
};

inline Message::Message(const Message& m) : contents(m.contents), folders(m.folders) {
    add_to_Folders(m); // 更新Folder集合
}

inline Message& Message::operator=(const Message& rhs) {
    remove_from_Folders(); // 更新Folder集合
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders(rhs); // 更新Folder集合
    return *this;
}

inline Message::~Message() {
    remove_from_Folders(); // 更新Folder集合
}

inline void Message::addFldr(Folder* f) {
    folders.insert(f);
}

inline void Message::remFldr(Folder* f) {
    folders.erase(f);
}

inline void swap(Message& lhs, Message& rhs) noexcept {
    using std::swap;
    for (const auto f: lhs.folders) {
        f->remMsg(&lhs);
    }
    for (const auto f: rhs.folders) {
        f->remMsg(&rhs);
    }

    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);

    for (const auto f: lhs.folders) {
        f->addMsg(&lhs);
    }
    for (auto f: rhs.folders) {
        f->addMsg(&rhs);
    }
}

inline void Message::save(Folder& f) {
    folders.insert(&f);
    f.addMsg(this);
}

inline void Message::remove(Folder& f) {
    folders.erase(&f);
    f.remMsg(this);
}

inline void Message::add_to_Folders(const Message& m) {
    for (const auto f: m.folders) {
        f->addMsg(this);
    }
}

inline void Message::remove_from_Folders() {
    for (const auto f: folders) {
        f->remMsg(this);
    }
}

inline void Folder::addMsg(Message* m) {
    messages.insert(m);
}

inline void Folder::remMsg(Message* m) {
    messages.erase(m);
}

inline void Folder::add_to_Messages(const Folder& f) {
    for (const auto m: f.messages) {
        m->addFldr(this);
    }
}

inline void Folder::remove_to_Messages() {
    while (!messages.empty()) {
        (*messages.begin())->remove(*this);
    }
}

#endif
