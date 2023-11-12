#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <gpiod.hpp>

#include "LED.hpp"


LED::LED(const std::string dev, const unsigned int line) :
    mChip(dev),
    mLine(line)
{
}

LED::~LED()
{
    if (mLineRequest.has_value()) {
        mLineRequest.value().release();
        mLineRequest = {};
    }
}

void LED::init()
{
    gpiod::line::offsets offsets;

    offsets.push_back(mLine);

    mLineRequest = gpiod::chip(mChip)
        .prepare_request()
        .add_line_settings(
            offsets,
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::OUTPUT)
        )
        .do_request();
}

void LED::set_value(const bool onoff)
{
    gpiod::line::values values;
    values.push_back(onoff ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
    mLineRequest->set_values(values);
}
