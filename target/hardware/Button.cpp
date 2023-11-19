#include <iostream>
#include <chrono>
#include <gpiod.hpp>

#include "Button.hpp"


using namespace std::chrono_literals;

Button::Button(const std::string dev, const unsigned int line) :
    mChip(dev),
    mLine(line)
{
}

Button::~Button()
{
    if (mLineRequest.has_value()) {
        mLineRequest.value().release();
        mLineRequest = {};
    }
}

void Button::init()
{
    gpiod::line::offsets offsets;
    offsets.push_back(mLine);

    mLineRequest = gpiod::chip(mChip)
        .prepare_request()
        .add_line_settings(
            offsets,
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::INPUT)
                .set_edge_detection(gpiod::line::edge::RISING)
                .set_debounce_period(2ms)
        )
        .do_request();
}

void Button::wait_for_input()
{
    gpiod::edge_event_buffer buffer;

    // Read all pending events
    if (mLineRequest->wait_edge_events(1ns)) {
        mLineRequest->read_edge_events(buffer);
    }

    // Wait for new input
    mLineRequest->read_edge_events(buffer);
}
