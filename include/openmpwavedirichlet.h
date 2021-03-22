#pragma once

#include <cmath>
#include <functional>
#include <stdexcept>

#include <omp.h>

#include "network.h"

Network openmpWaveDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                            size_t node_count, double epsilon, int thread_count);
