#include <string>
#include <sstream>

#include "Protocol.hpp"


std::string Protocol::buildMessageValue(const float value)
{
    std::stringstream ss;
    ss << "VAL:" << value << "\n";
    return ss.str();
}

std::string Protocol::buildMessageAlarm(const bool active)
{
    std::stringstream ss;
    std::string tmp = active ? "1" : "0";
    ss << "ALM:" << tmp << "\n";
    return ss.str();
}