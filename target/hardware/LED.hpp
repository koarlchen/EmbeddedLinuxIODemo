#pragma once

#include <optional>
#include <gpiod.hpp>

/**
 * LED Abstraction.
 */
class LED
{
public:

    /**
     * Constrctor.
     *
     * @param dev Device to which the LED is connected to (e.g. /dev/gpiochip1)
     * @param line Line to which the LED is connected to (e.g. 26)
     */   
    LED(const std::string dev, const unsigned int line);

    /**
     * Desturctor.
     * Close connection to device.
     */
    ~LED();

    /**
     * Initialize connection to device.
     */
    void init();

    /**
     * Set value of LED.
     * 
     * @param onoff True to switch LED on, false to switch LED off
     */
    void set_value(const bool onoff);

private:

    std::string mChip; // Chip / Device
    unsigned int mLine; // Line on chip
    std::optional<gpiod::line_request> mLineRequest; // Line handle

};