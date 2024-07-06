#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <optional>

std::optional<int> parseInt(const std::string_view str) {
    try {
        return std::stoi(std::string(str));
    } catch (...) { // 一种用来捕获所有类型异常的通用方式
        return std::nullopt;
    }
}

// Product base class
class Product {
public:
    Product(std::string name, const double price) : m_name(std::move(name)), m_price(price) {}
    virtual ~Product() = default;
    virtual void printInfo() const = 0;  // Pure virtual function for printing product info
protected:
    std::string m_name;
    double m_price;
};

// Pizza class
class Pizza final : public Product {
public:
    explicit Pizza(std::string name, const double price, std::string taste)
        : Product(std::move(name), price), m_taste(std::move(taste)) {
        std::cout << "Making Pizza " << '\n';
    }
    void printInfo() const override {
        std::cout << "Pizza Name: " << m_name << ", Price: " << m_price << ", Taste: " << m_taste << '\n';
    }
private:
    std::string m_taste;
};

// ToyCar class
class ToyCar final : public Product {
public:
    explicit ToyCar(std::string name, const double price, std::string type)
        : Product(std::move(name), price), m_type(std::move(type)) {
        std::cout << "Making Toy Car " << '\n';
    }
    void printInfo() const override {
        std::cout << "ToyCar Name: " << m_name << ", Price: " << m_price << ", Type: " << m_type << '\n';
    }
private:
    std::string m_type;
};

// Box base class
class Box {
public:
    explicit Box(std::unique_ptr<Product> product)
        : m_product(std::move(product)) {}
    virtual ~Box() = default;
    virtual void printInfo() const = 0;  // Pure virtual function for printing box info
protected:
    std::unique_ptr<Product> m_product;
};

// GourmetBox class
class GourmetBox final : public Box {
public:
    explicit GourmetBox(std::unique_ptr<Product> product)
        : Box(std::move(product)), m_price(10) {
        std::cout << "Wrapping with GourmetBox" << '\n';
    }
    void printInfo() const override {
        std::cout << "GourmetBox Price: " << m_price << ", Material: " << m_material << ", Address: " << m_address << '\n';
        m_product->printInfo();
    }
private:
    double m_price;
    std::string m_material = "Cardboard";
    std::string m_address = "LinLin Road";
};

// PlayBox class
class PlayBox final : public Box {
public:
    explicit PlayBox(std::unique_ptr<Product> product)
        : Box(std::move(product)), m_price(20) {
        std::cout << "Wrapping with PlayBox" << '\n';
    }
    void printInfo() const override {
        std::cout << "PlayBox Price: " << m_price << ", Material: " << m_material << ", Info: " << m_info << '\n';
        m_product->printInfo();
    }
private:
    double m_price;
    std::string m_material = "Plastic";
    std::string m_info = "Made In China";
};

// Factory class for creating products and wrapping them in boxes
class Factory {
public:
    template<typename ProductType, typename... Args>
    static std::unique_ptr<ProductType> createProduct(Args&&... args) {
        return std::make_unique<ProductType>(std::forward<Args>(args)...);
    }

    template<typename BoxType>
    static std::unique_ptr<BoxType> wrapProduct(std::unique_ptr<Product> product) {
        return std::make_unique<BoxType>(std::move(product));
    }
};

// Factory mode
class ProductFactory {
public:
    [[nodiscard]] static std::unique_ptr<Pizza> makePizza() {
        return std::make_unique<Pizza>("Beef Pizza", 10, "Classical");
    }

    [[nodiscard]] static std::unique_ptr<ToyCar> makeToyCar() {
        return std::make_unique<ToyCar>("Red Racing Car", 39.99, "Racing");
    }
};

// Factory mode
class WrapFactory {
public:
    template<typename BoxType, typename ProductType>
    std::unique_ptr<BoxType> wrapProduct(std::unique_ptr<ProductType> (*makeProduct)()) {
        std::cout << "--> Wrapping -->" << '\n';
        return std::make_unique<BoxType>(makeProduct());
    }
};

int main() {
    auto p_pizza = Factory::createProduct<Pizza>("Margherita", 12.99, "Classic");
    auto p_toy_car = Factory::createProduct<ToyCar>("Red Racing Car", 39.99, "Racing");

    p_pizza->printInfo();
    p_toy_car->printInfo();

    const auto p_pizza_box = Factory::wrapProduct<GourmetBox>(std::move(p_pizza));
    const auto p_toy_car_box = Factory::wrapProduct<PlayBox>(std::move(p_toy_car));

    p_pizza_box->printInfo();
    p_toy_car_box->printInfo();

    WrapFactory wrapFactory;
    auto wrappedPizzaBox = wrapFactory.wrapProduct<GourmetBox>(ProductFactory::makePizza);
    auto wrappedToyCarBox = wrapFactory.wrapProduct<PlayBox>(ProductFactory::makeToyCar);
    return 0;
}
