#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "mpidirichlet.h"
#include "openmpblockdirichlet.h"
#include "openmpdirichlet.h"
#include "openmpwavedirichlet.h"
#include "sequentialdirichlet.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    auto begin_time = std::chrono::steady_clock::now();
    auto result =
        sequentialDirichlet([](double x, double y) { return 100; },
                            [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {-1, 1, -1, 1}, 100, 0.1);
    auto end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;
    /*begin_time = std::chrono::steady_clock::now();
    auto parallel_result = openmpDirichlet([](double x, double y) { return 100; },
                                           [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, 1000, 0.1,
                                           std::thread::hardware_concurrency());
    end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;*/
    begin_time = std::chrono::steady_clock::now();
    auto block_result =
        mpiDirichlet([](double x, double y) { return 100; },
                     [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {-1, 1, -1, 1}, 100, 0.1);
    end_time = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << std::endl;

    result.save("network.bin");
    MPI_Finalize();
    return 0;
}
