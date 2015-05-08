#pragma once

namespace ivi {

template<typename EnumType>
bool hasFlag(EnumType value, EnumType flag)
{
    return (static_cast<int>(value) & static_cast<int>(flag));
}

}
