#include "../display.cpp"
#include <cassert>
#include <cstring>
static opxy::i32 display_words[2];
extern "C" opxy::i32 sdk_control_display(opxy::u32 index) { return index < 2 ? display_words[index] : 0; }
int main() {
    controlall::DisplayState state = {};
    controlall::Frame frame = {};
    opxy::draw::Input input = {};
    input.reduced_motion = 1;
    for (unsigned i = 0; i < 4; ++i) input.shown[i] = i;
    for (auto& value : input.slots) value = controlall::center;
    controlall::display_reset(state);
    controlall::display(state, input, frame);
    assert(std::strcmp(frame.header[0], "0") == 0);
    input.slots[0] = 0; input.slots[1] = controlall::maximum;
    input.slots[4] = controlall::center / 2;
    controlall::display(state, input, frame);
    assert(std::strcmp(frame.header[0], "-100") == 0);
    assert(std::strcmp(frame.header[1], "+100") == 0);
    for (unsigned i = 0; i < 4; ++i) input.shown[i] = i + 4;
    controlall::display(state, input, frame);
    assert(std::strcmp(frame.header[0], "-50") == 0);
    display_words[0] = 1; display_words[1] = 1;
    controlall::display(state, input, frame);
    assert(std::strcmp(frame.header[0], "25") == 0);
    assert(frame.nodes[17].shade == 15 && frame.nodes[16].shade == 3);
    assert(frame.nodes[18].x > 480 * opxy::draw::unit);
}
