#include "display.h"
#include "controls.h"
#include <opxy/track_control.hpp>
namespace controlall {
inline void display_reset(DisplayState& state) { state.header.reset(); }
inline void display(DisplayState& state, const opxy::draw::Input& input, Frame& frame) {
    using namespace opxy::draw;
    const bool absolute = opxy::control::from_control(0) != 0;
    const bool muted = opxy::control::from_control(1) != 0;
    const char* const titles[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    char values[8][header_capacity];
    for (opxy::u32 i = 0; i < 8; ++i) {
        const opxy::u32 v = input.slots[i] < 0 ? 0 : static_cast<opxy::u32>(input.slots[i]);
        if (absolute) header_amount(values[i], header_capacity, static_cast<opxy::i32>(v));
        else {
            const opxy::u32 distance = v >= center ? v - center : center - v;
            const opxy::u32 magnitude = distance * 2;
            char amount[header_capacity];
            header_amount(amount, header_capacity, static_cast<opxy::i32>(magnitude > maximum ? maximum : magnitude));
            if (amount[0] == '0') copy(values[i], header_capacity, "0");
            else {
                values[i][0] = v >= center ? '+' : '-';
                copy(values[i] + 1, header_capacity - 1, amount);
            }
        }
        const opxy::i32 height = static_cast<opxy::i32>((v >> 16) * 80 >> 15);
        place(frame, 8 + i, (30 + 60 * i) * unit, (150 - height) * unit, unit,
              muted ? 5 : 13, fill_and_stroke);
    }
    page_header(state.header, input, frame, titles, values, 0);
    place(frame, 16, 0, 0, unit, absolute ? 3 : 15, fill_and_stroke);
    place(frame, 17, 0, 0, unit, absolute ? 15 : 3, fill_and_stroke);
    place(frame, 18, muted ? 600 * unit : 0, 0, unit, 0, fill_and_stroke);
}
}
