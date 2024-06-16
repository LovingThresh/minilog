#include <fstream>
#include <boost/log/trivial.hpp>
#include "spdlog/spdlog.h"
#include <gsl/gsl_util>

void logger_init() {
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::set_level(spdlog::level::info); // Set global log level to debug
}

// void log_test() {
    // BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    // BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    // BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    // BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    // BOOST_LOG_TRIVIAL(error) << "An error severity message";
    // BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
    // spdlog::info("Welcome to spdlog!");
    // spdlog::error("Some error message with arg: {}", 1);
    //
    // spdlog::warn("Easy padding in numbers like {:08d}", 12);
    // spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    // spdlog::info("Support for floats {:03.2f}", 1.23456);
    // spdlog::info("Positional args are {1} {0}..", "too", "supported");
    // spdlog::info("{:<30}", "left aligned");
    //
    // spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // spdlog::debug("This message should be displayed..");
    //
    // change log pattern
    // spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // Note that this does not change the current log level, it will only
    // remove (depending on SPDLOG_ACTIVE_LEVEL) the call on the release code.
    // SPDLOG_TRACE("Some trace message with param {}", 42);
    // SPDLOG_DEBUG("Some debug message");
    // spdlog::warn("Easy padding in numbers like {:08d}", 12);
    // spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    // spdlog::info("Support for floats {:03.2f}", 1.23456);
    // spdlog::info("Positional args are {1} {0}..", "too", "supported");
    // spdlog::info("{:<30}", "left aligned");

// }


// 文件操作函数
void openFile(const char* filePath)
{
    spdlog::info("Attempting to open file: {}", filePath);

    // 使用gsl::final_action确保在作用域结束时打印消息，这里仅作演示，实际上ofstream会自动关闭
    gsl::final_action atExit{[] {
        spdlog::info("End of scope action executed.");
    }};

    try {
        std::ofstream file(filePath);
        spdlog::info("File object create successfully");
        file <<  "Hello, World!" << std::endl;
        file.close();
        spdlog::info("File written and closed successfully");
    } catch (const std::exception& e) {
        spdlog::error("Caught exception: {}", e.what());
    }
}

int main() {

    logger_init();
    openFile("example.txt");

    return 0;
}