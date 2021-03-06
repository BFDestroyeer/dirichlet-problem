#include "mpidirichlet.h"

Network mpiDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                     std::array<double, 4> ranges, size_t node_count, double epsilon) {
    if (node_count < 2) {
        throw std::runtime_error("Node count can't be lesser than 2");
    }

    Network u(node_count, ranges), f_network(node_count, ranges), result(node_count, ranges);
    int begin_row, end_row, rows_in_part;
    int rank, process_count;
    int *counts = new int[process_count];
    int *displs = new int[process_count];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    // MPI variables initialization
    rows_in_part = node_count / process_count;
    begin_row = (rank != 0 ? rank * rows_in_part : 1);
    end_row = (rank != process_count - 1 ? (rank + 1) * rows_in_part : node_count - 1);

    // MPI displs and counts initialization
    counts[0] = rows_in_part * node_count;
    displs[0] = 0;
    for (int i = 1; i < process_count - 1; i++) {
        counts[i] = rows_in_part * node_count;
        displs[i] = i * rows_in_part * node_count;
    }
    counts[process_count - 1] = (node_count - (process_count - 1) * rows_in_part) * node_count;
    displs[process_count - 1] = (process_count - 1) * rows_in_part * node_count;

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
    double delta, max_delta, local_delta, h = 1 / static_cast<double>(node_count - 1);
    do {
        local_delta = 0;
        max_delta = 0;
        for (int i = begin_row; i < end_row; i++) {
            for (size_t j = 1; j < node_count - 1; j++) {
                double temp = u(i, j);
                u(i, j) = 0.25 * (u(i - 1, j) + u(i + 1, j) + u(i, j - 1) + u(i, j + 1) - h * h * f_network(i, j));
                delta = std::fabs(temp - u(i, j));
                if (local_delta < delta) {
                    local_delta = delta;
                }
            }
        }
        MPI_Allreduce(&local_delta, &max_delta, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (max_delta > epsilon) {
            MPI_Status status;
            if (rank == 0) {
                MPI_Send(u.data() + (end_row - 1) * node_count, node_count, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(u.data() + end_row * node_count, node_count, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
            } else if (rank != process_count - 1) {
                MPI_Sendrecv(u.data() + (end_row - 1) * node_count, node_count, MPI_DOUBLE, rank + 1, 0,
                             u.data() + (begin_row - 1) * node_count, node_count, MPI_DOUBLE, rank - 1, 0,
                             MPI_COMM_WORLD, &status);
                MPI_Sendrecv(u.data() + begin_row * node_count, node_count, MPI_DOUBLE, rank - 1, 0,
                             u.data() + end_row * node_count, node_count, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD,
                             &status);
            } else {
                MPI_Recv(u.data() + (begin_row - 1) * node_count, node_count, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD,
                         &status);
                MPI_Send(u.data() + begin_row * node_count, node_count, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            }
        } else {
            if (rank == 0) {
                begin_row = 0;
            } else if (rank == process_count - 1) {
                end_row++;
            }
            MPI_Gatherv(u.data() + begin_row * node_count, (end_row - begin_row) * node_count, MPI_DOUBLE,
                        result.data(), counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    } while (max_delta > epsilon);
    delete[] counts;
    delete[] displs;

    return result;
}
