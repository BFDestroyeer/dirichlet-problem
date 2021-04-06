#include "sequentialdirichlet.h"

Network sequentialDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                            std::array<double, 4> ranges, size_t node_count, double epsilon) {
    if (node_count < 2) {
        throw std::exception("Node count can't be lesser than 2");
    }
    Network u(node_count, ranges), f_network(node_count, ranges);
    // Initialization
    for (size_t i = 0; i < node_count; i++) {
        double shift_x = ranges[0] + i * ((ranges[1] - ranges[0]) / static_cast<double>(node_count - 1));
        double shift_y = ranges[2] + i * ((ranges[3] - ranges[2]) / static_cast<double>(node_count - 1));
        u(0, i) = g(0, shift_y);
        u(i, 0) = g(shift_x, 0);
        u(i, node_count - 1) = g(shift_x, 1);
        u(node_count - 1, i) = g(1, shift_y);
    }
    for (size_t i = 0; i < node_count; i++) {
        for (size_t j = 0; j < node_count; j++) {
            f_network(i, j) = f(ranges[0] + i * ((ranges[1] - ranges[0]) / static_cast<double>(node_count - 1)),
                                ranges[2] + j * ((ranges[3] - ranges[2]) / static_cast<double>(node_count - 1)));
        }
    }

    // Calculation
    double delta, max_delta, h = 1 / static_cast<double>(node_count - 1);
    do {
        max_delta = 0;
        for (size_t i = 1; i < node_count - 1; i++) {
            for (size_t j = 1; j < node_count - 1; j++) {
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
