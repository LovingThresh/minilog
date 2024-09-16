#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using zoned_time_type = std::chrono::zoned_time<std::chrono::seconds>;

// 将系统当前的时间转换到指定时区的时间，默认是纽约时间
auto getXTime(const std::string_view X_zone = "America/New_York") -> zoned_time_type
{
    using namespace std::chrono;
    // 获取系统当前时间
    const zoned_time_type local_time = floor<seconds>(system_clock::now());

    // 定义X时区
    const auto x_tz = locate_zone(X_zone);

    return zoned_time_type{x_tz, local_time};
}

// 将输入的时区时间转换到对应的时区时间，默认是纽约时间
auto getXTime(const zoned_time_type Input_time, const std::string_view X_zone = "America/New_York") -> zoned_time_type
{
    using namespace std::chrono;
    // 定义X时区
    const auto x_tz = locate_zone(X_zone);
    return zoned_time_type{x_tz, Input_time};
}

void slow_motion()
{
    static int a[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // 生成 Γ(13) == 12! 的排列:
    while (std::ranges::next_permutation(a).found)
    {
    }
}

int main()
{
    using namespace std::chrono;

    // 设置会议时间
    const auto ny_time = zoned_time_type{"America/New_York", local_days{year{2023} / month{10} / day{5}} + 15h + 30min};

    // 将给定的纽约时间转换为伦敦和东京的时间
    const auto london_time = getXTime(ny_time, "Europe/London");
    const auto tokyo_time = getXTime(ny_time, "Asia/Tokyo");

    // 打印各时区对应的会议时间
    // std::cout << "会议时间安排如下：" << std::endl;
    // std::cout << "纽约时间: " << ny_time << std::endl;
    // std::cout << "伦敦时间: " << london_time << std::endl;
    // std::cout << "东京时间: " << tokyo_time << std::endl;

    // 输出本时刻对应NY时间
    std::cout << getXTime() << std::endl;

    const auto organizer_time_zone = current_zone(); // 假设为组织者的当前时区
    const auto meeting_time = zoned_time_type{organizer_time_zone, local_days{year{2024} / month{3} / day{15}} + 14h +
                                                                           30min}; // 设置会议时间为2024年3月15日14:30

    for (const std::vector<std::string> participant_time_zones = {"America/New_York", "Europe/London", "Asia/Tokyo"};
         const auto &tz_name: participant_time_zones)
    {
        auto const tz = locate_zone(tz_name);
        auto participant_time = zoned_time{tz, meeting_time}.get_local_time();
        std::cout << std::format("Meeting time in {}: {}\n", tz_name, participant_time);
    }

    using namespace std::literals; // 允许用字面量后缀，如 24h、1ms、1s。

    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

    const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);
    // std::cout << "24 小时前，时间是 "
    //           << std::put_time(std::localtime(&t_c), "%F %T。\n") << std::flush;

    const std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

    // std::cout << "不同的时钟无法比较：\n"
    //             "  系统时间：" << now.time_since_epoch() << "\n"
    //             "  稳定时间：" << start.time_since_epoch() << '\n';

    slow_motion();

    const auto end = std::chrono::steady_clock::now();
    // std::cout
    //     << "缓慢的计算花费了 "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(end - start) << " ≈ "
    //    << (end - start) / 1ms << "ms ≈ " // 几乎等价于以上形式，
    //    << (end - start) / 1s << "s。\n";  // 但分别使用毫秒和秒
    return 0;
}
