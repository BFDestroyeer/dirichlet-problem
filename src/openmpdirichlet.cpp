#include "openmpdirichlet.h"

Network openmpDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                        size_t node_count, double epsilon, int thread_count) {
    if (thread_count > 0) {
        omp_set_num_threads(thread_count);
    } else {
        throw std::runtime_error("Invalid thread count");
    }
    if (node_count < 2) {
        throw std::exception("Node count can't be lesser than 2");
    }

    Network u(node_count), f_network(node_count);
// Initialization
#pragma omp parallel for shared(u, node_count)
    for (size_t i = 0; i < node_count; i++) {
        double shift = i * (1 / static_cast<double>(node_count - 1));
        u(0, i) = g(0, shift);
        u(i, 0) = g(shift, 0);
        u(i, node_count - 1) = g(shift, 1);
        u(node_count - 1, i) = g(1, shift);
    }
#pragma omp parallel for shared(f_network, node_count)
    for (size_t i = 0; i < node_count; i++) {
        for (size_t j = 0; j < node_count; j++) {
            f_network(i, j) =
                f(i * (1 / static_cast<double>(node_count - 1)), j * (1 / static_cast<double>(node_count - 1)));
        }
    }

    // Calculation
    double delta, max_delta, h = 1 / static_cast<double>(node_count - 1);
    do {
        max_delta = 0;
#pragma omp parallel for shared(u, f, node_count) private(delta) reduction(max : max_delta)
        for (int32_t i = 1; i < node_count - 1; i++) {
            for (int32_t j = 1; j < node_count - 1; j++) {
                double temp = u(i, j);
                u(i, j) = 0.25 * (u(i - 1, j) + u(i + 1, j) + u(i, j - 1) + u(i, j + 1) - h * h * f_network(i, j));
                delta = std::fabs(temp - u(i, j));
                if (max_delta < delta) {
                    max_delta = delta;
                }
            }
        }
    } while (max_delta > epsilon);
    return u;
}
