#pragma once

/**
 * Protocol message generation.
 */
class Protocol
{
public:

    /**
     * Generate a `value` message.
     *
     * @param value Value to add to message
     * @return Message
     */
    static std::string buildMessageValue(const float value);

    /**
     * Generate a `alarm` message.
     *
     * @param active Current alarm state. 1 if present, 0 if cleared.
     * @return Message
     */
    static std::string buildMessageAlarm(const bool active);
};