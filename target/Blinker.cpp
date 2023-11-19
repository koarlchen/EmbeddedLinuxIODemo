#include <iostream>
#include <pthread.h>

#include "Blinker.hpp"


using namespace std::chrono_literals;


Blinker::Blinker(LED* led) :
    mLed(led),
    mFreq(0),
    mBlinks(0),
    mStateActive(false)
{
}

Blinker::~Blinker()
{
    stop();
}

void Blinker::init()
{
    mThread = std::thread(&Blinker::run, this);
    auto handle = mThread.native_handle();
    pthread_setname_np(handle, "blinker");
    mThread.detach();
}

void Blinker::start(unsigned int freq, int blinks)
{
    stop();

    std::lock_guard<std::mutex> lk(mMtx);
    if (!mStateActive) {
        mFreq = freq;
        mBlinks = blinks;
        mStateActive = true;
        mCv.notify_one();
    }
}

void Blinker::stop()
{
    std::lock_guard<std::mutex> lk(mMtx);
    mStateActive = false;
    mCv.notify_one();
}

void Blinker::run()
{
    while (true) {

        // Wait for next blinking task
        if (!mStateActive) {
            std::unique_lock<std::mutex> lock(mMtx);
            mCv.wait(lock, [&]{ return mStateActive; });
        }

        auto waitTime = std::chrono::milliseconds(500 / mFreq);
        int blinks = 0;

        // Blink until finished
        while (true) {
            // Switch off
            mLed->set_value(false);
            {
                std::unique_lock<std::mutex> lock(mMtx);
                if (mCv.wait_for(lock, waitTime, [&]{ return !mStateActive; })) {
                    break;
                }
            }

            // Switch on
            mLed->set_value(true);
            {
                std::unique_lock<std::mutex> lock(mMtx);
                if (mCv.wait_for(lock, waitTime, [&]{ return !mStateActive; })) {
                    break;
                }
            }

            // Take care of number of configured blinks
            blinks += 1;
            if (mBlinks >= 0 && blinks >= mBlinks) {
                std::unique_lock<std::mutex> lock(mMtx);
                mStateActive = false;
                break;
            }
        }
    }
}