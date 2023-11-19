#include <iostream>
#include <chrono>
#include <thread>

#include "hardware/LED.hpp"
#include "hardware/Button.hpp"
#include "hardware/DS18B20.hpp"
#include "communication/Protocol.hpp"
#include "Handler.hpp"

using namespace std::chrono_literals;


// Port for TCP Server to listen for incoming connections
static const unsigned int PORT = 4711;

// Max allowed temperature before switching to alarm state (in degree celsius)
static const float LIMIT_OVERTEMP = 25;

// Temperature sending interval
static const std::chrono::duration SEND_TEMP_INT = 1s;

// Max number of retries to measure temperature before re-initialize sensor
static const unsigned int SENSOR_MAX_RETRY = 5;


int main(void)
{
    // Initialize hardware
    auto led1 = LED("/dev/gpiochip1", 12); // green
    auto led2 = LED("/dev/gpiochip0", 26); // red
    auto button = Button("/dev/gpiochip1", 14);
    auto sensor = DS18B20("/sys/class/hwmon/hwmon0/temp1_input");

    try {
        led1.init();
        led2.init();
        button.init();
    } catch (...) {
        std::cerr << "Failed to initialize LEDs and button" << std::endl;
        return 1;
    }

    if (!sensor.init()) {
        std::cerr << "Failed to open sensor device" << std::endl;
        return 1;
    }

    // Initialize communication handler
    auto handler = Handler(&led1, &led2, PORT);
    if (!handler.init()) {
        std::cerr << "Failed to initialize handler" << std::endl;
        return 1;
    }

    // Main loop
    bool alarm_present = false;
    unsigned int sensor_error = 0;
    while (true) {
        auto time_before = std::chrono::system_clock::now();

        auto temp = sensor.measure();
        if (temp.has_value()) {
            sensor_error = 0;

            // Sleep for the right amount of time (roughly)
            auto time_after = std::chrono::system_clock::now();
            auto time_diff = SEND_TEMP_INT - (time_after - time_before);
            if (time_diff > 0ms) {
                std::this_thread::sleep_for(time_diff);
            }

            // Check if required to send temperature
            std::cout << "Send temperature message" << std::endl;
            auto message = Protocol::buildMessageValue(temp.value());
            handler.send_message(message);

            // Check for overtemperature
            if (temp.value() >= LIMIT_OVERTEMP) {
                alarm_present = true;
                std::cout << "Send alarm message" << std::endl;
                auto message = Protocol::buildMessageAlarm(true);
                handler.send_message(message);

                std::cout << "Wait for user input before continue" << std::endl;
                button.wait_for_input();
            } else if (alarm_present && temp.value() < LIMIT_OVERTEMP) {
                alarm_present = false;
                std::cout << "Send alarm cleared message" << std::endl;
                auto message = Protocol::buildMessageAlarm(false);
                handler.send_message(message);
            }
        } else {
            sensor_error += 1;
            std::cerr << "Failed to measure temperature" << std::endl;
        }

        // Take care in case the sensor stopped working
        if (sensor_error >= SENSOR_MAX_RETRY) {
            std::cerr << "Restart sensor" << std::endl;
            sensor.close();
            sensor.init();
            sensor_error = 0;
        }
    }

    return 0;
}
