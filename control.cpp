#include <opxy/track_control.hpp>
#include "controls.h"
namespace controlall {
struct ControlState { opxy::u32 absolute; };
// Project state outlasts track switches. The SDK zero-initializes new storage.
inline void control_reset(ControlState&) {}
inline opxy::u32 control_save(const ControlState& state, opxy::u8* data, opxy::u32 capacity) {
    if (capacity < 2) return 0;
    data[0] = 1;
    data[1] = state.absolute ? 1 : 0;
    return 2;
}
inline void control_load(ControlState& state, const opxy::u8* data, opxy::u32 bytes) {
    state.absolute = bytes == 2 && data[0] == 1 && data[1] == 1 ? 1 : 0;
}
inline void control(ControlState& state, const opxy::control::Input& input,
                    opxy::control::Overlay& overlay) {
    if (input.page_key == 2) state.absolute = 0;
    if (input.page_key == 3) state.absolute = 1;
    overlay.display[0] = static_cast<opxy::i32>(state.absolute);
    overlay.display[1] = static_cast<opxy::i32>(input.muted);
    if (input.muted || input.track >= opxy::control::all_tracks) return;
    for (opxy::u32 slot = 0; slot < 8; ++slot) {
        const opxy::i32 v = input.values[input.track][slot];
        if (!state.absolute && v == static_cast<opxy::i32>(center)) continue;
        for (opxy::u32 track = 0; track < opxy::control::tracks; ++track)
            opxy::control::add(overlay, track, slot, state.absolute ? (v < 0 ? 0 : v)
                : relative(input.values[track][slot], v));
    }
    overlay.mark = overlay.count ? 1 : 0;
}
}
