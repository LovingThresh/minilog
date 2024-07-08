#include <iostream>
#include <string>
#include <vector>

class Screen;
// class BitMap;
extern std::ostream& storeOn(std::ostream&, const Screen&);
// extern BitMap& storeOn(BitMap&, const Screen&);

class Screen {
    friend class Window_mgr;
    friend std::ostream& storeOn(std::ostream&, const Screen&);
    // friend void Window_mgr::clear(ScreenIndex);
public:
    using pos = std::string::size_type;

    Screen(const pos ht, const pos wd) : height(ht), width(wd), contents(ht * wd, ' ') {
    }
    Screen(const pos ht, const pos wd, const char c) : height(ht), width(wd), contents(ht * wd, c) {
    }

    Screen(const Screen&other) : height(other.height), width(other.width), contents(other.contents) {
    }

    // void set(const pos r, const pos c, const char ch) {
    //     contents[r * width + c] = ch;
    // }

    [[nodiscard]] pos size() const;

    Screen& set(char);

    Screen& set(pos, pos, char);

    Screen& move(const pos r, const pos c) {
        cursor = r * width + c;
        return *this;
    }

    Screen& display(std::ostream&os) {
        do_display(os);
        return *this;
    }

    const Screen& display(std::ostream&os) const {
        do_display(os);
        return *this;
    }

    [[nodiscard]] char get(const pos r, const pos c) const {
        return contents[r * width + c];
    }

    pos height = 0, width = 0;

private:
    pos cursor = 0;

    void do_display(std::ostream&os) const {
        for (pos r = 0; r < height; ++r) {
            for (pos c = 0; c < width; ++c) {
                os << contents[r * width + c];
            }
            os << "\n";
        }
    }

    std::string contents;
};

Screen::pos Screen::size() const {
    return height * width;
}

inline Screen& Screen::set(const char ch) {
    contents[cursor] = ch;
    return *this;
}

inline Screen& Screen::set(const pos r, const pos c, const char ch) {
    contents[r * width + c] = ch;
    return *this;
}

class Window_mgr {
public:
    using ScreenIndex = std::vector<Screen>::size_type;
    void clear(ScreenIndex);
    ScreenIndex addScreen(const Screen&);
private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};
};

Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen&) {
    screens.emplace_back(std::move(Screen(24, 80, ' ')));
    return screens.size() - 1;
}

void Window_mgr::clear(ScreenIndex i) {
    Screen& screen = screens[i];
    screen.contents = std::string(screen.height * screen.width, ' ');
}

int main() {
    Screen screen1(3, 4);
    screen1.set(0, 0, 'X');
    screen1.set(1, 1, 'Y');

    std::cout << "Before copy:\n";
    for (Screen::pos r = 0; r < screen1.height; ++r) {
        for (Screen::pos c = 0; c < screen1.width; ++c) {
            std::cout << screen1.get(r, c);
        }
        std::cout << "\n";
    }

    Screen screen2(screen1);

    std::cout << "After copy:\n";
    for (Screen::pos r = 0; r < screen2.height; ++r) {
        for (Screen::pos c = 0; c < screen2.width; ++c) {
            std::cout << screen2.get(r, c);
        }
        std::cout << "\n";
    }

    screen2.set(0, 0, 'Z'); // 修改screen2的第一个字符

    std::cout << "After modifying screen2:\n";
    for (Screen::pos r = 0; r < screen1.height; ++r) {
        for (Screen::pos c = 0; c < screen1.width; ++c) {
            std::cout << screen1.get(r, c);
        }
        std::cout << "\n";
    }

    Screen myScreen(5, 5, 'X');
    myScreen.move(4, 0).set('#').display(std::cout);
    std::cout << "\n";
    myScreen.display(std::cout);
    std::cout << "\n";
    return 0;
}
