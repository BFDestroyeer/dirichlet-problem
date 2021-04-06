#pragma once

#include <cmath>
#include <functional>
#include <stdexcept>

#include <mpi.h>

#include "network.h"

Network mpiDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                     size_t node_count, double epsilon);
