#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "openmpdirichlet.h"
#include "openmpwavedirichlet.h"
#include "sequentialdirichlet.h"

int main(int argc, char *argv[]) {
    auto begin_time = std::chrono::steady_clock::now();
    auto result = sequentialDirichlet([](double x, double y) { return 100; },
                                      [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, 100, 1);
    auto end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;
    begin_time = std::chrono::steady_clock::now();
    auto parallel_result = openmpDirichlet([](double x, double y) { return 100; },
                                           [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, 100, 1,
                                           std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;
    begin_time = std::chrono::steady_clock::now();
    auto wave_result = openmpWaveDirichlet([](double x, double y) { return 100; },
                                           [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, 100, 1,
                                           std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;

    result.save("network.bin");
    return 0;
}
