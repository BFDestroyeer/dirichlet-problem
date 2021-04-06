#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Network of double values
 *
 */
class Network {
private:
    size_t m_size;
    double *array;

public:
    Network() = delete;

    /**
     * @brief Construct a new Network object
     *
     * @param size Count of nodes per each dimension
     */
    Network(size_t size);

    /**
     * @brief Construct a new Network object
     *
     * @param network
     */
    Network(const Network &network);

    /**
     * @brief Construct a new Network object
     *
     * @param network
     */
    Network(Network &&network);

    /**
     * @brief Destroy the Network object
     *
     */
    ~Network();

    /**
     * @brief Give acess to network values
     *
     * @param row Row number
     * @param column Column number
     * @return double Network value
     */
    const double operator()(size_t row, size_t column) const;

    /**
     * @brief Give acess to network values
     *
     * @param row Row number
     * @param column Column number
     * @return double& Network value
     */
    double &operator()(size_t row, size_t column);

    /**
     * @brief Export network to file
     *
     * @param path Path to file
     */
    void save(const std::string path);

    /**
     * @brief Return raw array pointer
     *
     * @return double Raw array pointer;
     */
    double *data();

    friend std::ostream &operator<<(std::ostream &out, const Network &network);
};
