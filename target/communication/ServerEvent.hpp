#pragma once

#include <string>


/**
 * Event raised from TCP server.
 */
class ServerEvent
{
public:

    /**
     * Type of event.
     */
    enum EventType
    {
        CLIENT_CONNECTED,
        CLIENT_DISCONNECTED,
        RECEIVED_MESSAGE
    };

    /**
     * Additional event information.
     */
    struct EventInfo
    {
        std::string message;
    };

};
