#include <chrono>
#include <iostream>
#include <thread>

#include "openmpdirichlet.h"
#include "sequentialdirichlet.h"

int main(int argc, char *argv[]) {
    auto begin_time = std::chrono::steady_clock::now();
    auto result = sequentialDirichlet([](double x, double y) { return x * y; },
                                      [](double x, double y) { return x + y; }, 10000, 1.0);
    auto end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;
    begin_time = std::chrono::steady_clock::now();
    auto paraller_result =
        openmpDirichlet([](double x, double y) { return x * y; }, [](double x, double y) { return x + y; }, 10000, 1.0,
                        std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;
    paraller_result.save("network.bin");
    return 0;
}
