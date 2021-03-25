#pragma once

#include <array>
#include <cmath>
#include <functional>
#include <stdexcept>

#include <omp.h>

#include "network.h"

Network openmpBlockDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                             std::array<double, 4> ranges, size_t node_count, double epsilon, int thread_count);
