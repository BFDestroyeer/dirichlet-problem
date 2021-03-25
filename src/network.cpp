#include "network.h"

Network::Network(size_t size, std::array<double, 4> ranges) {
    if (size == 0) {
        throw std::runtime_error("Can't create zero size network");
    }
    if ((ranges[0] > ranges[1]) || (ranges[2] > ranges[3])) {
        throw std::runtime_error("Wrong range");
    }
    m_size = size;
    m_ranges = ranges;
    array = new double[m_size * m_size]();
}

Network::Network(const Network &network) {
    m_size = network.m_size;
    std::memcpy(array, network.array, m_size * m_size * sizeof(double));
}

Network::Network(Network &&network) {
    m_size = network.m_size;
    array = network.array;

    network.m_size = 0;
    network.array = nullptr;
}

Network::~Network() {
    m_size = 0;
    if (array != nullptr) {
        delete[] array;
    }
}

const double Network::operator()(size_t row, size_t column) const {
    return array[row * m_size + column];
}

double &Network::operator()(size_t row, size_t column) {
    return array[row * m_size + column];
}

void Network::save(const std::string path) {
    std::ofstream file(path, std::ios::binary);
    file.write((char *)&m_size, sizeof(m_size));
    for (size_t i = 0; i < 4; i++) {
        file.write((char *)&m_ranges[i], sizeof(double));
    }
    file.write((char *)array, m_size * m_size * sizeof(double));
}

std::ostream &operator<<(std::ostream &out, const Network &network) {
    for (size_t i = 0; i < network.m_size; i++) {
        for (size_t j = 0; j < network.m_size; j++) {
            out << network(i, j) << ' ';
        }
        out << std::endl;
    }
    return out;
}
