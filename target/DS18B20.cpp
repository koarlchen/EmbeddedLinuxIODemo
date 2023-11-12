#include <iostream>

#include "DS18B20.hpp"


DS18B20::DS18B20(const std::string device) :
    mDeviceName(device)
{
}

DS18B20::~DS18B20()
{
    close();
}

void DS18B20::close()
{
    mDevice.close();
}

bool DS18B20::init()
{
    mDevice.open(mDeviceName);
    return mDevice.is_open();
}

std::optional<float> DS18B20::measure()
{
    std::string tmp;

    mDevice.seekg(0);
    mDevice >> tmp;
    if (tmp != "") {
        float value = std::stof(tmp) / 1000.0;
        return value;
    }

    return {};
}