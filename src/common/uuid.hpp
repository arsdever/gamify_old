#pragma once

#include <random>

namespace g::common
{

using uuid = std::string;

inline std::string generate_uuid()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);

    static char const* lut = "0123456789abcdef";

    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    for (auto i = 0; i < uuid.size(); ++i)
    {
        if (uuid[ i ] == 'x')
        {
            uuid[ i ] = lut[ dis(gen) ];
        }
        else if (uuid[ i ] == 'y')
        {
            uuid[ i ] = lut[ (dis(gen) & 0x03) | 0x08 ];
        }
    }

    return uuid;
}

} // namespace g::common
