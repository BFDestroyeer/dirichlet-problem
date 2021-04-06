#include "openmpblockdirichlet.h"
#include <iostream>

Network openmpBlockDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
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
    size_t block_size = 40;
    int32_t nb = (node_count - 2) / block_size + 1 + ((node_count - 2) % block_size != 0 ? 1 : 0);
    // Calculation
    double delta, max_delta, max_delta_1, max_delta_2, h = 1 / static_cast<double>(node_count - 1);
    do {
        max_delta_1 = 0;
        max_delta_2 = 0;
        for (int32_t nx = 0; nx < nb; nx++) {
            // Wave increase
#pragma omp parallel for shared(u, f, node_count) private(delta) reduction(max : max_delta_1)
            for (int32_t i = 0; i < nx + 1; i++) {
                int32_t j = nx - i;
                for (int32_t k = 1 + block_size * i; k < 1 + block_size * (i + 1) && k < node_count - 1; k++) {
                    for (int32_t l = 1 + block_size * j; l < 1 + block_size * (j + 1) && l < node_count - 1; l++) {
                        double temp = u(k, l);
                        u(k, l) =
                            0.25 * (u(k - 1, l) + u(k + 1, l) + u(k, l - 1) + u(k, l + 1) - h * h * f_network(k, l));
                        delta = std::fabs(temp - u(k, l));
                        if (max_delta_1 < delta) {
                            max_delta_1 = delta;
                        }
                    }
                }
            }
        }

        for (int32_t nx = nb - 2; nx > -1; nx--) {
            // Wave increase
#pragma omp parallel for shared(u, f, node_count) private(delta) reduction(max : max_delta_1)
            for (int32_t i = nb - nx - 1; i < nb; i++) {
                int32_t j = 2 * (nb - 1) - nx - i;
                for (int32_t k = 1 + block_size * i; k < 1 + block_size * (i + 1) && k < node_count - 1; k++) {
                    for (int32_t l = 1 + block_size * j; l < 1 + block_size * (j + 1) && l < node_count - 1; l++) {
                        double temp = u(k, l);
                        u(k, l) =
                            0.25 * (u(k - 1, l) + u(k + 1, l) + u(k, l - 1) + u(k, l + 1) - h * h * f_network(k, l));
                        delta = std::fabs(temp - u(k, l));
                        if (max_delta_1 < delta) {
                            max_delta_1 = delta;
                        }
                    }
                }
            }
        }
        max_delta = (max_delta_1 > max_delta_2 ? max_delta_1 : max_delta_2);
    } while (max_delta > epsilon);
    return u;
}
