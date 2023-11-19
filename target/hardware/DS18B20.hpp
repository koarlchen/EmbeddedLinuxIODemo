#pragma once

#include <string>
#include <fstream>
#include <optional>


/**
 * DS18B20 One-Wire Temperature Sensor Abstraction.
 */
class DS18B20
{
public:

    /**
     * Constructor.
     *
     * @param device Device to read temperature from
     */
    DS18B20(const std::string device);

    /**
     * Destructor.
     * Close connection to device.
     */
    ~DS18B20();

    /**
     * Close connection to device.
     */
    void close();

    /**
     * Initialize connection to device.
     *
     * @return True if sucessfull, false otherwise.
     */
    bool init();

    /**
     * Get value from sensor.
     *
     * @return Value or none, if the measurement failed.
     */
    std::optional<float> measure();

private:

    std::string mDeviceName; // Device file
    std::ifstream mDevice; // Device file handle
};