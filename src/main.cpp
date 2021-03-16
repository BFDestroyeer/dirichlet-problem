#include "sequentialdirichlet.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto result = sequentialDirichlet([](double x, double y) { return x * y; },
                                      [](double x, double y) { return x + y; }, 10, 1.0);
    result.save("network.bin");
    return 0;
}
