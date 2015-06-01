#pragma once

namespace ivi_main_loop {

template<typename EnumType>
bool hasFlag(EnumType value, EnumType flag)
{
    return (static_cast<int>(value) & static_cast<int>(flag));
}

}
