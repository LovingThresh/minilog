#include "round_robin_example.h"

int main() {
    const std::vector<Process> processes {Process {"1"}, Process {"2"}, Process {"3"}};

    Scheduler scheduler {processes};
    for (int i {0}; i < 4; ++i) {scheduler.scheduleTimeSlice();}

    scheduler.removeProcess(processes[1]);
    std::cout << "Removed second peocess" << std::endl;

    for (int i {0}; i < 4; ++i) {scheduler.scheduleTimeSlice();}
}