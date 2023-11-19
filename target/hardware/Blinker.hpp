#pragma once

#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <optional>
#include <condition_variable>

#include "LED.hpp"


/**
 * Thread-based LED blinker.
 */
class Blinker
{
public:

    /**
     * Constructor.
     *
     * @param led LED instance.
     */
    Blinker(LED* led);

    /**
     * Destructor.
     */
    ~Blinker();

    /**
     * Initialize blinker thread.
     */
    void init();

    /**
     * Let LED blink.
     *
     * @param freq Blinking frequency in Hz.
     * @param blinks Number > 0 defines how often the LED shall blink.
     */
    void start(unsigned int freq, int blinks=-1);

    /**
     * Stop blinking LED.
     */
    void stop();

private:

    /**
     * Thread function.
     * Takes care of blinking.
     */
    void run();

    LED* mLed; // LED instance
    std::atomic<unsigned int> mFreq; // Blinking frequency in Hz
    std::atomic<int> mBlinks; // Number of blinks before automatic end
    bool mStateActive; // Thread state
    std::thread mThread; // Thread handle
    std::mutex mMtx; // Mutex use for conditional wait
    std::condition_variable mCv; // Conditional
};