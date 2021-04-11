#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

#include "mpidirichlet.h"
#include "openmpblockdirichlet.h"
#include "openmpdirichlet.h"
#include "openmpwavedirichlet.h"
#include "sequentialdirichlet.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, process_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    uint64_t thread_count = std::thread::hardware_concurrency();

    if (argc < 2) {
        std::cout << "Wrong nuber of arguments" << std::endl;
        return 0;
    }
    std::array<std::array<int64_t, 3>, 3> results;
    std::string argument(argv[1]);
    std::string method_name;
    size_t node_count = 75;

    for (size_t i = 0; i < 3; i++) {
        node_count *= 2;
        for (size_t j = 0; j < 3; j++) {
            if (argument == "-seq") {
                method_name = "Sequential";
                auto begin_time = std::chrono::steady_clock::now();
                auto volatile network = sequentialDirichlet(
                    [](double x, double y) { return 100; },
                    [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {-1, 1, -1, 1}, node_count, 0.1);
                auto end_time = std::chrono::steady_clock::now();
                results[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            }
            if (argument == "-omp") {
                method_name = "OpenMP independent";
                auto begin_time = std::chrono::steady_clock::now();
                auto volatile network = openmpDirichlet([](double x, double y) { return 100; },
                                                        [](double x, double y) { return 200 - 100 * x * x - 200 * y; },
                                                        {-1, 1, -1, 1}, node_count, 0.1, thread_count);
                auto end_time = std::chrono::steady_clock::now();
                results[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            }
            if (argument == "-ompblock") {
                method_name = "OpenMP block";
                auto begin_time = std::chrono::steady_clock::now();
                auto volatile network =
                    openmpBlockDirichlet([](double x, double y) { return 100; },
                                         [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {-1, 1, -1, 1},
                                         node_count, 0.1, thread_count);
                auto end_time = std::chrono::steady_clock::now();
                results[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            }
            if (argument == "-ompwave") {
                method_name = "OpenMP wave";
                auto begin_time = std::chrono::steady_clock::now();
                auto volatile network =
                    openmpWaveDirichlet([](double x, double y) { return 100; },
                                        [](double x, double y) { return 200 - 100 * x * x - 200 * y; }, {-1, 1, -1, 1},
                                        node_count, 0.1, thread_count);
                auto end_time = std::chrono::steady_clock::now();
                results[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            }
            if (argument == "-mpi") {
                method_name = "MPI independent";
                thread_count = process_count;
                auto begin_time = std::chrono::steady_clock::now();
                auto volatile network = mpiDirichlet([](double x, double y) { return 100; },
                                                     [](double x, double y) { return 200 - 100 * x * x - 200 * y; },
                                                     {-1, 1, -1, 1}, node_count, 0.1);
                auto end_time = std::chrono::steady_clock::now();
                results[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            }
        }
    }
    MPI_Finalize();
    if (rank != 0) {
        return 0;
    }
    for (size_t i = 0; i < 3; i++) {
        std::sort(results[i].begin(), results[i].end());
    }
    std::cout << '|' << method_name << " (Best)" << '|' << thread_count << '|' << results[0][0] << "ms|"
              << results[1][0] << "ms|" << results[2][0] << "ms|" << std::endl;
    std::cout << '|' << method_name << " (Median)" << '|' << thread_count << '|' << results[0][1] << "ms|"
              << results[1][1] << "ms|" << results[2][1] << "ms|" << std::endl;
    return 0;
}
