#pragma once

#include <string>
#include "Observer.hpp"
#include "ServerEvent.hpp"
#include "Server.hpp"
#include "LED.hpp"
#include "Blinker.hpp"


/**
 * Communication handler.
 */
class Handler : public Observer<ServerEvent::EventInfo>
{
public:

    /**
     * Constructor.
     *
     * @param led1 First LED instance
     * @param led2 Second LED instance
     * @param port Port to listen on
     */
    Handler(LED* led1, LED* led2, unsigned int port);

    /**
     * Destructor.
     */
    ~Handler();

    /**
     * Initialize handler.
     * Start TCP server and let LEDs blink.
     *
     * @return True if succesfull, false otherwise
     */
    bool init();

    /**
     * Send a message to a connected client.
     * Message will be ignored if no client is connected.
     *
     * @param message Message to send
     * @return True if message was sent, false otherwise
     */
    bool send_message(const std::string& message);

    /**
     * Notification from TCP server about a new event.
     * 
     * @param event_id Event type
     * @param info Additional information related to event
     */
    void update(int event_id, ServerEvent::EventInfo info) override;

private:

    LED* mLed1; // LED1 instance
    LED* mLed2; // LED2 instance
    Blinker mBlinker1; // LED Blinker for first LED instance
    Blinker mBlinker2; // LED Blinker for second LED instance
    Server mServer; // TCP Server
};
