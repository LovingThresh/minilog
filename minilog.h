#ifndef MINILIG_H
#define MINILIG_H

#include <source_location> // 提供了一种获取当前源代码的文件名、行号和函数名的方法，有助于跟踪日志消息的来源
#include <chrono> // 用于处理日期和时间
#include <iostream> // 提供标准输入输出流
#include <fstream> // 用于文件输入输出操作
#include <cstdint> // 定义了固定宽度的整数类型
#include <format>
namespace minilog {

#define MINILOG_FOREACH_LOG_LEVEL(f) \
    f(trace) \
    f(debug) \
    f(info) \
    f(critical) \
    f(warn) \
    f(error) \
    f(fatal)

enum class log_level : std::uint8_t {
#define FUNCTION(name) name, //
    MINILOG_FOREACH_LOG_LEVEL(FUNCTION)
#undef FUNCTION
};
// 等价于下面的展开式
// 枚举常用于替代硬编码的数字，提供更具描述性的名称
// 对于 enum class 定义的枚举类型，更倾向于将 trace、debug、info 等称为枚举值或枚举成员
/* enum class log_level : std::uint8_t {
    trace,
    debug,
    info,
    critical,
    warn,
    error,
    fatal
};
*/
namespace details {
    inline constexpr char k_level_ansi_colors[static_cast<std::uint8_t>(log_level::fatal) + 1][8] = {
        "\x1B[37m", // 白色
        "\x1B[35m", // 紫色
        "\x1B[32m", // 绿色
        "\x1B[34m", // 蓝色
        "\x1B[33m", // 黄色
        "\x1B[31m", // 红色
        "\x1B[31;1m", // 亮红色
    };
    inline constexpr char k_reset_ansi_color[4] = "\x1B[m";

#define MINILOG_IF_HAS_ANSI_COLORS(x) x

    inline std::string log_level_name(const log_level lev) {
        switch (lev) {
#define FUNCTION(name) case log_level::name: return #name;
            MINILOG_FOREACH_LOG_LEVEL(FUNCTION)
#undef FUNCTION
    }
        return "unknown";
    }

    /*
    const char* getLogLevelName(log_level level) {
    switch (level) {
        case log_level::trace: return "trace";
        case log_level::debug: return "debug";
        case log_level::info: return "info";
        case log_level::critical: return "critical";
        case log_level::warn: return "warn";
        case log_level::error: return "error";
        case log_level::fatal: return "fatal";
        default:
        return "unknown";
        }
    }
    */

    inline log_level log_level_from_name(const std::string &lev) {
#define FUNCTION(name) if (lev == #name) return log_level::name;
        MINILOG_FOREACH_LOG_LEVEL(FUNCTION)
#undef FUNCTION
        return log_level::info;
    }
    template <class T>
    struct with_source_location {

    public:
        template <class U> requires std::constructible_from<T, U>
        consteval with_source_location(U && inner, std::source_location loc = std::source_location::current()):
        inner(std::forward<U>(inner)), loc(loc) {}
        constexpr T const &format() const {return inner;}
        [[nodiscard]] constexpr std::source_location const &location() const {return loc;}

    private:
        T inner;
        std::source_location loc;
    };

    inline log_level g_max_level = [] () -> log_level {
        if (const auto lev = std::getenv("MINILOG_LEVEL")) {
            return details::log_level_from_name(lev);
        }
        return log_level::info;
    } ();

    inline std::ofstream g_log_file = [] () -> std::ofstream {
        if (const auto path = std::getenv("MINILOG_FILE")) {
            return std::ofstream(path, std::ios::app);
        }
        return std::ofstream();
    } ();

    inline void output_log(log_level lev, std::string msg, std::source_location const &loc) {
        auto now = std::chrono::zoned_time(
        std::chrono::current_zone(), std::chrono::system_clock::now()
    );
        msg = std::format("{} {}:{} [{}] {}", now, loc.file_name(), loc.line(), details::log_level_name(lev), msg);
        if (g_log_file) {
            g_log_file << msg + '\n';
        }
        if (lev >= g_max_level) {
            std::cout << MINILOG_IF_HAS_ANSI_COLORS(k_level_ansi_colors[static_cast<std::uint8_t>( lev )]) << msg
            << MINILOG_IF_HAS_ANSI_COLORS(k_reset_ansi_color) << '\n';
        }
    }
}

    inline void set_log_file(const std::string& path) {
        details::g_log_file = std::ofstream(path, std::ios::app);
}
    inline void set_log_level(const log_level lev) {
    details::g_max_level = lev;
}
    template <typename... Args>
    void generic_log(log_level lev, details::with_source_location<std::format_string<Args...>> fmt, Args &&...args) {
        auto const &loc = fmt.location();
        auto msg = std::vformat(fmt.format().get(), std::make_format_args(args...));
        details::output_log(lev, std::move(msg), loc);
}
#define FUNCTION(name) \
template <typename... Args> \
void log_##name(details::with_source_location<std::format_string<Args...>> fmt, Args &&...args) { \
    return generic_log(log_level::name, std::move(fmt), std::forward<Args>(args)...); \
}
    MINILOG_FOREACH_LOG_LEVEL(FUNCTION)
#undef FUNCTION

#define MINILOG_P(x) ::minilog::log_debug(#x "={}", x)
}

#endif //MINILIG_H
