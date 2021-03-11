#include "sequentialdirichlet.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto result =
        sequentialDirichlet([](double x, double y) { return x * y; }, [](double x, double y) { return x + y; }, 5, 1.0);
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            std::cout << result(i, j) << ' ';
        }
        std::cout << "\n";
    }
    return 0;
}
