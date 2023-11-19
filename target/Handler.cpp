#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <sstream>

#include "Handler.hpp"

// Number of blinks for each sent/received message
static const int NUM_BLINK_MESSAGE = 2;

// Blink frequency for a sent/received message (in Hz)
static const unsigned int FREQ_MESSAGE = 4;

// Blink frequency on if no client is connected (in Hz)
static const unsigned int FREQ_DISCONNECTED = 2;


Handler::Handler(LED* led1, LED* led2, unsigned int port) :
    mLed1(led1),
    mLed2(led2),
    mBlinker1(led1),
    mBlinker2(led2),
    mServer(port)
{
}

Handler::~Handler()
{
    mServer.closeServer();
    mBlinker1.stop();
    mBlinker2.stop();
}

bool Handler::init()
{
    mServer.attachObserver(*this);
    mServer.setReceiver(true);
    if (!mServer.start()) {
        return false;
    }

    mBlinker1.init();
    mBlinker2.init();
    mBlinker1.start(FREQ_DISCONNECTED);

    return true;
}

bool Handler::send_message(const std::string& message)
{
    if (mServer.clientConnected()) {
        mBlinker2.start(FREQ_MESSAGE, NUM_BLINK_MESSAGE);
        return mServer.sendString(message);
    }

    return false;
}

void Handler::update(int event_id, ServerEvent::EventInfo evt)
{
    switch (event_id)
    {
        case ServerEvent::CLIENT_CONNECTED:
            std::cout << "Client connected" << std::endl;
            mBlinker1.stop();
            mLed1->set_value(false);
            mLed2->set_value(true);
            break;
        case ServerEvent::CLIENT_DISCONNECTED:
            std::cout << "Client disconnected" << std::endl;
            mBlinker2.stop();
            mLed2->set_value(false);
            mBlinker1.start(FREQ_DISCONNECTED);
            break;
        case ServerEvent::RECEIVED_MESSAGE:
            std::cout << "Received message: " << evt.message << std::endl;
            mBlinker2.start(FREQ_MESSAGE, NUM_BLINK_MESSAGE);
            break;
        default:
            std::cout << "Unhandled event" << std::endl;
    }
}