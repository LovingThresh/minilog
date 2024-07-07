#include <iostream>
#include <exception>

class Sales_data {
public:
    Sales_data() = default;
    explicit Sales_data(std::istream& is);
    explicit Sales_data(std::string&& s) : bookNo(std::move(s)) {
    }
    Sales_data(std::string&& s, const unsigned int n, const double d)
        : bookNo(std::move(s)), units_sold(n), revenue(d * n) {
    }
    [[nodiscard]] std::string isbn() const { return bookNo; }
    Sales_data& combine(const Sales_data&);
    [[nodiscard]] double avg_price() const;

    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};

Sales_data add(const Sales_data&, const Sales_data&);
std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&);

Sales_data& Sales_data::combine(const Sales_data& rhs) {
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}

double Sales_data::avg_price() const {
    if (units_sold)
        return revenue / units_sold;
    return 0;
}

Sales_data add(const Sales_data& lhs, const Sales_data& rhs) {
    Sales_data sum = lhs;
    sum.combine(rhs);
    return sum;
}

std::istream& read(std::istream& is, Sales_data& item) {
    try {
        item = Sales_data(); // 清理旧数据
        double price = 0;
        is >> item.bookNo >> item.units_sold >> price;
        item.revenue = price * item.units_sold;
    }
    catch (...) {
        throw std::runtime_error("Failed to read data");
    }
    return is;
}

std::ostream& print(std::ostream& os, const Sales_data& item) {
    os << item.isbn() << " " << item.units_sold << " "
            << item.revenue << " " << item.avg_price();
    return os;
}

Sales_data::Sales_data(std::istream& is) {
    read(is, *this);
}

int main() {
    try {
        if (Sales_data currentBookData; read(std::cin, currentBookData)) {
            Sales_data trans;
            while (read(std::cin, trans)) {
                if (currentBookData.isbn() == trans.isbn())
                    currentBookData.combine(trans);
                else {
                    print(std::cout, currentBookData) << '\n';
                    currentBookData = trans;
                }
            }
            print(std::cout, currentBookData) << '\n';
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
