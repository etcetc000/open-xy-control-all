#pragma once
#include <opxy/types.hpp>
namespace controlall {
const opxy::u32 center = 0x40000000u, maximum = 0x7fffffffu;
// Stock stores the default midpoint exactly as 16384 << 16.
inline opxy::i32 relative(opxy::i32 base, opxy::i32 value) {
    const opxy::u32 b = base < 0 ? 0 : static_cast<opxy::u32>(base);
    const opxy::u32 v = value < 0 ? 0 : static_cast<opxy::u32>(value);
    if (v >= center) {
        const opxy::u32 delta = (v - center) * 2;
        return static_cast<opxy::i32>(delta > maximum - b ? maximum : b + delta);
    }
    const opxy::u32 delta = (center - v) * 2;
    return static_cast<opxy::i32>(delta > b ? 0 : b - delta);
}
}
