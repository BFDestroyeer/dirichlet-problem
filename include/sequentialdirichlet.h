#pragma once

#include <array>
#include <cmath>
#include <functional>
#include <stdexcept>

#include "network.h"

Network sequentialDirichlet(const std::function<double(double, double)> &f, std::function<double(double, double)> g,
                            std::array<double, 4> ranges, size_t node_count, double epsilon);
