#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "openmpblockdirichlet.h"
#include "openmpdirichlet.h"
#include "openmpwavedirichlet.h"
#include "sequentialdirichlet.h"

int main(int argc, char *argv[]) {
    // Seqential
    auto begin_time = std::chrono::steady_clock::now();
    auto result =
        sequentialDirichlet([](double x, double y) { return 100; },
                            [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {0, 1, 0, 1}, 5, 1);
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "========== Seqential ==========" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
              << std::endl;
    std::cout << result << std::endl;

    // Parallel
    begin_time = std::chrono::steady_clock::now();
    auto parallel_result = openmpDirichlet([](double x, double y) { return 100; },
                                           [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {0, 1, 0, 1},
                                           5, 1, std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << "========== Parallel ==========" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
              << std::endl;
    std::cout << parallel_result << std::endl;

    // Wave
    begin_time = std::chrono::steady_clock::now();
    auto wave_result = openmpWaveDirichlet([](double x, double y) { return 100; },
                                           [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {0, 1, 0, 1},
                                           5, 1, std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << "========== Wave ==========" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
              << std::endl;
    std::cout << wave_result << std::endl;

    // Block
    begin_time = std::chrono::steady_clock::now();
    auto block_result = openmpBlockDirichlet([](double x, double y) { return 100; },
                                             [](double x, double y) { return 200 - 100 * x * x - 200 * y; },
                                             {0, 1, 0, 1}, 5, 1, std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << "========== Block ==========" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
              << std::endl;
    std::cout << block_result << std::endl;

    result.save("network.bin");
    return 0;
}
