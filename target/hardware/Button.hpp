#pragma once

#include <optional>
#include <gpiod.hpp>
#include <string>


/**
 * Button abstraction.
 */
class Button
{
public:

    /**
     * Constructor.
     *
     * @param dev Device to which the LED is connected to (e.g. /dev/gpiochip1)
     * @param line Line to which the LED is connected to (e.g. 26)
     */
    Button(const std::string dev, const unsigned int line);

    /**
     * Desturctor.
     * Close connection to device.
     */
    ~Button();

    /**
     * Initialize connection to device.
     */
    void init();

    /**
     * Wait for user input (button press).
     * Blocks until button was pressed.
     */
    void wait_for_input();

private:

    std::string mChip; // Chip / Device
    unsigned int mLine; // Line on chip
    std::optional<gpiod::line_request> mLineRequest; // Line handle

};