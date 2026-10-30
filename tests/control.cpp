#include "../control.cpp"
#include <cassert>
using namespace opxy;
using namespace controlall;
static opxy::control::Input input = {};
static opxy::control::Overlay tick(ControlState& state) {
    opxy::control::Overlay result = {};
    controlall::control(state, input, result);
    return result;
}
int main() {
    ControlState state = {};
    input.track = 12;
    for (unsigned slot = 0; slot < 8; ++slot) input.values[12][slot] = center;
    for (unsigned track = 0; track < 8; ++track)
        for (unsigned slot = 0; slot < 8; ++slot)
            input.values[track][slot] = (track + 1) * 0x08000000;
    assert(tick(state).count == 0);
    input.values[12][0] = center + 0x08000000;
    auto result = tick(state);
    assert(result.count == 8 && result.mark == 1 && result.capture == 0);
    for (unsigned t = 0; t < 8; ++t) {
        assert(result.entries[t].track == t && result.entries[t].index == 0);
        assert(result.entries[t].value == input.values[t][0] + 0x10000000);
    }
    // Repeated ticks never compound offsets. Changes to the underlying track
    // (including its own step locks) contribute on the next tick.
    assert(tick(state).entries[0].value == result.entries[0].value);
    input.values[0][0] = 0x30000000;
    assert(tick(state).entries[0].value == 0x40000000);
    input.values[12][0] = center - 0x08000000;
    assert(tick(state).entries[0].value == 0x20000000);
    assert(relative(0x70000000, maximum) == static_cast<i32>(maximum));
    assert(relative(0x10000000, 0) == 0);
    assert(relative(123, center) == 123);
    input.page_key = 3;
    result = tick(state);
    assert(state.absolute == 1 && result.count == 64);
    for (unsigned slot = 0; slot < 8; ++slot)
        for (unsigned t = 0; t < 8; ++t)
            assert(result.entries[slot * 8 + t].value == input.values[12][slot]);
    // Simulate sequencer delivery of a new eight-value snapshot.
    input.page_key = 0;
    for (unsigned slot = 0; slot < 8; ++slot) input.values[12][slot] = slot * 0x10000000;
    result = tick(state);
    for (unsigned slot = 0; slot < 8; ++slot)
        assert(result.entries[slot * 8].value == static_cast<i32>(slot * 0x10000000));
    input.muted = 1;
    result = tick(state);
    assert(result.count == 0 && result.mark == 0 && result.capture == 0 && result.display[1] == 1);
    input.muted = 0;
    assert(tick(state).count == 64);
    u8 saved[2] = {};
    assert(control_save(state, saved, 1) == 0);
    assert(control_save(state, saved, 2) == 2);
    state.absolute = 0;
    control_load(state, saved, 2);
    assert(state.absolute == 1);
    control_reset(state);
    assert(state.absolute == 1);
    control_load(state, nullptr, 0);
    assert(state.absolute == 0);
    for (unsigned version = 0; version < 4; ++version)
        for (unsigned mode = 0; mode < 4; ++mode) {
            saved[0] = version; saved[1] = mode;
            control_load(state, saved, 2);
            assert(state.absolute == (version == 1 && mode == 1));
        }
    input.page_key = 2;
    tick(state);
    assert(state.absolute == 0);
}
