#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <cctype>
#include <range/v3/view/chunk.hpp>
#include <cxxopts.hpp>
#include <format>

void hexdump(std::ranges::input_range auto const &range, const std::size_t width = 16) {
    using T = std::ranges::range_value_t<decltype(range)>;
    constexpr std::size_t fmtWidth = 2 * sizeof(T);
    std::size_t addr = 0;
    std::vector<char> saved;
    for (auto chunk: range | ranges::views::chunk(width)) {
        std::cout << std::format("{0:08X}", addr) << ' ';
        for (auto c: chunk) {
            std::cout << std::format(" {0:>{1}X}",
                                    static_cast<std::make_unsigned_t<T>>(c),
                                    fmtWidth);
            ++addr;
            if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>) {
                saved.push_back(static_cast<char>(c));
            }
        }
        if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>) {
            if (addr % width != 0) {
                for (std::size_t i = 0; i < (width - addr % width) * 3; i++) {
                    std::cout << ' ';
                }
            }
            std::cout << "  |";
            for (auto c: saved) {
                if (!std::isprint(c)) {
                    c = '.';
                }
                std::cout << c;
            }
            std::cout << "|";
            saved.clear();
        }
        std::cout << '\n';
    }
}

struct IstreamRange {
    std::istreambuf_iterator<char> b, e;

    explicit IstreamRange(std::istream &is)
    : b(std::istreambuf_iterator<char>(is))
    , e(std::istreambuf_iterator<char>()) {}

    auto begin() const {
        return b;
    }

    auto end() const {
        return e;
    }
};

int main(int argc, char **argv) {
    cxxopts::Options options("hexdump", "A command line utility for printing the hexadecimal and ASCII representation of a file.");
    options.add_options()("f,file", "Input file", cxxopts::value<std::string>()->default_value("-"));
    options.add_options()("w,width", "Number of bytes per line", cxxopts::value<std::size_t>()->default_value("16"));
    // add help
    options.add_options()("h,help", "Print usage");
    auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }
    auto path = args["file"].as<std::string>();
    auto width = args["width"].as<std::size_t>();
    if (path == "-") {
        hexdump(IstreamRange(std::cin), width);
        return 0;
    }
    std::ifstream ifs(path);
    if (!ifs.good()) {
        std::cerr << std::strerror(errno) << ": " << path << '\n';
        return errno;
    }
    hexdump(IstreamRange(ifs), width);

    return 0;
}